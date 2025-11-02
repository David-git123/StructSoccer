#include "../include/menu.h"

#ifndef PI
#define PI 3.14159265358979323846f
#endif

static float clampf(float x, float a, float b) { return (x < a) ? a : (x > b) ? b : x; }

// botao do menu
static bool DrawMenuButton(Rectangle r, const char* label, const char* desc, bool selected, float dt) {
    static float pulse = 0.0f;
    pulse += dt * 2.5f;
    float pulseAmt = selected ? (0.02f * sinf(pulse)) : 0.0f;

    bool hovered = CheckCollisionPointRec(GetMousePosition(), r);
    bool pressedMouse = hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    float k = selected ? 1.0f : hovered ? 0.7f : 0.45f;
    Color base  = (Color){ (unsigned char)(25 + 80*k), (unsigned char)(120 + 50*k), (unsigned char)(200*k), 180 };
    DrawRectangleRounded(r, 0.20f, 12, Fade(base, 0.85f));
    DrawRectangleRoundedLines(r, 0.20f, 12, (selected || hovered) ? Fade(RAYWHITE, 0.85f) : Fade(LIGHTGRAY, 0.6f));

    if (selected || hovered) {
        Rectangle glow = r;
        glow.x -= 6; glow.y -= 6; glow.width += 12; glow.height += 12;
        DrawRectangleRoundedLines(glow, 0.25f, 12, Fade((Color){255,255,255,60}, 0.35f));
    }

    int fontSizeTitle = (int)(32.0f + 32.0f*pulseAmt);
    int fontSizeDesc  = 18;

    Vector2 tsize = MeasureTextEx(GetFontDefault(), label, fontSizeTitle, 1.0f);
    Vector2 posTitle = (Vector2){ r.x + 24, r.y + r.height*0.5f - tsize.y*0.65f };

    DrawTextEx(GetFontDefault(), label, (Vector2){ posTitle.x+1, posTitle.y+1 }, fontSizeTitle, 1.0f, (Color){0,0,0,120});
    DrawTextEx(GetFontDefault(), label, posTitle, fontSizeTitle, 1.0f, RAYWHITE);

    if (desc && desc[0] != '\0') {
        DrawTextEx(GetFontDefault(), desc, (Vector2){ r.x + 24, posTitle.y + tsize.y + 6 }, fontSizeDesc, 1.0f, Fade(RAYWHITE, 0.85f));
    }

    bool kbActivate = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE);
    bool gpActivate = (IsGamepadAvailable(0) && (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)
                                               || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)));
    return (selected && (kbActivate || gpActivate)) || pressedMouse;
}

// Desenha imagem cobrindo a tela (cover)
static void DrawTextureCover(Texture2D tex) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    float scale = fmaxf((float)sw / (float)tex.width, (float)sh / (float)tex.height);
    float dw = tex.width  * scale;
    float dh = tex.height * scale;

    Rectangle src = { 0, 0, (float)tex.width, (float)tex.height };
    Rectangle dst = { (sw - dw) * 0.5f, (sh - dh) * 0.5f, dw, dh };
    Vector2  org = { 0, 0 };

    DrawTexturePro(tex, src, dst, org, 0.0f, WHITE);
}

void UpdateAndDrawMainMenu(Menu* menu, EstadoDoJogo* estado,
                           Texture2D bg, Texture2D titleTex, float dt)
{
    ClearBackground(BLACK);

    // 1) FUNDO: usa imagem estática (sem wobble/animação)
    DrawTextureCover(bg);

    // 2) SOBREPOSIÇÃO leve para ler o texto/botões melhor
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0,0,0,70});

    // 3) TÍTULO: usa imagem (logo) centralizada no topo
    {
        // largura desejada (pode ajustar): 40% da tela (clamp 280–720)
        float maxW = clampf(GetScreenWidth() * 0.40f, 280.0f, 720.0f);
        float scale = maxW / (float)titleTex.width;
        float w = titleTex.width  * scale;
        float h = titleTex.height * scale;

        Rectangle src = { 0, 0, (float)titleTex.width, (float)titleTex.height };
        Rectangle dst = { (GetScreenWidth() - w) * 0.5f, 60.0f, w, h };
        DrawTexturePro(titleTex, src, dst, (Vector2){0,0}, 0.0f, WHITE);
    }

    // ---------- Navegação (igual) ----------
    static float navCd = 0.0f;
    if (navCd > 0.0f) navCd -= dt;

    bool up   = IsKeyPressed(KEY_UP)   || (IsGamepadAvailable(0) && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP));
    bool down = IsKeyPressed(KEY_DOWN) || (IsGamepadAvailable(0) && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN));
    if (navCd <= 0.0f && (up || down)) {
        int dir = up ? -1 : +1;
        menu->selecionar = (menu->selecionar + dir + menu->cont) % menu->cont;
        navCd = 0.12f;
    }

    // ---------- Painel + Botões (igual) ----------
    float panelW = clampf(GetScreenWidth()*0.50f, 520.0f, 700.0f);
    float panelH = (float)menu->cont * 96.0f + (menu->cont - 1) * 18.0f + 40.0f;
    Rectangle panel = {
        .x = (GetScreenWidth() - panelW)/2.0f,
        .y = (GetScreenHeight() - panelH)/2.0f + 40.0f,
        .width = panelW,
        .height = panelH
    };

    DrawRectangleRounded(panel, 0.08f, 14, (Color){ 255,255,255, 18 });
    DrawRectangleRoundedLines(panel, 0.08f, 14, (Color){ 255,255,255, 48 });

    const char* descs[4] = {
        "Partida tradicional, foco em passe/chute.",
        "Poderes arcade caem no campo no decorrer do jogo.",
        "Opcional 3", "Opcional 4"
    };

    float btnH = 96.0f;
    float btnW = panelW - 40.0f;
    float y = panel.y + 20.0f;

    int hoverToSelect = -1;
    for (int i = 0; i < menu->cont; i++) {
        Rectangle r = { panel.x + 20.0f, y, btnW, btnH };
        bool selected = (i == menu->selecionar);

        if (CheckCollisionPointRec(GetMousePosition(), r)) hoverToSelect = i;

        const char* desc = (i < 4) ? descs[i] : NULL;
        bool activated = DrawMenuButton(r, menu->itens[i], desc, selected, dt);

        if (activated) {
            if (i == 0) *estado = ST_CLASSICO;
            else if (i == 1) *estado = ST_ARCADE;
            return;
        }

        y += btnH + 18.0f;
    }

    if (hoverToSelect >= 0) menu->selecionar = hoverToSelect;

    const char* tips = IsGamepadAvailable(0)
        ? "[\xE2\x96\xB2/\xE2\x96\xBC] Navegar  -  [A/Start] Selecionar  -  [B] Sair"
        : "[Enter/Espaço] Selecionar  -  [Esc] Sair";
    Vector2 tipsSize = MeasureTextEx(GetFontDefault(), tips, 18, 1.0f);
    DrawTextEx(GetFontDefault(), tips,
               (Vector2){ (GetScreenWidth()-tipsSize.x)/2.0f, panel.y + panel.height + 10.0f },
               18, 1.0f, Fade(RAYWHITE, 0.75f));
}
