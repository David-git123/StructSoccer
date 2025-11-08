// src/classico.c
#include <pthread.h>
#include "raylib.h"
#include "../include/modes.h"
#include "../include/tempo.h"
#include <stdio.h>

// ---- declarações das funções ----
extern pthread_mutex_t lock;
void AtualizarPosJogador(Jogador * jogador, Jogador * head1 , Jogador * head2,Jogo * jogo);
void EstadoBola(Bola * bola, Jogador * jogador,Jogador * jogadorControladoTime1,Jogador * jogadorControladoTime2,Jogador * head1,Jogador *tail1, Jogador * head2, Jogador * tail2, Jogo * jogo);
void Passe(Bola * bola, Jogador * jogador, Jogo * jogo);
void Chutar(Bola* bola, Jogador* jogador, Jogo * jogo);
void Atrito(Bola * bola);
void MudarPosicaoBola(Bola * bola);
void AtualizarCamera(Camera2D * camera, Jogo  * jogo, Jogador * jogadorControladoTime1, Jogador* jogadorControladoTime2,Bola * bola);
void desenharTexturaBola(Texture2D bola, Bola * bola1, int contadorFrames, Jogador * jogadorControladoTime1, Jogador * jogadorControladoTime2);
void desenharTexturaJogador(Texture2D jogador, Bola * bola1, Jogador * jogador1, RectangleSprites ** headSprites, int contadorFramesJogador, Jogo * jogo);
void TratarColisoesParedeBola(Bola * bola, Rectangle rectangleParede, Jogo * jogo);
void TratarColisoesJogadorParede(Jogador * jogador, Rectangle rectangleParede ,Jogo * jogo);
void tratarGol(Jogo * jogo, Bola * bola, Jogador * head1, Jogador *head2, Jogador * tail1, Jogador * tail2);
void movimentoAutomaticoJogo(Jogo * jogo,Bola * bola, Jogador * jogadorControladoTime1, Jogador * jogadorControladoTime2, Jogador * head1,Jogador * tail1, Jogador * head2,Jogador * tail2);

// ---------------------------------------------

void RunModoClassico(GameCtx* ctx) {
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
            movimentoAutomaticoJogo(ctx->jogo,ctx->bola1,(*ctx->ctrl1),(*ctx->ctrl2),ctx->head1,ctx->tail1,ctx->head2,ctx->tail2);
            AtualizarPosJogador(*(ctx->ctrl1), ctx->head1, ctx->head2, ctx->jogo);
            AtualizarPosJogador(*(ctx->ctrl2), ctx->head1, ctx->head2, ctx->jogo);
            EstadoBola(ctx->bola1, *(ctx->ctrl1),*(ctx->ctrl1),*(ctx->ctrl2),ctx->head1,ctx->tail1, ctx->head2, ctx->tail2, ctx->jogo);
            EstadoBola(ctx->bola1, *(ctx->ctrl2),*(ctx->ctrl1),*(ctx->ctrl2), ctx->head1,ctx->tail1, ctx->head2,ctx->tail2, ctx->jogo);
            
            if (ctx->jogo->timeComBola == 1 || ctx->jogo->timeComBola == 0) {
                Passe(ctx->bola1, *(ctx->ctrl1), ctx->jogo);
                Chutar(ctx->bola1, *(ctx->ctrl1), ctx->jogo);
            } else if (ctx->jogo->timeComBola == 2 || ctx->jogo->timeComBola == 0) {
                Passe(ctx->bola1, *(ctx->ctrl2), ctx->jogo);
                Chutar(ctx->bola1, *(ctx->ctrl2), ctx->jogo);
            }
            pthread_mutex_unlock(&lock);

            Atrito(ctx->bola1);

            TratarColisoesParedeBola(ctx->bola1, ctx->jogo->rectangleParedeCima,     ctx->jogo);
            TratarColisoesParedeBola(ctx->bola1, ctx->jogo->rectangleParedeBaixo,   ctx->jogo);
            TratarColisoesParedeBola(ctx->bola1, ctx->jogo->rectangleParedeFundoDir1, ctx->jogo);
            TratarColisoesParedeBola(ctx->bola1, ctx->jogo->rectangleParedeFundoDir2, ctx->jogo);
            TratarColisoesParedeBola(ctx->bola1, ctx->jogo->rectangleParedeFundoEsq1, ctx->jogo);
            TratarColisoesParedeBola(ctx->bola1, ctx->jogo->rectangleParedeFundoEsq2, ctx->jogo);
            TratarColisoesParedeBola(ctx->bola1,ctx->jogo->linhaGolFim1,ctx->jogo);
            TratarColisoesParedeBola(ctx->bola1,ctx->jogo->linhaGolFim2,ctx->jogo);

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
        
        tratarGol(ctx->jogo,ctx->bola1,ctx->head1,ctx->tail1,ctx->head2,ctx->tail2);

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
                DrawTexturePro(ctx->barraTopo, (Rectangle){0,0,-ctx->barraTopo.width, ctx->barraTopo.height}, (Rectangle){795,90,ctx->barraTopo.width, ctx->barraTopo.height}, (Vector2){0,0}, 0.0f, WHITE);

                desenharTexturaJogador(ctx->jogadorTex, ctx->bola1, ctx->j1, ctx->headSprites, contadorFramesJogador,ctx->jogo);
                desenharTexturaJogador(ctx->jogadorTex, ctx->bola1, ctx->j2, ctx->headSprites, contadorFramesJogador,ctx->jogo);
                desenharTexturaJogador(ctx->jogadorTex, ctx->bola1, ctx->j3, ctx->headSprites, contadorFramesJogador,ctx->jogo);
                desenharTexturaJogador(ctx->jogadorTex, ctx->bola1, ctx->j4, ctx->headSprites, contadorFramesJogador,ctx->jogo);

                
                
                DrawRectangleLines(ctx->j1->posJogador.x, ctx->j1->posJogador.y, ctx->j1->rectJogador.width, ctx->j1->rectJogador.height, WHITE);
                DrawRectangleLines(ctx->jogo->rectangleParedeCima.x,     ctx->jogo->rectangleParedeCima.y,     ctx->jogo->rectangleParedeCima.width,     ctx->jogo->rectangleParedeCima.height,     RED);
                DrawRectangleLines(ctx->jogo->rectangleParedeBaixo.x,    ctx->jogo->rectangleParedeBaixo.y,    ctx->jogo->rectangleParedeBaixo.width,    ctx->jogo->rectangleParedeBaixo.height,    RED);
                DrawRectangleLines(ctx->jogo->rectangleParedeFundoEsq1.x,ctx->jogo->rectangleParedeFundoEsq1.y,ctx->jogo->rectangleParedeFundoEsq1.width,ctx->jogo->rectangleParedeFundoEsq1.height,RED);
                DrawRectangleLines(ctx->jogo->rectangleParedeFundoEsq2.x,ctx->jogo->rectangleParedeFundoEsq2.y,ctx->jogo->rectangleParedeFundoEsq2.width,ctx->jogo->rectangleParedeFundoEsq2.height,RED);
                DrawRectangleLines(ctx->jogo->rectangleParedeFundoDir1.x,ctx->jogo->rectangleParedeFundoDir1.y,ctx->jogo->rectangleParedeFundoDir1.width,ctx->jogo->rectangleParedeFundoDir1.height,RED);
                DrawRectangleLines(ctx->jogo->rectangleParedeFundoDir2.x,ctx->jogo->rectangleParedeFundoDir2.y,ctx->jogo->rectangleParedeFundoDir2.width,ctx->jogo->rectangleParedeFundoDir2.height,RED);
                DrawRectangleLines(ctx->jogo->linhaGol1.x,ctx->jogo->linhaGol1.y, ctx->jogo->linhaGol1.width, ctx->jogo->linhaGol1.height,RED);
                DrawRectangleLines(ctx->jogo->linhaGol2.x,ctx->jogo->linhaGol2.y, ctx->jogo->linhaGol2.width, ctx->jogo->linhaGol2.height,RED);
                DrawRectangleLines(ctx->jogo->linhaGolFim1.x,ctx->jogo->linhaGolFim1.y, ctx->jogo->linhaGolFim1.width, ctx->jogo->linhaGolFim1.height,RED);
                DrawRectangleLines(ctx->jogo->linhaGolFim2.x,ctx->jogo->linhaGolFim2.y, ctx->jogo->linhaGolFim2.width, ctx->jogo->linhaGolFim2.height,RED);
                // if(ctx->jogo->voltandoDoGol == 1){
                //     int width = GetScreenWidth();
                // }
                desenharTexturaBola(ctx->bolaTex, ctx->bola1, contFramesBola, *(ctx->ctrl1), *(ctx->ctrl2));
            EndMode2D();

            DrawText("MODO: CLASSICO", 20, 20, 22, WHITE);
            DesenharCronometroHUD(ctx->jogo, 20, 50);


            // menu para o fim do jogo (caio: vou ajeitar ainda pra ficar 100%)
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

        contFramesBola++;
        contadorFramesJogador++;
    }
}