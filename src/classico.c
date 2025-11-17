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

            // ---------------- TELA FINAL ----------------
            if (fimDeJogo) {
                int sw = GetScreenWidth();
                int sh = GetScreenHeight();

                Jogador *lista[6];
                int count = 0;

                Jogador *aux = ctx->head1;
                if (aux) {
                    do {
                        lista[count++] = aux;
                        aux = aux->prox;
                    } while (aux != ctx->tail1->prox && count < 6);
                }

                aux = ctx->head2;
                if (aux) {
                    do {
                        lista[count++] = aux;
                        aux = aux->prox;
                    } while (aux != ctx->tail2->prox && count < 6);
                }

                for (int i = 0; i < count - 1; ++i) {
                    for (int j = i + 1; j < count; ++j) {
                        if (lista[j]->gols > lista[i]->gols) {
                            Jogador *tmp = lista[i];
                            lista[i] = lista[j];
                            lista[j] = tmp;
                        }
                    }
                }

                DrawRectangle(0, 0, sw, sh, (Color){0, 0, 0, 200});

                int panelW = 760;
                int panelH = 460;
                Rectangle panel = {
                    sw/2 - panelW/2,
                    sh/2 - panelH/2,
                    panelW,
                    panelH
                };

                DrawRectangleRounded(panel, 0.06f, 10, (Color){10, 12, 30, 245});
                DrawRectangleRoundedLines(panel, 0.06f, 10, (Color){230, 230, 255, 90});

                const char *titulo = "FIM DE PARTIDA";
                int fontTitulo = 40;
                int tw = MeasureText(titulo, fontTitulo);
                DrawText(titulo,
                        (int)(panel.x + (panel.width - tw)/2),
                        (int)(panel.y + 18),
                        fontTitulo,
                        YELLOW);

                const char *sub = "Pressione ENTER para voltar ao menu";
                int fontSub = 18;
                int ts = MeasureText(sub, fontSub);
                DrawText(sub,
                        (int)(panel.x + (panel.width - ts)/2),
                        (int)(panel.y + 65),
                        fontSub,
                        RAYWHITE);

                int boxY = (int)(panel.y + 100);
                int boxH = 64;
                int boxW = (int)(panel.width/2 - 40);

                Rectangle boxT1 = { panel.x + 24, boxY, boxW, boxH };
                Rectangle boxT2 = { panel.x + 24 + boxW + 24, boxY, boxW, boxH };

                DrawRectangleRounded(boxT1, 0.30f, 8, (Color){0, 115, 230, 220});
                DrawRectangleRounded(boxT2, 0.30f, 8, (Color){210, 50, 80, 220});

                const char *lbl1 = "TIME 1";
                const char *lbl2 = "TIME 2";
                int fontLbl = 22;
                int w1 = MeasureText(lbl1, fontLbl);
                int w2 = MeasureText(lbl2, fontLbl);

                DrawText(lbl1,
                        (int)(boxT1.x + (boxT1.width - w1)/2),
                        boxY + 6,
                        fontLbl,
                        WHITE);

                DrawText(lbl2,
                        (int)(boxT2.x + (boxT2.width - w2)/2),
                        boxY + 6,
                        fontLbl,
                        WHITE);

                char s1[8];
                char s2[8];
                sprintf(s1, "%d", ctx->jogo->placarTime1); 
                sprintf(s2, "%d", ctx->jogo->placarTime2); 

                int fontScore = 32;
                int ws1 = MeasureText(s1, fontScore);
                int ws2 = MeasureText(s2, fontScore);

                DrawText(s1,
                        (int)(boxT1.x + (boxT1.width - ws1)/2),
                        boxY + 30,
                        fontScore,
                        WHITE);

                DrawText(s2,
                        (int)(boxT2.x + (boxT2.width - ws2)/2),
                        boxY + 30,
                        fontScore,
                        WHITE);

                // ---------- TÍTULO DESTAQUES ----------
                const char *titDest = "Destaques da partida";
                int fontDest = 24;
                int wDest = MeasureText(titDest, fontDest);
                int destY = boxY + boxH + 30;
                DrawText(titDest,
                        (int)(panel.x + (panel.width - wDest)/2),
                        destY,
                        fontDest,
                        RAYWHITE);

                // ---------- LISTA TOP 3 ----------
                int maxRank = (count < 3) ? count : 3;
                int rowH = 60;
                int rowX = (int)(panel.x + 24);
                int rowW = (int)(panel.width - 48);
                int startY = destY + 30;

                for (int i = 0; i < maxRank; ++i) {
                    Jogador *p = lista[i];
                    int y = startY + i * (rowH + 6);

                    Color bg = (i == 0) ? (Color){230, 190, 40, 220} : (Color){25, 25, 45, 230};
                    Color textColor = (i == 0) ? BLACK : RAYWHITE;

                    DrawRectangleRounded(
                        (Rectangle){ rowX, y, rowW, rowH },
                        0.20f, 8,
                        bg
                    );

                    char posTxt[8];
                    sprintf(posTxt, "%do", i + 1);
                    int fontPos = 20;
                    DrawText(posTxt,
                            rowX + 12,
                            y + rowH/2 - fontPos/2,
                            fontPos,
                            textColor);

                    const char *timeStr   = (p->time == 1) ? "T1" : "T2";
                    const char *funcaoStr = "Jogador";
                    switch (p->funcaoDoJogador) {
                        case 1: funcaoStr = "Atacante"; break;
                        case 2: funcaoStr = "Defensor"; break;
                        case 3: funcaoStr = "Meia";      break;
                        case 4: funcaoStr = "Goleiro";   break;
                    }

                    char linha[64];
                    sprintf(linha, "%s - %s", timeStr, funcaoStr);
                    int fontLinha = 20;
                    DrawText(linha,
                            rowX + 60,
                            y + rowH/2 - fontLinha/2,
                            fontLinha,
                            textColor);

                    char golsTxt[32];
                    sprintf(golsTxt, "%d gol%s", p->gols, (p->gols == 1 ? "" : "s"));
                    int fontG = 20;
                    int wg = MeasureText(golsTxt, fontG);

                    DrawText(golsTxt,
                            rowX + rowW - wg - 18,
                            y + rowH/2 - fontG/2,
                            fontG,
                            textColor);
                }
            }


        EndDrawing();

        contFramesBola++;
        contadorFramesJogador++;
    }
}