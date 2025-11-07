#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "../include/tempo.h"
#include "../include/menu.h"
#include "../include/structsoccer.h"
#include "../include/modes.h"                

//Controles: JOGADOR 1: UP, DOWN , LEFT RIGTH. SHIT DIREITO (TROCA). Ç PASSE . CHUTA;
//Controles: JOGADOR 2: W, S , A ,D. SHIT ESQUERDO (TROCA). C PASSE. LEFT CONTROL CHUTE;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void main() {

    Jogo * jogo = (Jogo *)malloc(sizeof(Jogo));
    jogo->timeComBola =0;
    jogo->placarTime1 =0;
    jogo->placarTime2 =0;
    jogo->rectangleParedeCima = (Rectangle){50,20,750,10};
    jogo->rectangleParedeBaixo = (Rectangle){28,355,800,10};
    jogo->rectangleParedeFundoEsq1 = (Rectangle){55,25,10,110};
    jogo->rectangleParedeFundoEsq2 = (Rectangle){38,215,10,144};
    jogo->rectangleParedeFundoDir1 = (Rectangle){785,30,10,120};
    jogo->rectangleParedeFundoDir2 = (Rectangle){810,220,10,140};

    //rectangleGol1 e rectangleGol2 podem ser excluidos (David)
    jogo->rectangleGol1.x= 1720.0f;
    jogo->rectangleGol1.y = 540.0f;
    jogo->rectangleGol1.height = 300.0f;
    jogo->rectangleGol1.width = 200.0f;


    jogo->rectangleGol2.x= 0;
    jogo->rectangleGol2.y = 540.0f;
    jogo->rectangleGol2.height = 300.0f;
    jogo->rectangleGol2.width = 200.0f;

    jogo->linhaGol1 = (Rectangle){60,153,2,70};
    jogo->linhaGol2 = (Rectangle){800,145,2,70};
    jogo->linhaGolFim1 = (Rectangle){30,153,2,70};
    jogo->linhaGolFim2 = (Rectangle){830,145,2,70};

    jogo->voltandoDoGol = 0;
    Jogador* head1Jogador = NULL;
    Jogador* tail1Jogador = NULL;

    Jogador* head2Jogador = NULL;
    Jogador* tail2Jogador = NULL;

    Jogador* jogador1 = (Jogador*)malloc(sizeof(Jogador));
    Jogador* jogador2 = (Jogador*)malloc(sizeof(Jogador));
    Jogador* jogador3 = (Jogador*)malloc(sizeof(Jogador));
    Jogador* jogador4 = (Jogador*)malloc(sizeof(Jogador));
    
    
    head1Jogador = jogador1;
    jogador1->prox = jogador2;  
    jogador2->prox = head1Jogador;
    tail1Jogador = jogador2;
    Rectangle src1 = {0,0,24,32};
    
    head2Jogador = jogador3;
    jogador3->prox = jogador4;
    jogador4->prox = head2Jogador;
    tail2Jogador = jogador4;
    
    //Sprites de jogador correndo
    RectangleSprites * headSpritesJogador;
    Rectangle srcJ2 = {0,32,24,32};
    Rectangle srcJ3 = {32,32,24,32};
    Rectangle srcJ4 = {64,32,24,32};
    Rectangle srcJ5 = {96,32,24,32};
    Rectangle srcJ6 = {128,32,24,32};
    Rectangle srcJ7 = {160,32,24,32};

    RectangleSprites * rsj2 = (RectangleSprites *)malloc(sizeof(RectangleSprites));
    RectangleSprites * rsj3 = (RectangleSprites *)malloc(sizeof(RectangleSprites));
    RectangleSprites * rsj4 = (RectangleSprites *)malloc(sizeof(RectangleSprites));
    RectangleSprites * rsj5 = (RectangleSprites *)malloc(sizeof(RectangleSprites));
    RectangleSprites * rsj6 = (RectangleSprites *)malloc(sizeof(RectangleSprites));
    RectangleSprites * rsj7 = (RectangleSprites *)malloc(sizeof(RectangleSprites));
    
 
    rsj2->Rectangle = srcJ2;
    rsj2->prox = rsj3;
    rsj3->Rectangle = srcJ3;
    rsj3->prox = rsj4;
    rsj4->Rectangle = srcJ4;
    rsj4->prox = rsj5;
    rsj5->Rectangle = srcJ5;
    rsj5->prox = rsj6;
    rsj6->Rectangle = srcJ6;
    rsj6->prox = rsj7;
    rsj7->Rectangle = srcJ7;
    rsj7->prox = rsj2;
    headSpritesJogador = rsj2;

    if (jogador1) {
        jogador1->temDominio = 0;
        jogador1->forcaChute = 10.0f;
        jogador1->posJogador = (Vector2){ 500.0f,180.0f};
        jogador1->posJogadorInicial = (Vector2){ 500.0f,180.0f};
        jogador1->largura = 24;
        jogador1->altura = 30;
        jogador1->velocidadeJogador = (Vector2){0.0f,0.0f};
        jogador1->funcaoDoJogador = 1;
        jogador1->rectJogador = (Rectangle){ jogador1->posJogador.x, jogador1->posJogador.y,jogador1->largura,jogador1->altura };
        jogador1->time =1;
    }
    if (jogador2) {
        jogador2->temDominio = 0;
        jogador2->forcaChute = 10.0f;
        jogador2->posJogador = (Vector2){ 600.0f,180.0f };
        jogador2->posJogadorInicial = (Vector2){ 600.0f,180.0f};
        jogador2->velocidadeJogador = (Vector2){0.0f,0.0f};
        jogador2->largura = 24;
        jogador2->altura = 30;
        jogador2->funcaoDoJogador = 2;
        jogador2->rectJogador = (Rectangle){ jogador2->posJogador.x, jogador2->posJogador.y,jogador2->largura,jogador2->altura };
        jogador2->time =1;
    }

    if (jogador3) {
        jogador3->temDominio = 0;
        jogador3->forcaChute = 10.0f;
        jogador3->posJogador = (Vector2){ 250.0f,180.0f };
        jogador3->posJogadorInicial = (Vector2){250.0f ,180.0f};
        jogador3->velocidadeJogador = (Vector2){0.0f,0.0f};
        jogador3->largura = 24;
        jogador3->altura = 30;
        jogador3->funcaoDoJogador = 1;
        jogador3->rectJogador = (Rectangle){ jogador3->posJogador.x, jogador3->posJogador.y,jogador3->largura,jogador3->altura };
        jogador3->time =2;
    }
    if (jogador4) {
        jogador4->temDominio = 0;
        jogador4->forcaChute = 10.0f;
        jogador4->posJogador = (Vector2){ 300,180.0f };
        jogador4->posJogadorInicial = (Vector2){300.0f,180.0f};
        jogador4->velocidadeJogador = (Vector2){0.0f,0.0f};
        jogador4->largura = 24;
        jogador4->altura = 30;
        jogador4->funcaoDoJogador = 2;
        jogador4->rectJogador = (Rectangle){ jogador4->posJogador.x, jogador4->posJogador.y,jogador4->largura,jogador4->altura };
        jogador4->time =2;
    }

    Jogador* jogadorControladoTime1 = jogador1;
    Jogador* jogadorControladoTime2 = jogador3;
    
    pthread_t threadChecarControlado1;
    pthread_t threadChecarControlado2;

    const int screenWidth = 1920;
    const int screenHeight = 1080;
    Color corVerdeGrama;
    corVerdeGrama.r = 124;
    corVerdeGrama.g = 252;
    corVerdeGrama.b = 0;
    corVerdeGrama.a = 1;
    
    InitWindow(screenWidth, screenHeight, "StructSoccer");

    InitAudioDevice();
    SetMasterVolume(1.0f);

    Texture2D campo = LoadTexture("assets/art/backgrounds/pitch-lines.png");
    Texture2D jogador = LoadTexture("assets/art/characters/soccer-player.png");
    Texture2D bola = LoadTexture("assets/art/props/soccer-ball.png");
    Texture2D paredeFundoCampo = LoadTexture("assets/art/backgrounds/side-wall.png");
    Texture2D paredeLadoCima = LoadTexture("assets/art/backgrounds/top-wall.png");
    Texture2D paredeLadoBaixo = LoadTexture("assets/art/backgrounds/bottom-wall.png");
    Texture2D barra = LoadTexture("assets/art/backgrounds/goal-bottom.png");
    Texture2D barraTopo = LoadTexture("assets/art/backgrounds/goal-top.png");
    Texture2D menuBg   = LoadTexture("assets/art/ui/mainmenu/menu-background.png");     
    Texture2D menuLogo = LoadTexture("assets/art/ui/mainmenu/title.png");
    Texture2D goalMensagem = LoadTexture("assets/art/ui/goal.png");

    EstadoDoJogo estado = ST_MENU;
    Menu menu = {
    .itens = {"Classico", "PowerUps"},
    .cont = 2,
    .selecionar = 0
    };

    Music menuMusic = LoadMusicStream("assets/music/menu.mp3");
    menuMusic.looping = true;           
    SetMusicVolume(menuMusic, 0.6f);
    PlayMusicStream(menuMusic);

    //loop do menu
    while (estado == ST_MENU && !WindowShouldClose()) {
        UpdateMusicStream(menuMusic);
        float dt = GetFrameTime();

        BeginDrawing();
            UpdateAndDrawMainMenu(&menu, &estado, menuBg, menuLogo, dt);
        EndDrawing();

        if (IsKeyPressed(KEY_ESCAPE)) {
            //libera apenas as texturas do menu
            UnloadTexture(menuBg);
            UnloadTexture(menuLogo);
            StopMusicStream(menuMusic);
            UnloadMusicStream(menuMusic);
            CloseWindow();
            return;
        }
    }

    //como saiu do menu, nao precisa mais delas
    UnloadTexture(menuBg);
    UnloadTexture(menuLogo);

    Rectangle srcParedeFundoCampoDir = {0,0,paredeFundoCampo.width,-paredeFundoCampo.height};
    Rectangle destParedeFundoCampoDir2 = {805,220,paredeFundoCampo.width,paredeFundoCampo.height};

    Rectangle srcBarraEsquerda = (Rectangle){0,0,-barra.width,barra.height};
    Rectangle destBarraEsquerda = (Rectangle){795,90,barra.width,barra.height};

    Bola* bola1 = (Bola*)malloc(sizeof(Bola));
    if (bola1) {
        bola1->posBola = (Vector2){ 425,180 };
        bola1->posInicialBola = (Vector2){425,180};
        bola1->raioBola = 5;
        bola1->velocidadeAtual = (Vector2){ 0.0f,0.0f };
    }

    Camera2D *camera = (Camera2D *)malloc(sizeof(Camera2D));
    camera->offset = (Vector2){screenWidth/2, screenHeight/2};
    camera->rotation = 0.0f;
    camera->zoom =  3.5f;

    pthread_create(&threadChecarControlado1,NULL,DefinirJogadorControlado,&jogadorControladoTime1);
    pthread_create(&threadChecarControlado2,NULL,DefinirJogadorControlado,&jogadorControladoTime2);

    //prepara ponteiro da lista de sprites para passar por referência
    RectangleSprites* headSpritesJogador_local = headSpritesJogador;

    //configura contexto do jogo
    GameCtx ctx = {
        .campo = campo,
        .jogadorTex = jogador,
        .bolaTex = bola,
        .goalMensagemTex = goalMensagem,
        .paredeFundoCampo = paredeFundoCampo,
        .paredeLadoCima = paredeLadoCima,
        .paredeLadoBaixo = paredeLadoBaixo,
        .barra = barra,
        .barraTopo = barraTopo,

        .srcParedeFundoCampoDir = srcParedeFundoCampoDir,
        .destParedeFundoCampoDir2 = destParedeFundoCampoDir2,
        .srcBarraEsquerda = srcBarraEsquerda,
        .destBarraEsquerda = destBarraEsquerda,

        .jogo = jogo,
        .bola1 = bola1,
        .camera = camera,
        .corVerdeGrama = corVerdeGrama,
        .j1 = jogador1,
        .j2 = jogador2,
        .j3 = jogador3,
        .j4 = jogador4,
        
        .head1 = head1Jogador,
        .head2 = head2Jogador,
        .tail1 = tail1Jogador,
        .tail2 = tail2Jogador,

        .ctrl1 = &jogadorControladoTime1,
        .ctrl2 = &jogadorControladoTime2,

        .headSprites = &headSpritesJogador_local
    };

    SetTargetFPS(60);

    if (estado == ST_CLASSICO) {
        RunModoClassico(&ctx);
    } else if (estado == ST_ARCADE) {
        RunModoPowerUps(&ctx);
    }

    // Libera texturas do gameplay e fecha
    UnloadTexture(campo);
    UnloadTexture(jogador);
    UnloadTexture(bola);
    UnloadTexture(paredeFundoCampo);
    UnloadTexture(paredeLadoCima);
    UnloadTexture(paredeLadoBaixo);
    UnloadTexture(barra);
    UnloadTexture(barraTopo);

    CloseWindow();
    CloseAudioDevice();
}

void AtualizarPosJogador(Jogador * jogador, Jogador * head1 , Jogador * head2, Jogo * jogo) {
    if(jogador->time == 1 && jogo->voltandoDoGol == 0){
        if (IsKeyDown(KEY_RIGHT)) jogador->velocidadeJogador.x = 5.0f;
        if (IsKeyDown(KEY_LEFT)) jogador->velocidadeJogador.x = -5.0f;
        if (IsKeyDown(KEY_UP)) jogador->velocidadeJogador.y = -5.0f;
        if (IsKeyDown(KEY_DOWN)) jogador->velocidadeJogador.y = 5.0f;
    }else if(jogador->time==2 && jogo->voltandoDoGol == 0){
        if (IsKeyDown(KEY_D)) jogador->velocidadeJogador.x = 5.0f;
        if (IsKeyDown(KEY_A)) jogador->velocidadeJogador.x = -5.0f;
        if (IsKeyDown(KEY_W)) jogador->velocidadeJogador.y = -5.0f;
        if (IsKeyDown(KEY_S)) jogador->velocidadeJogador.y = 5.0f;
    }
    TratarColisoesJogadorParede(jogador,jogo->rectangleParedeCima,jogo);
    TratarColisoesJogadorParede(jogador,jogo->rectangleParedeBaixo,jogo);
    TratarColisoesJogadorParede(jogador,jogo->rectangleParedeFundoEsq1,jogo);
    TratarColisoesJogadorParede(jogador,jogo->rectangleParedeFundoEsq2,jogo);
    TratarColisoesJogadorParede(jogador,jogo->rectangleParedeFundoDir1,jogo);
    TratarColisoesJogadorParede(jogador,jogo->rectangleParedeFundoDir2,jogo);
    TratarColisoesJogadorParede(jogador,jogo->linhaGolFim1,jogo);
    TratarColisoesJogadorParede(jogador,jogo->linhaGolFim2,jogo);

    jogador->posJogador.x += jogador->velocidadeJogador.x;
    jogador->posJogador.y += jogador->velocidadeJogador.y;
    if(jogador->velocidadeJogador.x != 0.0f || jogador->velocidadeJogador.y != 0.0f){
        jogador->isMovendo =1;
    }
    else if(jogo->voltandoDoGol == 0){
        jogador->isMovendo = 0;
    }
    jogador->rectJogador.x = jogador->posJogador.x;
    jogador->rectJogador.y = jogador->posJogador.y;

    jogador->velocidadeJogador.x = 0;
    jogador->velocidadeJogador.y = 0;
}

void EstadoBola(Bola * bola, Jogador * jogador,Jogador * head1, Jogador * tail1,Jogador * head2,Jogador * tail2, Jogo * jogo) {
    if (!jogador->temDominio) {
        if (CheckCollisionCircleRec(bola->posBola, bola->raioBola, jogador->rectJogador)) {
            jogador->temDominio = 1; 
            bola->velocidadeAtual.x = 0.0f;
            bola->velocidadeAtual.y = 0.0f;
            jogo->timeComBola = jogador->time;
            TratamentoColisaoJogadorBola(jogador,bola,head1,tail1,head2,tail2, jogo);
        }
    }
    else{//Tem dominio sendo true
        if(jogador->time == 1 && jogo->voltandoDoGol == 0){

            if(IsKeyDown(KEY_UP)){
                bola->posBola.x = jogador->posJogador.x + jogador->rectJogador.width + bola->raioBola;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height;
                bola->ladoBola =2;
                if(IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_UP)){
                    bola->ladoBola =4;//Diagonal superior esquerda;
                }
                else if(IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_UP)){
                    bola->ladoBola = 5; //Diagonal superior direita
                }
            }   
            else if (IsKeyDown(KEY_RIGHT)) {
                bola->posBola.x = jogador->posJogador.x + jogador->rectJogador.width + bola->raioBola;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height;
                bola->ladoBola = 0;//Direita
               
            }
            else if (IsKeyDown(KEY_LEFT)) {
                bola->posBola.x = jogador->posJogador.x - bola->raioBola;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height;
                bola->ladoBola = 1;//Esquerda
            }
            else if(IsKeyDown(KEY_DOWN)){
                bola->posBola.x = jogador->posJogador.x - bola->raioBola;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height;
                bola->ladoBola = 3;
                if(IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_DOWN)){
                    bola->ladoBola =6;//Diagonal inferior esquerda;
                }
                else if(IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_DOWN)){
                    bola->ladoBola = 7; //Diagonal inferior direita
                }
            }
        }
        else if(jogador->time== 2 && jogo->voltandoDoGol == 0){
            if(IsKeyDown(KEY_W)){
                bola->posBola.x = jogador->posJogador.x + jogador->rectJogador.width + bola->raioBola;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height;
                bola->ladoBola =2;
                if(IsKeyDown(KEY_A) && IsKeyDown(KEY_W)){
                    bola->ladoBola =4;//Diagonal superior esquerda;
                }
                else if(IsKeyDown(KEY_D) && IsKeyDown(KEY_W)){
                    bola->ladoBola = 5; //Diagonal superior direita
                }
            }   
            else if (IsKeyDown(KEY_D)) {
                bola->posBola.x = jogador->posJogador.x + jogador->rectJogador.width + bola->raioBola;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height;
                bola->ladoBola = 0;//Direita
            }   
            else if (IsKeyDown(KEY_A)) {
                bola->posBola.x = jogador->posJogador.x - bola->raioBola;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height;
                bola->ladoBola = 1;//Esquerda
            }
            else if(IsKeyDown(KEY_S)){
                bola->posBola.x = jogador->posJogador.x - bola->raioBola;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height;
                bola->ladoBola =3;//baixo
                if(IsKeyDown(KEY_A) && IsKeyDown(KEY_S)){
                    bola->ladoBola =6;//Diagonal inferior esquerda;
                }
                else if(IsKeyDown(KEY_D) && IsKeyDown(KEY_S)){
                    bola->ladoBola = 7; //Diagonal inferior direita
                }
            }
        }
    }
}

void Atrito(Bola * bola) {
    if (bola->velocidadeAtual.x > 0.0f) {
        bola->velocidadeAtual.x -= 0.5f;
        if (bola->velocidadeAtual.x < 0.0f) bola->velocidadeAtual.x = 0.0f;
    }
    if (bola->velocidadeAtual.x < 0.0f) {
        bola->velocidadeAtual.x += 0.5f;
        if (bola->velocidadeAtual.x > 0.0f) bola->velocidadeAtual.x = 0.0f;
    }
    if (bola->velocidadeAtual.y > 0.0f) {
        bola->velocidadeAtual.y -= 0.5f;
        if (bola->velocidadeAtual.y < 0.0f) bola->velocidadeAtual.y = 0.0f;
    }
    if (bola->velocidadeAtual.y < 0.0f) {
        bola->velocidadeAtual.y += 0.5f;
        if (bola->velocidadeAtual.y > 0.0f) bola->velocidadeAtual.y = 0.0f;
    }
}

void Passe(Bola * bola, Jogador * jogador, Jogo * jogo) {
    //Semicolon é o ç.
    if((jogador->temDominio && jogador->time ==1 && IsKeyDown(KEY_SEMICOLON)) || (jogador->temDominio && jogador->time ==2 && IsKeyDown(KEY_C))) {
        if (bola->ladoBola == 0) {
            bola->velocidadeAtual.x += 20.0f;
        }
        else if (bola->ladoBola == 1) {
            bola->velocidadeAtual.x -= 20.0f;
        }
        else if (bola->ladoBola == 2) {
            bola->velocidadeAtual.y -= 20.0f;
        }
        else if (bola->ladoBola == 3) {
            bola->velocidadeAtual.y += 20.0f;
        }
        else if(bola->ladoBola == 4){
            bola->velocidadeAtual.x -= 12;
            bola->velocidadeAtual.y -= 12;
        }
        else if(bola->ladoBola == 5){
            bola->velocidadeAtual.x += 12;
            bola->velocidadeAtual.y -= 12;
        }
        else if(bola->ladoBola ==  6){
            bola->velocidadeAtual.x -= 12;
            bola->velocidadeAtual.y += 12;
        }
        else if(bola->ladoBola == 7){
            bola->velocidadeAtual.x += 12;
            bola->velocidadeAtual.y +=12;
        }
        jogador->temDominio = 0;
        jogo->timeComBola = 0;
    }
}

void Chutar(Bola* bola, Jogador* jogador, Jogo * jogo) {
    if ((jogador->temDominio && jogador->time== 1 && IsKeyPressed(KEY_PERIOD)) || (jogador->temDominio && jogador->time==2 && IsKeyDown(KEY_LEFT_CONTROL))) {
        
        if (bola->ladoBola == 0) {
            bola->velocidadeAtual.x += 40.0f;
        }
        else if (bola->ladoBola == 1) {
            bola->velocidadeAtual.x -= 40.0f;
        }
        else if (bola->ladoBola == 2) {
            bola->velocidadeAtual.y -= 40.0f;
        }
        else if (bola->ladoBola == 3) {
            bola->velocidadeAtual.y += 40.0f;
        }
               else if(bola->ladoBola == 4){
            bola->velocidadeAtual.x -= 30;
            bola->velocidadeAtual.y -= 30;
        }
        else if(bola->ladoBola == 5){
            bola->velocidadeAtual.x += 30;
            bola->velocidadeAtual.y -= 30;
        }
        else if(bola->ladoBola ==  6){
            bola->velocidadeAtual.x -= 30;
            bola->velocidadeAtual.y += 30;
        }
        else if(bola->ladoBola == 7){
            bola->velocidadeAtual.x += 30;
            bola->velocidadeAtual.y +=30;
        }
        jogador->temDominio = 0;
        jogo->timeComBola = 0;
    }
}

void MudarPosicaoBola(Bola * bola) {
    bola->posBola.x += bola->velocidadeAtual.x;
    bola->posBola.y += bola->velocidadeAtual.y;
}

void * DefinirJogadorControlado(void * jogadorAtual){
    while(1){
        int mudou =0;
        Jogador ** jogador1 = (Jogador **)jogadorAtual;

        if((*jogador1)->temDominio == 0 && (*jogador1)->time == 1){
            while(!IsKeyPressed(KEY_RIGHT_SHIFT) && (*jogador1)->temDominio == 0);
            if((*jogador1)->temDominio == 0){
                pthread_mutex_lock(&lock);
                *jogador1 = (*jogador1)->prox;
                mudou =1;
                pthread_mutex_unlock(&lock);
            }
        }
        else if((*jogador1)->temDominio == 0 && (*jogador1)->time == 2){
            while(!IsKeyPressed(KEY_LEFT_SHIFT) && (*jogador1)->temDominio == 0 );
            if((*jogador1)->temDominio == 0){
                pthread_mutex_lock(&lock);
                *jogador1 = (*jogador1)->prox;
                mudou =1;
                pthread_mutex_unlock(&lock);
            }
        }
        if(mudou){  
            usleep(500000);
        }
    }
}

void TratamentoColisaoJogadorBola(Jogador * jogador, Bola * bola, Jogador *head1, Jogador * tail1,Jogador * head2, Jogador * tail2, Jogo * jogo){
    if(jogo->timeComBola != 0 ){
        if(jogador->time == 1){
            do{
                if(head2->temDominio == 1){
                    break;
                }
                head2 = head2->prox;
            }while(head2!=tail2->prox);
            head2->temDominio = 0;
            jogador->temDominio =1;
            jogo->timeComBola = 1;
            
        }
        else if(jogador->time == 2){
            do{
                if(head1->temDominio == 1){
                    break;
                }
                head1 = head1->prox;
            }while(head1!=tail1->prox);
            head1->temDominio = 0;
            jogador->temDominio = 1;
            jogo->timeComBola = 2;
        }
    }
}



void AtualizarCamera(Camera2D * camera, Jogo  * jogo, Jogador * jogadorControladoTime1, Jogador* jogadorControladoTime2,Bola * bola){
    if(jogo->timeComBola == 0){
        camera->target = bola->posBola;
    }
    else if(jogo->timeComBola == 1){
        camera->target = jogadorControladoTime1->posJogador;
    }
    else if(jogo->timeComBola == 2){
        camera->target = jogadorControladoTime2->posJogador;
    }
}

void desenharTexturaBola(Texture2D bola, Bola * bola1, int contadorFrames, Jogador * jogadorControladoTime1, Jogador * jogadorControladoTime2){
    Rectangle src1 = {0,0,10,10};
    Rectangle src2 = {20,20,10,10};
    Rectangle src3 = {30,30,10,10};
    Rectangle src4 = {40,40,10,10};
    Rectangle dest = {bola1->posBola.x, bola1->posBola.y,10,10};
    Vector2 origin = {dest.width/2,dest.height/2};

    if((bola1->velocidadeAtual.x == 0.0f && bola1->velocidadeAtual.y == 0.0f && jogadorControladoTime1->temDominio == 0 && jogadorControladoTime2->temDominio == 0) || ((jogadorControladoTime1->temDominio == 1 && jogadorControladoTime1->velocidadeJogador.x == 0.0f && jogadorControladoTime1->velocidadeJogador.y == 0.0f) || (jogadorControladoTime2->temDominio == 1 && jogadorControladoTime2->velocidadeJogador.x == 0.0f && jogadorControladoTime2->velocidadeJogador.y==0.0f))){
        DrawTexturePro(bola,src1,dest,origin,0.0f,WHITE);
    }
    else{
        if(contadorFrames<=15){
            DrawTexturePro(bola,src2,dest,origin,0.0f,WHITE);
        }
        else if(contadorFrames<=30){
            DrawTexturePro(bola,src2,dest,origin,0.0f,WHITE);
        }
        else if(contadorFrames<=45){
            DrawTexturePro(bola,src3,dest,origin,0.0f,WHITE);
        }
        else if(contadorFrames<=60){
            DrawTexturePro(bola,src4,dest,origin,0.0f,WHITE);
        }
    }
}

void desenharTexturaJogador(Texture2D jogador, Bola * bola1, Jogador * jogador1,RectangleSprites ** headSprites, int contadorFramesJogador) {
    Vector2 origin = (Vector2){0,0};
    Rectangle dest  = (Rectangle){ jogador1->posJogador.x, jogador1->posJogador.y, 24, 32 };
    Rectangle srcIdle = (Rectangle){ 0, 0, 24, 32 };

    // parado = idle, andando = animação na lista
    Rectangle srcDaVez;
    if (jogador1->isMovendo == 0) {
        srcDaVez = srcIdle;
    } else {
        srcDaVez = (*headSprites)->Rectangle;
        if (contadorFramesJogador % 10 == 0) {
            *headSprites = (*headSprites)->prox;
        }
    }

    bool faceLeft = false;

    if (jogador1->isMovendo) {
        if (jogador1->time == 1) {
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_DOWN)) faceLeft = true;
        } else { // time 2
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_S))      faceLeft = true;
        }
    } else {
        // Parado: olha para a bola
        float playerCenterX = jogador1->posJogador.x + jogador1->rectJogador.width * 0.5f;
        float ballX         = bola1->posBola.x;
        faceLeft            = (ballX < playerCenterX);
    }

    srcDaVez.width = faceLeft ? -fabsf(srcDaVez.width) :  fabsf(srcDaVez.width);

    DrawTexturePro(jogador, srcDaVez, dest, origin, 0.0f, WHITE);
}

void TratarColisoesParedeBola(Bola * bola, Rectangle rectangleParede, Jogo * jogo){
    if(bola->velocidadeAtual.x!= 0.0f || bola->velocidadeAtual.y !=0.0f){
        if(CheckCollisionCircleRec(bola->posBola,bola->raioBola,rectangleParede)){
          
            if((rectangleParede.x == jogo->rectangleParedeCima.x && rectangleParede.y == jogo->rectangleParedeCima.y) || (rectangleParede.x == jogo->rectangleParedeBaixo.x && rectangleParede.y == jogo->rectangleParedeBaixo.y)){
                bola->velocidadeAtual.y = -(bola->velocidadeAtual.y);
            }
            else if((rectangleParede.x == jogo->rectangleParedeFundoDir1.x && rectangleParede.y == jogo->rectangleParedeFundoDir1.y) ||
            (rectangleParede.x == jogo->rectangleParedeFundoDir2.x && rectangleParede.y == jogo->rectangleParedeFundoDir2.y) || 
            (rectangleParede.x == jogo->rectangleParedeFundoEsq1.x && rectangleParede.y == jogo->rectangleParedeFundoEsq1.y) ||
            (rectangleParede.x == jogo->rectangleParedeFundoEsq2.x && rectangleParede.y == jogo->rectangleParedeFundoEsq2.y) || 
            (rectangleParede.x == jogo->linhaGolFim1.x && rectangleParede.y == jogo->linhaGolFim1.y) || (rectangleParede.x == jogo->linhaGolFim2.x && rectangleParede.y == jogo->linhaGolFim2.y)){
                bola->velocidadeAtual.x = -(bola->velocidadeAtual.x);
            }
        }
    }
}



void TratarColisoesJogadorParede(Jogador * jogador, Rectangle rectangleParede ,Jogo * jogo){

    Rectangle rectangleLinhaColisao = (Rectangle){jogador->posJogador.x,jogador->posJogador.y+20,jogador->largura,2};

    if(CheckCollisionRecs(rectangleLinhaColisao,rectangleParede)){
        if(jogador->time == 1){
            if(IsKeyDown(KEY_UP) && (rectangleParede.x == jogo->rectangleParedeCima.x && rectangleParede.y == jogo->rectangleParedeCima.y )){
                jogador->velocidadeJogador.y = 0.0f;
            }
            else if(IsKeyDown(KEY_DOWN) && (rectangleParede.x == jogo->rectangleParedeBaixo.x && rectangleParede.y == jogo->rectangleParedeBaixo.y )){
                jogador->velocidadeJogador.y = 0.0f;
            } 
            else if(IsKeyDown(KEY_LEFT) && ((rectangleParede.x == jogo->rectangleParedeFundoEsq1.x && rectangleParede.y == jogo->rectangleParedeFundoEsq1.y ) || 
            (rectangleParede.x == jogo->rectangleParedeFundoEsq2.x && rectangleParede.y == jogo->rectangleParedeFundoEsq2.y ))){
                jogador->velocidadeJogador.x = 0.0f;
            }
            else if(IsKeyDown(KEY_RIGHT) && ((rectangleParede.x == jogo->rectangleParedeFundoDir1.x && rectangleParede.y == jogo->rectangleParedeFundoDir1.y ) || 
            (rectangleParede.x == jogo->rectangleParedeFundoDir2.x && rectangleParede.y == jogo->rectangleParedeFundoDir2.y ))){
                jogador->velocidadeJogador.x = 0.0f;
            }
            else if(IsKeyDown(KEY_LEFT) && (rectangleParede.x == jogo->linhaGolFim1.x && rectangleParede.y == jogo->linhaGolFim1.y)){
                jogador->velocidadeJogador.x = 0.0f;
            }
            else if(IsKeyDown(KEY_RIGHT) && (rectangleParede.x == jogo->linhaGolFim2.x && rectangleParede.y == jogo->linhaGolFim2.y)){
                jogador->velocidadeJogador.x = 0.0f;
            }
            
        }
        else if (jogador->time == 2){
            if(IsKeyDown(KEY_W) && (rectangleParede.x == jogo->rectangleParedeCima.x && rectangleParede.y == jogo->rectangleParedeCima.y )){
                jogador->velocidadeJogador.y = 0.0f;
            }
            else if(IsKeyDown(KEY_S) && (rectangleParede.x == jogo->rectangleParedeBaixo.x && rectangleParede.y == jogo->rectangleParedeBaixo.y )){
                jogador->velocidadeJogador.y = 0.0f;
            } 
            else if(IsKeyDown(KEY_A) && ((rectangleParede.x == jogo->rectangleParedeFundoEsq1.x && rectangleParede.y == jogo->rectangleParedeFundoEsq1.y ) || 
            (rectangleParede.x == jogo->rectangleParedeFundoEsq2.x && rectangleParede.y == jogo->rectangleParedeFundoEsq2.y ))){
                jogador->velocidadeJogador.x = 0.0f;
            }
            else if(IsKeyDown(KEY_D) && ((rectangleParede.x == jogo->rectangleParedeFundoDir1.x && rectangleParede.y == jogo->rectangleParedeFundoDir1.y ) || 
            (rectangleParede.x == jogo->rectangleParedeFundoDir2.x && rectangleParede.y == jogo->rectangleParedeFundoDir2.y ))){
                jogador->velocidadeJogador.x = 0.0f;
            }
            else if(IsKeyDown(KEY_D) && (rectangleParede.x == jogo->linhaGolFim1.x && rectangleParede.y == jogo->linhaGolFim1.y)){
                jogador->velocidadeJogador.x = 0.0f;
            }
            else if(IsKeyDown(KEY_D) && (rectangleParede.x == jogo->linhaGolFim2.x && rectangleParede.y == jogo->linhaGolFim2.y)){
                jogador->velocidadeJogador.x = 0.0f;
            }
        }

    }

    
}

void tratarGol(Jogo * jogo, Bola * bola, Jogador * head1, Jogador * tail1, Jogador *head2, Jogador * tail2){

    if(jogo->voltandoDoGol == 1){
        int estaNaInicial = 1;
        do{
            if(head1->posJogador.x != head1->posJogadorInicial.x || head1->posJogador.y != head1->posJogador.y){
                estaNaInicial = 0;
            }
            else if(head2->posJogador.x != head2->posJogadorInicial.x || head2->posJogador.y != head2->posJogador.y){
                estaNaInicial = 0;
            }
            head1=head1->prox;
            head2 = head2->prox;
        }while(head1!=tail1->prox && head2!=head2->prox);

        if(estaNaInicial == 0){
            movimentoAutomatico(jogo,head1,tail1,head2,tail2);
        }
        else if(estaNaInicial == 1){
            bola->posBola.x = bola->posInicialBola.x;
            bola->posBola.y = bola->posInicialBola.y;
            jogo->voltandoDoGol = 0;
        }
    }
    else if(CheckCollisionCircleRec(bola->posBola,bola->raioBola,jogo->linhaGol1) && jogo->voltandoDoGol == 0){
        jogo->placarTime2+=1;
        jogo->voltandoDoGol = 1;
        Jogador * aux = head1;
        Jogador * aux2 = head2;
        do{
          
          if(aux->temDominio == 1) aux->temDominio = 0;
          if(aux2->temDominio ==1) aux2->temDominio = 0;
          head1 = head1->prox;
          head2 = head2->prox;
        }while(head1!=tail1->prox && head2!=tail2->prox);
    
    }
    else if(CheckCollisionCircleRec(bola->posBola,bola->raioBola,jogo->linhaGol2) && jogo->voltandoDoGol == 0){
        jogo->placarTime1+=1;
        jogo->voltandoDoGol = 1;
        Jogador * aux = head1;
        Jogador * aux2 = head2;
        do{
          
          if(aux->temDominio == 1) aux->temDominio = 0;
          if(aux2->temDominio ==1) aux2->temDominio = 0;
          head1 = head1->prox;
          head2 = head2->prox;
        }while(head1!=tail1->prox && head2!=tail2->prox);
    
    }

}

void movimentoAutomatico(Jogo * jogo, Jogador * head1, Jogador * tail1, Jogador * head2, Jogador *tail2){
    if(jogo->voltandoDoGol == 1){
        do{
            if(head1->posJogador.x > head1->posJogadorInicial.x){
                head1->isMovendo = 1;
                head1->posJogador.x -=1;
            }
            else if(head1->posJogador.x < head1->posJogadorInicial.x){
                head1->isMovendo = 1;
                head1->posJogador.x +=1;
            }

            if(head1->posJogador.y >head1->posJogadorInicial.y){
                head1->isMovendo = 1;
                head1->posJogador.y -=1;
            }
            else if(head1->posJogador.y < head1->posJogadorInicial.y){
                head1->isMovendo = 1;
                head1->posJogador.y +=1;
            }

            if(head2->posJogador.x > head2->posJogadorInicial.x){
                head2->isMovendo = 1;
                head2->posJogador.x -=1;
            }
            else if(head2->posJogador.x < head2->posJogadorInicial.x){
                head2->isMovendo = 1;
                head2->posJogador.x +=1;
            }
            
            if(head2->posJogador.y >head2->posJogadorInicial.y){
                head2->isMovendo = 1;
                head2->posJogador.y -=1;
            }
            else if(head2->posJogador.y < head2->posJogadorInicial.y){
                head2->isMovendo = 1;
                head2->posJogador.y +=1;
            }
            head1 = head1->prox;
            head2 = head2->prox;
        }while(head1!=tail1->prox && head2!=tail2->prox);

    }
}

void movimentoAutomaticoJogo(Jogo * jogo,Bola * bola, Jogador * jogadorControladoTime1, Jogador * jogadorControladoTime2, Jogador * head1,Jogador * tail1, Jogador * head2,Jogador * tail2){
    if(jogo->voltandoDoGol == 0){
        int timeComBola = 0;
        if(jogadorControladoTime1->temDominio == 1) timeComBola = 1;
        else if(jogadorControladoTime2->temDominio == 1) timeComBola = 2;

        do{
            if(timeComBola == 1){
                if(head1!=jogadorControladoTime1){
                    if(head1->funcaoDoJogador == 1){
                        if(bola->posBola.x>550){
                            if(head1->posJogador.x <550){
                                head1->posJogador.x+=1;
                            }
                            else if(head1->posJogador.x<300){
                                head1->posJogador.x +=3;
                            }

                            if(head1->posJogador.y>bola->posBola.y){
                                head1->posJogador.y += 3;
                            }
                            else if(head1->posJogador.y<bola->posBola.y){
                                head1->posJogador.y +=3;
                            }
                        }
                        else if(bola->posBola.x>550){
                            if(head1->posJogador.x>300) head1->posJogador.x -= 3;
                            if(head1->posJogador.y == head2->posJogador.y) head1->posJogador.y += 2;
 //                           else if(head1->posJogador.y>)
                        }
                        
                    }
                    
                    else if(head1->funcaoDoJogador == 2 && head2 ){

                    }
                }
            }

        }while(head1 != tail1->prox);

    }
}