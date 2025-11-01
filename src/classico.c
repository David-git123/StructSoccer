// src/classico.c
#include <pthread.h>
#include "raylib.h"
#include "../include/modes.h"

// ---- declarações das funções ----
extern pthread_mutex_t lock;
void AtualizarPosJogador(Jogador * jogador, Jogador * head1 , Jogador * head2);
void EstadoBola(Bola * bola, Jogador * jogador,Jogador * head1, Jogador * head2, Jogo * jogo);
void Passe(Bola * bola, Jogador * jogador, Jogo * jogo);
void Chutar(Bola* bola, Jogador* jogador, Jogo * jogo);
void Atrito(Bola * bola);
void MudarPosicaoBola(Bola * bola);
void AtualizarCamera(Camera2D * camera, Jogo  * jogo, Jogador * jogadorControladoTime1, Jogador* jogadorControladoTime2,Bola * bola);
void desenharTexturaBola(Texture2D bola, Bola * bola1, int contadorFrames, Jogador * jogadorControladoTime1, Jogador * jogadorControladoTime2);
void desenharTexturaJogador(Texture2D jogador, Bola * bola1, Jogador * jogador1, RectangleSprites ** headSprites, int contadorFramesJogador);

// ---------------------------------------------

void RunModoClassico(GameCtx* ctx) {
    int contFramesBola = 0;
    int contadorFramesJogador = 0;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (contFramesBola == 60) contFramesBola = 0;
        if (contadorFramesJogador == 60) contadorFramesJogador = 0;

        pthread_mutex_lock(&lock);
        AtualizarPosJogador(*(ctx->ctrl1), ctx->j1, ctx->j3);
        AtualizarPosJogador(*(ctx->ctrl2), ctx->j1, ctx->j3);
        EstadoBola(ctx->bola1, *(ctx->ctrl1), ctx->j1, ctx->j3, ctx->jogo);
        EstadoBola(ctx->bola1, *(ctx->ctrl2), ctx->j1, ctx->j3, ctx->jogo);
        if (ctx->jogo->timeComBola == 1 || ctx->jogo->timeComBola == 0) {
            Passe(ctx->bola1, *(ctx->ctrl1), ctx->jogo);
            Chutar(ctx->bola1, *(ctx->ctrl1), ctx->jogo);
        } else if (ctx->jogo->timeComBola == 2 || ctx->jogo->timeComBola == 0) {
            Passe(ctx->bola1, *(ctx->ctrl2), ctx->jogo);
            Chutar(ctx->bola1, *(ctx->ctrl2), ctx->jogo);
        }
        pthread_mutex_unlock(&lock);

        Atrito(ctx->bola1);
        MudarPosicaoBola(ctx->bola1);

        AtualizarCamera(ctx->camera, ctx->jogo, *(ctx->ctrl1), *(ctx->ctrl2), ctx->bola1);

        BeginDrawing();
            ClearBackground(ctx->corVerdeGrama);
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

                desenharTexturaJogador(ctx->jogadorTex, ctx->bola1, ctx->j1, ctx->headSprites, contadorFramesJogador);
                desenharTexturaJogador(ctx->jogadorTex, ctx->bola1, ctx->j2, ctx->headSprites, contadorFramesJogador);
                desenharTexturaJogador(ctx->jogadorTex, ctx->bola1, ctx->j3, ctx->headSprites, contadorFramesJogador);
                desenharTexturaJogador(ctx->jogadorTex, ctx->bola1, ctx->j4, ctx->headSprites, contadorFramesJogador);

                // exemplo de debug
                DrawRectangleLines(ctx->j1->posJogador.x, ctx->j1->posJogador.y, ctx->j1->rectJogador.width, ctx->j1->rectJogador.height, WHITE);
                DrawRectangleLines(ctx->jogo->rectangleParedeCima.x, ctx->jogo->rectangleParedeCima.y, ctx->jogo->rectangleParedeCima.width, ctx->jogo->rectangleParedeCima.height, RED);

                desenharTexturaBola(ctx->bolaTex, ctx->bola1, contFramesBola, *(ctx->ctrl1), *(ctx->ctrl2));
            EndMode2D();

            DrawText("MODO: CLASSICO", 20, 20, 22, WHITE);
        EndDrawing();

        contFramesBola++;
        contadorFramesJogador++;
    }
}
