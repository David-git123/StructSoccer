#include "../include/menu.h"

#ifndef PI
#define PI 3.14159265358979323846f
#endif

static float clampf(float x, float a, float b) { return (x < a) ? a : (x > b) ? b : x; }

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
    bool gpActivate = (IsGamepadAvailable(0) && (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) // A
                                               || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT))); // Start
    return (selected && (kbActivate || gpActivate)) || pressedMouse;
}

void UpdateAndDrawMainMenu(Menu* menu, EstadoDoJogo* estado,
                           Texture2D campo, Texture2D bola, float dt) {
    ClearBackground((Color){20,20,28,255});

    float t = GetTime();
    float wobble = sinf(t*0.6f)*8.0f;
    DrawTextureEx(campo, (Vector2){ -40 + wobble, -30 - wobble }, 0.0f, 1.2f, Fade(WHITE, 0.25f));

    DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(),
                           (Color){10,10,16,180}, (Color){20,20,28,255});

    for (int x = -200; x < GetScreenWidth()+200; x += 60) {
        DrawLine(x, -20, x + 320, GetScreenHeight()+20, (Color){255,255,255,12});
    }

    const char* title = "STRUCT SOCCER";
    int titleSize = 64;
    Vector2 ts = MeasureTextEx(GetFontDefault(), title, titleSize, 1.0f);
    Vector2 tp = (Vector2){ (GetScreenWidth() - ts.x)/2.0f, 90 };
    DrawTextEx(GetFontDefault(), title, (Vector2){ tp.x+3, tp.y+3 }, titleSize, 1.0f, (Color){0,0,0,120});
    DrawTextEx(GetFontDefault(), title, tp, titleSize, 1.0f, RAYWHITE);

    const char* subtitle = "3v3 - PowerUps";
    int subSize = 22;
    Vector2 ss = MeasureTextEx(GetFontDefault(), subtitle, subSize, 1.0f);
    DrawTextEx(GetFontDefault(), subtitle,
               (Vector2){ (GetScreenWidth()-ss.x)/2.0f, tp.y + ts.y + 8 },
               subSize, 1.0f, Fade(RAYWHITE, 0.75f));

    static float navCd = 0.0f;
    if (navCd > 0.0f) navCd -= dt;

    bool up = IsKeyPressed(KEY_UP) || (IsGamepadAvailable(0) && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP));
    bool down = IsKeyPressed(KEY_DOWN) || (IsGamepadAvailable(0) && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN));
    if (navCd <= 0.0f && (up || down)) {
        int dir = up ? -1 : +1;
        menu->selecionar = (menu->selecionar + dir + menu->cont) % menu->cont;
        navCd = 0.12f;
    }

    float panelW = clampf(GetScreenWidth()*0.50f, 520.0f, 700.0f);
    float panelH = (float)menu->cont * 96.0f + (menu->cont - 1) * 18.0f + 40.0f;
    Rectangle panel = {
        .x = (GetScreenWidth() - panelW)/2.0f,
        .y = (GetScreenHeight() - panelH)/2.0f + 40.0f,
        .width = panelW,
        .height = panelH
    };

    DrawRectangleRounded(panel, 0.08f, 14, (Color){ 255,255,255, 10 });
    DrawRectangleRoundedLines(panel, 0.08f, 14, (Color){ 255,255,255, 40 });

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
               18, 1.0f, Fade(RAYWHITE, 0.65f));

    if (bola.id > 0) {
        float s = 1.8f + 0.05f * sinf(t*2.0f);
        Rectangle src = { 0,0, 10,10 };
        Vector2 pos = { panel.x + panel.width - 48.0f, panel.y - 32.0f };
        Rectangle dst = { pos.x, pos.y, 24*s, 24*s };
        DrawTexturePro(bola, src, dst, (Vector2){ dst.width/2, dst.height/2 }, t*120.0f, WHITE);
    }
}
