#include <pthread.h>
#include "raylib.h"
#include "../include/modes.h"
#include "../include/tempo.h"


extern pthread_mutex_t lock;
void AtualizarPosJogador(Jogador * jogador, Jogador * head1 , Jogador * head2, Jogo * jogo);
void EstadoBola(Bola * bola, Jogador * jogador,Jogador * jogadorControladoTime1,Jogador * jogadorControladoTime2,Jogador * head1,Jogador * tail1, Jogador * head2,Jogador * tail2, Jogo * jogo);
void Passe(Bola * bola, Jogador * jogador, Jogo * jogo);
void Chutar(Bola* bola, Jogador* jogador, Jogo * jogo);
void Atrito(Bola * bola);
void MudarPosicaoBola(Bola * bola);
void AtualizarCamera(Camera2D * camera, Jogo  * jogo, Jogador * jogadorControladoTime1, Jogador* jogadorControladoTime2,Bola * bola);
void desenharTexturaBola(Texture2D bola, Bola * bola1, int contadorFrames, Jogador * jogadorControladoTime1, Jogador * jogadorControladoTime2);
void desenharTexturaJogador(Texture2D jogador, Bola * bola1, Jogador * jogador1, RectangleSprites ** headSprites, int contadorFramesJogador,Jogo * jogo);



void RunModoPowerUps(GameCtx* ctx) {
    int contFramesBola = 0;
    int contadorFramesJogador = 0;

    bool fimDeJogo = false;
    int  opcaoFim  = 0;

    SetTargetFPS(60);

    ReiniciarCronometro(ctx->jogo, 60);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        AtualizarCronometro(ctx->jogo, dt);

        if (!fimDeJogo && ctx->jogo->tempoRestante <= 0.0f) {
            ctx->jogo->tempoRestante = 0.0f;
            fimDeJogo = true;
        }

        if (contFramesBola == 60) contFramesBola = 0;
        if (contadorFramesJogador == 60) contadorFramesJogador = 0;

        pthread_mutex_lock(&lock);

        if (!fimDeJogo) {
            AtualizarPosJogador(*(ctx->ctrl1), ctx->j1, ctx->j3, ctx->jogo);
            AtualizarPosJogador(*(ctx->ctrl2), ctx->j1, ctx->j3, ctx->jogo);
            // EstadoBola(ctx->bola1, *(ctx->ctrl1), ctx->head1,ctx->tail1, ctx->head2,ctx->tail2, ctx->jogo);
            // EstadoBola(ctx->bola1, *(ctx->ctrl2), ctx->head1,ctx->tail1, ctx->head2,ctx->tail2, ctx->jogo);

            if (ctx->jogo->timeComBola == 1 || ctx->jogo->timeComBola == 0) {
                Passe(ctx->bola1, *(ctx->ctrl1), ctx->jogo);
                Chutar(ctx->bola1, *(ctx->ctrl1), ctx->jogo);
            } else if (ctx->jogo->timeComBola == 2 || ctx->jogo->timeComBola == 0) {
                Passe(ctx->bola1, *(ctx->ctrl2), ctx->jogo);
                Chutar(ctx->bola1, *(ctx->ctrl2), ctx->jogo);
            }

            Atrito(ctx->bola1);
            MudarPosicaoBola(ctx->bola1);

            AtualizarCamera(ctx->camera, ctx->jogo, *(ctx->ctrl1), *(ctx->ctrl2), ctx->bola1);
        } else {
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) {
                opcaoFim = 1 - opcaoFim;
            }

            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                if (opcaoFim == 0) {
                    ReiniciarCronometro(ctx->jogo, ctx->jogo->tempoInicial);
                    fimDeJogo = false;
                } else {
                    pthread_mutex_unlock(&lock);
                    return;
                }
            }
        }

        BeginDrawing();
            ClearBackground((Color){ 100, 200, 120, 255 });
            BeginMode2D(*(ctx->camera));
                DrawTexture(ctx->campo, 0, 0, WHITE);
                DrawTexture(ctx->paredeLadoCima, 50, 20, WHITE);
                DrawTexture(ctx->paredeLadoBaixo, 28, 355, WHITE);
                DrawTexture(ctx->paredeFundoCampo, 25, 220, WHITE);
                DrawTexturePro(ctx->paredeFundoCampo, ctx->srcParedeFundoCampoDir, ctx->destParedeFundoCampoDir2, (Vector2){0,0}, 0.0f, WHITE);
                DrawTexture(ctx->barra, 20, 100, WHITE);
                DrawTexture(ctx->barraTopo, 20, 100, WHITE);
                DrawTexturePro(ctx->barra, ctx->srcBarraEsquerda, ctx->destBarraEsquerda, (Vector2){0,0}, 0.0f, WHITE);
                DrawTexturePro(ctx->barraTopo, (Rectangle){0,0,-ctx->barraTopo.width, ctx->barraTopo.height}, (Rectangle){805,90,ctx->barraTopo.width, ctx->barraTopo.height}, (Vector2){0,0}, 0.0f, WHITE);

                desenharTexturaJogador(ctx->jogadorTex, ctx->bola1, ctx->j1, ctx->headSprites, contadorFramesJogador,ctx->jogo);
                desenharTexturaJogador(ctx->jogadorTex, ctx->bola1, ctx->j2, ctx->headSprites, contadorFramesJogador,ctx->jogo);
                desenharTexturaJogador(ctx->jogadorTex, ctx->bola1, ctx->j3, ctx->headSprites, contadorFramesJogador,ctx->jogo);
                desenharTexturaJogador(ctx->jogadorTex, ctx->bola1, ctx->j4, ctx->headSprites, contadorFramesJogador,ctx->jogo);

                desenharTexturaBola(ctx->bolaTex, ctx->bola1, contFramesBola, *(ctx->ctrl1), *(ctx->ctrl2));
            EndMode2D();

            DrawText("MODO: POWERUPS", 20, 20, 22, WHITE);
            DesenharCronometroHUD(ctx->jogo, 20, 50);

            if (fimDeJogo) {
                int sw = GetScreenWidth();
                int sh = GetScreenHeight();

                DrawRectangle(0, 0, sw, sh, (Color){0,0,0,150});

                int panelW = 400;
                int panelH = 200;
                Rectangle panel = {
                    sw/2 - panelW/2,
                    sh/2 - panelH/2,
                    panelW,
                    panelH
                };

                DrawRectangleRec(panel, (Color){30,30,40,240});
                DrawRectangleLines(panel.x, panel.y, panel.width, panel.height, RAYWHITE);

                const char *titulo = "Fim de partida!";
                int fontTitulo = 30;
                int tw = MeasureText(titulo, fontTitulo);
                DrawText(titulo, panel.x + (panel.width - tw)/2, panel.y + 20, fontTitulo, YELLOW);

                const char *opt0 = "Jogar novamente";
                const char *opt1 = "Voltar";

                int fontOpt = 22;
                Color c0 = (opcaoFim == 0) ? YELLOW : LIGHTGRAY;
                Color c1 = (opcaoFim == 1) ? YELLOW : LIGHTGRAY;

                int o0w = MeasureText(opt0, fontOpt);
                int o1w = MeasureText(opt1, fontOpt);

                DrawText(opt0, panel.x + (panel.width - o0w)/2, panel.y + 80,  fontOpt, c0);
                DrawText(opt1, panel.x + (panel.width - o1w)/2, panel.y + 120, fontOpt, c1);
            }

        EndDrawing();
        pthread_mutex_unlock(&lock);

        contFramesBola++;
        contadorFramesJogador++;
    }
}
