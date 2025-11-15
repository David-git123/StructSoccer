    // src/powerups.c
    #include <pthread.h>
    #include "raylib.h"
    #include "../include/modes.h"
    #include "../include/tempo.h"
    #include <stdlib.h>  

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
    void tratarGol(Jogo *jogo, Bola *bola, Jogador *jogadorControladoTime1, Jogador *jogadorControladoTime2, Jogador *head1, Jogador *tail1, Jogador *head2, Jogador *tail2); void movimentoAutomaticoJogo(Jogo *jogo, Bola *bola, Jogador *jogadorControladoTime1, Jogador *jogadorControladoTime2, Jogador *headDaVez, Jogador *tailDaVez);
    void movimentarGoleiro(Jogador * goleiro, Jogo * jogo, Bola * bola);
    void mudarPosicaoJogadorVelocidade(Jogador *jogador);


    PowerUp *CriarPowerUp(TipoPowerUp tipo, float x, float y)
    {
        PowerUp *p = (PowerUp*)malloc(sizeof(PowerUp));
        if (!p) return NULL;

        p->tipo  = tipo;
        p->ativo = 1;
        p->caixa = (Rectangle){ x, y, 16.0f, 16.0f }; 
        p->prox  = NULL;
        return p;
    }

    void AdicionarPowerUpNaLista(Jogo *jogo, PowerUp *p) {
        if (!p) return;
        p->prox = jogo->listaPowerUps;
        jogo->listaPowerUps = p;
    }


    void SpawnPowerUpAleatorio(GameCtx *ctx) {
        int tipoInt = GetRandomValue(0, PW_MAX_TIPOS - 1);
        TipoPowerUp tipo = (TipoPowerUp)tipoInt;

    // nascer sempre no meio do campo,
        float x = ctx->bola1->posInicialBola.x - 8.0f; 
        float y = ctx->bola1->posInicialBola.y - 8.0f;

        PowerUp *p = CriarPowerUp(tipo, x, y);
        AdicionarPowerUpNaLista(ctx->jogo, p);
}

    

    void DesenharPowerUps(const Jogo *jogo) {
        PowerUp *p = jogo->listaPowerUps;
        while (p) {
            if (p->ativo) {
                Color cor;
                if (p->tipo == PW_VELOCIDADE) {
                    cor = BLUE;
                } else if(p->tipo == PW_CONGELAR){
                    cor = RED;
                } else if (p->tipo == PW_SUPERCHUTE) {
                    cor =  YELLOW;
                }
                DrawRectangleRec(p->caixa, cor);
            }
            p = p->prox;
        }
    }

    // Quando um jogador encosta num power-up, remove da lista
   void AtualizarColetaPowerUps(GameCtx *ctx) {
    Jogo *jogo = ctx->jogo;
    PowerUp **pp = &jogo->listaPowerUps;

    Jogador *ctrl1 = *(ctx->ctrl1);
    Jogador *ctrl2 = *(ctx->ctrl2);

    while (*pp) {
        PowerUp *p = *pp;

        int coletou = 0;
        Jogador *coletor = NULL;

        if (CheckCollisionRecs(p->caixa, ctrl1->rectJogador)) {
            coletou = 1;
            coletor = ctrl1;
        } else if (CheckCollisionRecs(p->caixa, ctrl2->rectJogador)) {
            coletou = 1;
            coletor = ctrl2;
        }

        if (coletou) {

            // remove do jogo
            p->ativo = 0;

           
            if (p->tipo == PW_VELOCIDADE) {
                jogo->buffVelocidadeTimer = 5.0f;   
                coletor->velocidadeBonus = 2.0f;    
            }

            
            else if (p->tipo == PW_CONGELAR) {
                if (coletor->time == 1)
                    jogo->congeladoTimerTime2 = 3.0f;
                
                   
                else
                    jogo->congeladoTimerTime1 = 3.0f;
                  
            }

            // remove da lista
            *pp = p->prox;
            free(p);
            continue;
        }

        pp = &((*pp)->prox);
    }
}


    void RunModoPowerUps(GameCtx* ctx) {
        int contFramesBola = 0;
        int contadorFramesJogador = 0;

        bool fimDeJogo = false; 
        int  opcaoFim  = 0;    

        SetTargetFPS(60);
        ReiniciarCronometro(ctx->jogo, 60);
        float tempoAteProximoSpawn = 2.0f;    


        while (!WindowShouldClose()) {
            float dt = GetFrameTime();
            AtualizarCronometro(ctx->jogo, dt);

        if (ctx->jogo->superChuteTimerTime1 > 0.0f) {
            ctx->jogo->superChuteTimerTime1 -= dt;
            if (ctx->jogo->superChuteTimerTime1 < 0.0f) ctx->jogo->superChuteTimerTime1 = 0.0f;
        }
        if (ctx->jogo->superChuteTimerTime2 > 0.0f) {
            ctx->jogo->superChuteTimerTime2 -= dt;
            if (ctx->jogo->superChuteTimerTime2 < 0.0f) ctx->jogo->superChuteTimerTime2 = 0.0f;
        }

            // Atualiza timers de power-ups
            if (ctx->jogo->buffVelocidadeTimer > 0) {
                ctx->jogo->buffVelocidadeTimer -= dt;
                if (ctx->jogo->buffVelocidadeTimer <= 0) {
                    ctx->jogo->buffVelocidadeTimer = 0;
                    ctx->j1->velocidadeBonus = 0;
                    ctx->j2->velocidadeBonus = 0;
                    ctx->j3->velocidadeBonus = 0;
                    ctx->j4->velocidadeBonus = 0;
                    ctx->j5->velocidadeBonus = 0;
                    ctx->j6->velocidadeBonus = 0;
                }
            }

            if (ctx->jogo->congeladoTimerTime1 > 0)
                ctx->jogo->congeladoTimerTime1 -= dt;

            if (ctx->jogo->congeladoTimerTime2 > 0)
                ctx->jogo->congeladoTimerTime2 -= dt;


            if (!fimDeJogo && ctx->jogo->tempoRestante <= 0.0f) {
                ctx->jogo->tempoRestante = 0.0f;
                fimDeJogo = true;
            }
            if (!fimDeJogo) {
                tempoAteProximoSpawn -= dt;
                if (tempoAteProximoSpawn <= 0.0f) {
                    SpawnPowerUpAleatorio(ctx);
                    tempoAteProximoSpawn = 5.0f; 
                }
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
            if (!fimDeJogo) {
                AtualizarColetaPowerUps(ctx);
            }


            BeginDrawing();
                ClearBackground((Color){100, 200, 120, 255});

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

                desenharTexturaJogador(ctx->jogadorTex, ctx->bola1, ctx->j1, ctx->headSprites, contadorFramesJogador, ctx->jogo);
                desenharTexturaJogador(ctx->jogadorTex, ctx->bola1, ctx->j2, ctx->headSprites, contadorFramesJogador, ctx->jogo);
                desenharTexturaJogador(ctx->texturaTime2, ctx->bola1, ctx->j3, ctx->headSprites, contadorFramesJogador, ctx->jogo);
                desenharTexturaJogador(ctx->texturaTime2, ctx->bola1, ctx->j4, ctx->headSprites, contadorFramesJogador, ctx->jogo);
                desenharTexturaJogador(ctx->jogadorTex, ctx->bola1, ctx->j5, ctx->headSprites, contadorFramesJogador, ctx->jogo);
                desenharTexturaJogador(ctx->texturaTime2, ctx->bola1, ctx->j6, ctx->headSprites, contadorFramesJogador, ctx->jogo);

                DrawTexture(ctx->txtJogadorControlado1, (*ctx->ctrl1)->posJogador.x, (*ctx->ctrl1)->posJogador.y - 8, WHITE);
                DrawTexture(ctx->txtJogadorControlado2, (*ctx->ctrl2)->posJogador.x + 7, (*ctx->ctrl2)->posJogador.y - 8, WHITE);

                desenharTexturaBola(ctx->bolaTex, ctx->bola1, contFramesBola, *(ctx->ctrl1), *(ctx->ctrl2));

                //  power-ups são desenhados NO MESMO MUNDO (com câmera) 
                DesenharPowerUps(ctx->jogo);
            EndMode2D();

            DesenharPlacarHUD(ctx->jogo);

            DrawText("MODO: POWERUPS", 20, 20, 22, WHITE);
            DesenharCronometroHUD(ctx->jogo, 20, 50);

            if (fimDeJogo) {
                int sw = GetScreenWidth();
                int sh = GetScreenHeight();

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
