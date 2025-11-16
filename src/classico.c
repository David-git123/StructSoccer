// src/classico.c
#include <pthread.h>
#include "raylib.h"
#include "../include/modes.h"
#include "../include/tempo.h"
#include <stdio.h>

// ---- declarações das funções ----
extern pthread_mutex_t lock;
void AtualizarPosJogador(Jogador *jogador, Jogador *head1, Jogador *tail1, Jogador *head2, Jogador *tail2, Jogo *jogo);
void EstadoBola(Bola * bola, Jogador * jogador,Jogador ** jogadorControladoTime1,Jogador ** jogadorControladoTime2,Jogador * goleiro1, Jogador * goleiro2,Jogador * head1,Jogador *tail1, Jogador * head2, Jogador * tail2, Jogo * jogo);
void Passe(Bola * bola, Jogador * jogador, Jogo * jogo, Jogador ** jogadorControladoTime1,Jogador ** jogadorControladoTime2);
void Chutar(Bola* bola, Jogador* jogador, Jogo * jogo);
void Atrito(Bola * bola);
void MudarPosicaoBola(Bola * bola);
void AtualizarCamera(Camera2D * camera, Jogo  * jogo, Jogador * jogadorControladoTime1, Jogador* jogadorControladoTime2,Bola * bola);
void desenharTexturaBola(Texture2D bola, Bola * bola1, int contadorFrames, Jogador * jogadorControladoTime1, Jogador * jogadorControladoTime2);
void desenharTexturaJogador(Texture2D jogador, Bola * bola1, Jogador * jogador1, RectangleSprites ** headSprites, int contadorFramesJogador, Jogo * jogo);
void TratarColisoesParedeBola(Bola * bola, Rectangle rectangleParede, Jogo * jogo);
void TratarColisoesJogadorParede(Jogador * jogador, Rectangle rectangleParede ,Jogo * jogo);
void tratarGol(Jogo *jogo, Bola *bola, Jogador *jogadorControladoTime1, Jogador *jogadorControladoTime2, Jogador *head1, Jogador *tail1, Jogador *head2, Jogador *tail2); 
void movimentoAutomaticoJogo(Jogo *jogo, Bola *bola, Jogador *jogadorControladoTime1, Jogador *jogadorControladoTime2, Jogador *headDaVez, Jogador *tailDaVez);
void movimentarGoleiro(Jogador *goleiro, Jogo *jogo, Bola *bola);
void mudarPosicaoJogadorVelocidade(Jogador *jogador);
void ordernarPorGols(Jogador *head1, Jogador *tail1);
    // ---------------------------------------------

    void RunModoClassico(GameCtx *ctx)
{
    int contFramesBola = 0;
    int contadorFramesJogador = 0;

    bool fimDeJogo = false; 
    int  opcaoFim  = 0;    

    SetTargetFPS(60);
    
    ReiniciarCronometro(ctx->jogo, 60);
    
    while (!WindowShouldClose()) {
        
        float dt = GetFrameTime();
        AtualizarCronometro(ctx->jogo, dt);

        if (ctx->jogo->tempoMostrarGol > 0.0f) {
            ctx->jogo->tempoMostrarGol -= dt;
            if (ctx->jogo->tempoMostrarGol < 0.0f) {
                ctx->jogo->tempoMostrarGol = 0.0f;
            }
        }
        
        if (!fimDeJogo && ctx->jogo->tempoRestante <= 0.0f) {
            ctx->jogo->tempoRestante = 0.0f;
            fimDeJogo = true;
        }
        
        if (contFramesBola == 40) contFramesBola = 0;
        if (contadorFramesJogador == 60) contadorFramesJogador = 0;
      
        pthread_mutex_lock(&lock);

        if (!fimDeJogo) {
            movimentarGoleiro(ctx->goleiro1,ctx->jogo,ctx->bola1);
            movimentarGoleiro(ctx->goleiro2,ctx->jogo,ctx->bola1);
            movimentoAutomaticoJogo(ctx->jogo,ctx->bola1,(*ctx->ctrl1),(*ctx->ctrl2),ctx->head1,ctx->tail1);
            movimentoAutomaticoJogo(ctx->jogo, ctx->bola1, (*ctx->ctrl1), (*ctx->ctrl2), ctx->head2, ctx->tail2);

            EstadoBola(ctx->bola1, ctx->j1,ctx->ctrl1,ctx->ctrl2,ctx->goleiro1,ctx->goleiro2,ctx->head1,ctx->tail1, ctx->head2, ctx->tail2, ctx->jogo);
            EstadoBola(ctx->bola1, ctx->j2,ctx->ctrl1,ctx->ctrl2,ctx->goleiro1,ctx->goleiro2, ctx->head1,ctx->tail1, ctx->head2,ctx->tail2, ctx->jogo);
            EstadoBola(ctx->bola1, ctx->j3, ctx->ctrl1, ctx->ctrl2, ctx->goleiro1, ctx->goleiro2, ctx->head1, ctx->tail1, ctx->head2, ctx->tail2, ctx->jogo);
            EstadoBola(ctx->bola1, ctx->j4, ctx->ctrl1, ctx->ctrl2, ctx->goleiro1, ctx->goleiro2, ctx->head1, ctx->tail1, ctx->head2, ctx->tail2, ctx->jogo);
            EstadoBola(ctx->bola1, ctx->j5, ctx->ctrl1, ctx->ctrl2, ctx->goleiro1, ctx->goleiro2, ctx->head1, ctx->tail1, ctx->head2, ctx->tail2, ctx->jogo);
            EstadoBola(ctx->bola1, ctx->j6, ctx->ctrl1, ctx->ctrl2, ctx->goleiro1, ctx->goleiro2, ctx->head1, ctx->tail1, ctx->head2, ctx->tail2, ctx->jogo);
            
            AtualizarPosJogador(*(ctx->ctrl1), ctx->head1,ctx->tail1, ctx->head2,ctx->tail2, ctx->jogo);
            AtualizarPosJogador(*(ctx->ctrl2), ctx->head1, ctx->tail1,ctx->head2, ctx->tail2,ctx->jogo);

            if (ctx->jogo->timeComBola == 1 || ctx->jogo->timeComBola == 0) {
                Passe(ctx->bola1, *(ctx->ctrl1), ctx->jogo,ctx->ctrl1,ctx->ctrl2);
                Chutar(ctx->bola1, *(ctx->ctrl1), ctx->jogo);
            } else if (ctx->jogo->timeComBola == 2 || ctx->jogo->timeComBola == 0) {
                Passe(ctx->bola1, *(ctx->ctrl2), ctx->jogo,ctx->ctrl1,ctx->ctrl2);
                Chutar(ctx->bola1, *(ctx->ctrl2), ctx->jogo);
            }
        } else {
            // 👉 NESSA TELA FINAL: só volta pro início
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                pthread_mutex_unlock(&lock);
                return;  // volta para quem chamou (main)
            }
        }

        pthread_mutex_unlock(&lock);

        if (!fimDeJogo) {
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

            tratarGol(ctx->jogo,ctx->bola1,*(ctx->ctrl1),*(ctx->ctrl2),ctx->head1,ctx->tail1,ctx->head2,ctx->tail2);
        }

        
        tratarGol(ctx->jogo,ctx->bola1,*(ctx->ctrl1),*(ctx->ctrl2),ctx->head1,ctx->tail1,ctx->head2,ctx->tail2);

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
                desenharTexturaJogador(ctx->texturaTime2, ctx->bola1, ctx->j3, ctx->headSprites, contadorFramesJogador,ctx->jogo);
                desenharTexturaJogador(ctx->texturaTime2, ctx->bola1, ctx->j4, ctx->headSprites, contadorFramesJogador,ctx->jogo);
                desenharTexturaJogador(ctx->jogadorTex,ctx->bola1,ctx->j5,ctx->headSprites,contadorFramesJogador,ctx->jogo);
                desenharTexturaJogador(ctx->texturaTime2,ctx->bola1,ctx->j6,ctx->headSprites,contadorFramesJogador,ctx->jogo);
                desenharTexturaJogador(ctx->txtGoleiro,ctx->bola1,ctx->goleiro1,ctx->headSprites,contadorFramesJogador,ctx->jogo);
                desenharTexturaJogador(ctx->txtGoleiro,ctx->bola1,ctx->goleiro2,ctx->headSprites,contadorFramesJogador,ctx->jogo);

                DrawTexture(ctx->txtJogadorControlado1,(*ctx->ctrl1)->posJogador.x,(*ctx->ctrl1)->posJogador.y-8,WHITE);
                DrawTexture(ctx->txtJogadorControlado2,(*ctx->ctrl2)->posJogador.x+7,(*ctx->ctrl2)->posJogador.y-8,WHITE);


                //DEBUG:
                // DrawRectangleLines(ctx->j1->posJogador.x, ctx->j1->posJogador.y, ctx->j1->rectJogador.width, ctx->j1->rectJogador.height, WHITE);
                // DrawRectangleLines(ctx->jogo->rectangleParedeCima.x,     ctx->jogo->rectangleParedeCima.y,     ctx->jogo->rectangleParedeCima.width,     ctx->jogo->rectangleParedeCima.height,     RED);
                // DrawRectangleLines(ctx->jogo->rectangleParedeBaixo.x,    ctx->jogo->rectangleParedeBaixo.y,    ctx->jogo->rectangleParedeBaixo.width,    ctx->jogo->rectangleParedeBaixo.height,    RED);
                // DrawRectangleLines(ctx->jogo->rectangleParedeFundoEsq1.x,ctx->jogo->rectangleParedeFundoEsq1.y,ctx->jogo->rectangleParedeFundoEsq1.width,ctx->jogo->rectangleParedeFundoEsq1.height,RED);
                // DrawRectangleLines(ctx->jogo->rectangleParedeFundoEsq2.x,ctx->jogo->rectangleParedeFundoEsq2.y,ctx->jogo->rectangleParedeFundoEsq2.width,ctx->jogo->rectangleParedeFundoEsq2.height,RED);
                // DrawRectangleLines(ctx->jogo->rectangleParedeFundoDir1.x,ctx->jogo->rectangleParedeFundoDir1.y,ctx->jogo->rectangleParedeFundoDir1.width,ctx->jogo->rectangleParedeFundoDir1.height,RED);
                // DrawRectangleLines(ctx->jogo->rectangleParedeFundoDir2.x,ctx->jogo->rectangleParedeFundoDir2.y,ctx->jogo->rectangleParedeFundoDir2.width,ctx->jogo->rectangleParedeFundoDir2.height,RED);
                // DrawRectangleLines(ctx->jogo->linhaGol1.x,ctx->jogo->linhaGol1.y, ctx->jogo->linhaGol1.width, ctx->jogo->linhaGol1.height,RED);
                // DrawRectangleLines(ctx->jogo->linhaGol2.x,ctx->jogo->linhaGol2.y, ctx->jogo->linhaGol2.width, ctx->jogo->linhaGol2.height,RED);
                // DrawRectangleLines(ctx->jogo->linhaGolFim1.x,ctx->jogo->linhaGolFim1.y, ctx->jogo->linhaGolFim1.width, ctx->jogo->linhaGolFim1.height,RED);
                // DrawRectangleLines(ctx->jogo->linhaGolFim2.x,ctx->jogo->linhaGolFim2.y, ctx->jogo->linhaGolFim2.width, ctx->jogo->linhaGolFim2.height,RED);

                desenharTexturaBola(ctx->bolaTex, ctx->bola1, contFramesBola, *(ctx->ctrl1), *(ctx->ctrl2));
            EndMode2D();

            DesenharPlacarHUD(ctx->jogo);

            DrawText("MODO: CLASSICO", 20, 20, 22, WHITE);
            DesenharCronometroHUD(ctx->jogo, 20, 50);

            // --- NOVO: sprite de GOL no centro da tela ---
            if (ctx->jogo->voltandoDoGol == 1) {
                int sw = GetScreenWidth();
                int sh = GetScreenHeight();

                int tw = ctx->goalMensagemTex.width;
                int th = ctx->goalMensagemTex.height;

                int x = sw/2 - tw/2;
                int y = sh/2 - th/2;

                // se quiser escurecer o fundo um pouco:
                DrawRectangle(0, 0, sw, sh, (Color){0, 0, 0, 120});

                DrawTexture(ctx->goalMensagemTex, x, y, WHITE);
            }

            // menu para o fim do jogo (caio: vou ajeitar ainda pra ficar 100%)
            if (fimDeJogo) {
                int sw = GetScreenWidth();
                int sh = GetScreenHeight();
                ordernarPorGols(ctx->head1,ctx->tail1);
                ordernarPorGols(ctx->head2,ctx->tail2);
                
                DrawRectangle(0, 0, sw, sh, (Color){0,0,0,150});

                int panelW = 400;
                int panelH = 160;
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
                DrawText(titulo, panel.x + (panel.width - tw)/2, panel.y + 25, fontTitulo, YELLOW);

                const char *msg = "Pressione ENTER para voltar.";
                int fontMsg = 20;
                int mw = MeasureText(msg, fontMsg);
                DrawText(msg, panel.x + (panel.width - mw)/2, panel.y + 90, fontMsg, RAYWHITE);
            }

        EndDrawing();

        contFramesBola++;
        contadorFramesJogador++;
    }
}