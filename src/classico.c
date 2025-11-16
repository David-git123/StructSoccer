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

static const char* NomeFuncaoJogador(const Jogador *j) {
    switch (j->funcaoDoJogador) {
        case 1: return "Atacante";
        case 2: return "Defensor";
        case 3: return "Meia";
        case 4: return "Goleiro";
        default: return "Jogador";
    }
}

static void DesenharTopScoresFinal(Jogador *head1, Jogador *tail1,
                                   Jogador *head2, Jogador *tail2,
                                   Rectangle panel) {
    Jogador *lista[6];
    int count = 0;

    if (head1) {
        Jogador *aux = head1;
        do {
            if (count < 6) lista[count++] = aux;
            if (aux == tail1) break;
            aux = aux->prox;
        } while (aux && aux != head1);
    }

    if (head2) {
        Jogador *aux = head2;
        do {
            if (count < 6) lista[count++] = aux;
            if (aux == tail2) break;
            aux = aux->prox;
        } while (aux && aux != head2);
    }

    for (int i = 0; i < count - 1; i++) {
        int best = i;
        for (int j = i + 1; j < count; j++) {
            if (lista[j]->gols > lista[best]->gols) {
                best = j;
            }
        }
        if (best != i) {
            Jogador *tmp = lista[i];
            lista[i] = lista[best];
            lista[best] = tmp;
        }
    }

    int maxMostra = 3;
    if (maxMostra > count) maxMostra = count;

    int cardW = (int)(panel.width - 40);
    int cardH = 52;
    int startY = (int)(panel.y + 210);

    for (int i = 0; i < maxMostra; i++) {
        Jogador *j = lista[i];

        Rectangle card = {
            panel.x + 20,
            startY + i * (cardH + 10),
            (float)cardW,
            (float)cardH
        };

        Color bg = (i == 0) ? (Color){60, 50, 20, 240}
                            : (Color){35, 40, 55, 230};
        Color borda = (i == 0) ? GOLD : (Color){200, 200, 220, 180};

        DrawRectangleRounded(card, 0.25f, 8, bg);
        DrawRectangleRoundedLines(card, 0.25f, 8, borda);

        char pos[8];
        snprintf(pos, sizeof(pos), "%dº", i + 1);
        int fontPos = 22;
        int pw = MeasureText(pos, fontPos);
        int px = (int)(card.x + 10);
        int py = (int)(card.y + card.height/2 - fontPos/2);
        DrawText(pos, px, py, fontPos, (i == 0 ? GOLD : RAYWHITE));

        char linha[128];
        snprintf(linha, sizeof(linha),
                 "T%d - %s",
                 j->time,
                 NomeFuncaoJogador(j));
        int fontInfo = 18;
        int lx = px + pw + 12;
        int ly = (int)card.y + 8;
        DrawText(linha, lx, ly, fontInfo, RAYWHITE);

        char golsTxt[64];
        snprintf(golsTxt, sizeof(golsTxt),
                 "%d gol%s",
                 j->gols,
                 (j->gols == 1 ? "" : "s"));
        int gw = MeasureText(golsTxt, fontInfo);
        int gx = (int)(card.x + card.width - gw - 16);
        int gy = (int)(card.y + card.height/2 - fontInfo/2);
        DrawText(golsTxt, gx, gy, fontInfo, (i == 0 ? GOLD : RAYWHITE));
    }
}

void RunModoClassico(GameCtx *ctx){
    int contFramesBola = 0;
    int contadorFramesJogador = 0;

    PlayMusicStream(ctx->musicPartida);
    SetMusicVolume(ctx->musicPartida, 0.8f);

    bool fimDeJogo = false; 
    int  opcaoFim  = 0;    

    SetTargetFPS(60);
    
    ReiniciarCronometro(ctx->jogo, 60);
    
    while (!WindowShouldClose()) {

        UpdateMusicStream(ctx->musicPartida);
        
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
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                StopMusicStream(ctx->musicPartida);
                pthread_mutex_unlock(&lock);
                return; 
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

            if (ctx->jogo->tempoMostrarGol > 0.0f) {
                int sw = GetScreenWidth();
                int sh = GetScreenHeight();

                float scale = 2.5f;

                float tw = (float)ctx->goalMensagemTex.width;
                float th = (float)ctx->goalMensagemTex.height;

                float dw = tw * scale;
                float dh = th * scale;

                float x = (sw - dw) * 0.5f;
                float y = (sh - dh) * 0.5f;

                DrawRectangle(0, 0, sw, sh, (Color){0, 0, 0, 120});

                DrawTextureEx(ctx->goalMensagemTex, (Vector2){ x, y }, 0.0f, scale, WHITE);
            }

            if (fimDeJogo) {
                int sw = GetScreenWidth();
                int sh = GetScreenHeight();

                ordernarPorGols(ctx->head1, ctx->tail1);
                ordernarPorGols(ctx->head2, ctx->tail2);

                DrawRectangle(0, 0, sw, sh, (Color){0, 0, 0, 180});

                int panelW = 720;
                int panelH = 420;
                Rectangle panel = {
                    sw/2 - panelW/2,
                    sh/2 - panelH/2,
                    panelW,
                    panelH
                };

                DrawRectangleRounded(panel, 0.08f, 16, (Color){20, 24, 38, 245});
                DrawRectangleRoundedLines(panel, 0.08f, 16, (Color){220, 220, 255, 90});

                Rectangle header = {
                    panel.x,
                    panel.y,
                    panel.width,
                    70
                };
                DrawRectangleRounded(header, 0.08f, 16, (Color){30, 35, 60, 255});

                const char *titulo = "FIM DE PARTIDA";
                int fontTitulo = 32;
                int tw = MeasureText(titulo, fontTitulo);
                DrawText(titulo,
                        (int)(panel.x + (panel.width - tw)/2),
                        (int)(panel.y + 18),
                        fontTitulo,
                        YELLOW);

                const char *msg = "Pressione ENTER para voltar ao menu";
                int fontMsg = 18;
                int mw = MeasureText(msg, fontMsg);
                DrawText(msg,
                        (int)(panel.x + (panel.width - mw)/2),
                        (int)(panel.y + 54),
                        fontMsg,
                        (Color){220, 220, 240, 230});


                int boxW = 260;
                int boxH = 74;
                int boxY = (int)(panel.y + 90);
                int espaco = 30;

                Rectangle boxTime1 = {
                    panel.x + panel.width/2 - boxW - espaco/2,
                    boxY,
                    boxW,
                    boxH
                };
                Rectangle boxTime2 = {
                    panel.x + panel.width/2 + espaco/2,
                    boxY,
                    boxW,
                    boxH
                };

                DrawRectangleRounded(boxTime1, 0.20f, 10, (Color){10, 110, 230, 220});
                DrawRectangleRoundedLines(boxTime1, 0.20f, 10, (Color){200, 220, 255, 255});
                DrawText("TIME 1",
                        (int)(boxTime1.x + 14),
                        (int)(boxTime1.y + 10),
                        20,
                        RAYWHITE);

                char plac1[8];
                sprintf(plac1, "%d", ctx->jogo->placarTime1);
                int fontPlacar = 36;
                int pw1 = MeasureText(plac1, fontPlacar);
                DrawText(plac1,
                        (int)(boxTime1.x + boxTime1.width - pw1 - 18),
                        (int)(boxTime1.y + 22),
                        fontPlacar,
                        RAYWHITE);

                DrawRectangleRounded(boxTime2, 0.20f, 10, (Color){190, 40, 70, 220});
                DrawRectangleRoundedLines(boxTime2, 0.20f, 10, (Color){255, 210, 220, 255});
                DrawText("TIME 2",
                        (int)(boxTime2.x + 14),
                        (int)(boxTime2.y + 10),
                        20,
                        RAYWHITE);

                char plac2[8];
                sprintf(plac2, "%d", ctx->jogo->placarTime2);
                int pw2 = MeasureText(plac2, fontPlacar);
                DrawText(plac2,
                        (int)(boxTime2.x + boxTime2.width - pw2 - 18),
                        (int)(boxTime2.y + 22),
                        fontPlacar,
                        RAYWHITE);

                const char *tituloSec = "Destaques da partida";
                int fontSec = 22;
                int wSec = MeasureText(tituloSec, fontSec);
                int ySec = (int)(boxY + boxH + 8);
                DrawText(tituloSec,
                        (int)(panel.x + (panel.width - wSec)/2),
                        ySec,
                        fontSec,
                        RAYWHITE);

                DesenharTopScoresFinal(ctx->head1, ctx->tail1,
                                    ctx->head2, ctx->tail2,
                                    panel);
            }

        EndDrawing();

        contFramesBola++;
        contadorFramesJogador++;
    }
}