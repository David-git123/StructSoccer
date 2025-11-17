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
    jogo->listaPowerUps = NULL;
    jogo->rectangleParedeCima = (Rectangle){50,20,750,10};
    jogo->rectangleParedeBaixo = (Rectangle){28,355,800,10};
    jogo->rectangleParedeFundoEsq1 = (Rectangle){55,25,10,110};
    jogo->rectangleParedeFundoEsq2 = (Rectangle){38,215,10,144};
    jogo->rectangleParedeFundoDir1 = (Rectangle){785,30,10,120};
    jogo->rectangleParedeFundoDir2 = (Rectangle){810,220,10,140};
    
    //rectangleGol1 e rectangleGol2 podem ser excluidos (David)
    // jogo->rectangleGol1.x= 1720.0f;
    // jogo->rectangleGol1.y = 540.0f;
    // jogo->rectangleGol1.height = 300.0f;
    // jogo->rectangleGol1.width = 200.0f;
    
    
    // jogo->rectangleGol2.x= 0;
    // jogo->rectangleGol2.y = 540.0f;
    // jogo->rectangleGol2.height = 300.0f;
    // jogo->rectangleGol2.width = 200.0f;
    
    jogo->linhaGol1 = (Rectangle){10,153,50,70};
    jogo->linhaGol2 = (Rectangle){800,145,50,70};
    jogo->linhaGolFim1 = (Rectangle){30,153,2,70};
    jogo->linhaGolFim2 = (Rectangle){830,145,2,70};
    
    jogo->voltandoDoGol = 0;
    Jogador* head1Jogador = NULL;
    Jogador* tail1Jogador = NULL;
    
    Jogador* head2Jogador = NULL;
    Jogador* tail2Jogador = NULL;
    //time 1
    Jogador* jogador1 = (Jogador*)malloc(sizeof(Jogador));
    Jogador* jogador2 = (Jogador*)malloc(sizeof(Jogador));
    Jogador* jogador5 = (Jogador *)malloc(sizeof(Jogador));
    //time 2
    Jogador* jogador3 = (Jogador*)malloc(sizeof(Jogador));
    Jogador* jogador4 = (Jogador*)malloc(sizeof(Jogador));
    Jogador* jogador6 = (Jogador*)malloc(sizeof(Jogador));
    
    Jogador * goleiro1 = (Jogador *)malloc(sizeof(Jogador));
    Jogador * goleiro2 = (Jogador *)malloc(sizeof(Jogador));
    
    goleiro1->posJogador.x = 70;
    goleiro1->posJogador.y = 153;
    goleiro1->funcaoDoJogador = 4;

    goleiro2->posJogador.x = 780;
    goleiro2->posJogador.y = 145;
    goleiro2->funcaoDoJogador = 4;

    goleiro1->rectJogador = (Rectangle){55,153,10,10};
    goleiro1->time = 1;
    goleiro1->gols = 0;   
    
    goleiro2->rectJogador = (Rectangle){790,145,10,10};
    goleiro2->time = 2;
    goleiro2->gols = 0;
    
    
    head1Jogador = jogador1;
    jogador1->prox = jogador5;  
    jogador5->prox = jogador2;
    jogador2->prox = head1Jogador;
    tail1Jogador = jogador2;
    
    head2Jogador = jogador3;
    jogador3->prox = jogador6;
    jogador6->prox = jogador4;
    jogador4->prox = head2Jogador;
    tail2Jogador = jogador4;
    
    Rectangle src1 = {0,0,24,32};
    //Sprites de jogador correndo
    RectangleSprites * headSpritesJogador;
    Rectangle srcJ2 = {0,32,24,32};
    Rectangle srcJ3 = {32,32,24,32};
    Rectangle srcJ4 = {64,32,24,32};
    Rectangle srcJ5 = {96,32,24,32};
    Rectangle srcJ6 = {128,32,24,32};
    Rectangle srcJ7 = {160,32,24,32};
    
    Rectangle rectangleCampo = {60,20,80,300};
    
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
        jogador1->gols = 0;
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
        jogador2->gols = 0;
    }
    if(jogador5){
        jogador5->temDominio = 0;
        jogador5->forcaChute = 10.0f;
        jogador5->posJogador = (Vector2){ 600.0f,150.0f };
        jogador5->posJogadorInicial = (Vector2){ 600.0f,150.0f};
        jogador5->velocidadeJogador = (Vector2){0.0f,0.0f};
        jogador5->largura = 24;
        jogador5->altura = 30;
        jogador5->funcaoDoJogador = 3;
        jogador5->rectJogador = (Rectangle){ jogador5->posJogador.x, jogador5->posJogador.y,jogador5->largura,jogador5->altura };
        jogador5->time =1;
        jogador5->gols = 0;
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
        jogador3->gols = 0;
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
        jogador4->gols = 0;
    }
    if (jogador6) {
        jogador6->temDominio = 0;
        jogador6->forcaChute = 10.0f;
        jogador6->posJogador = (Vector2){ 300.0f,200.0f };
        jogador6->posJogadorInicial = (Vector2){300.0f,200.0f};
        jogador6->velocidadeJogador = (Vector2){0.0f,0.0f};
        jogador6->largura = 24;
        jogador6->altura = 30;
        jogador6->funcaoDoJogador = 3;
        jogador6->rectJogador = (Rectangle){ jogador6->posJogador.x, jogador6->posJogador.y,jogador6->largura,jogador6->altura };
        jogador6->time =2;
        jogador6->gols = 0;
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
    Texture2D time2Textura = LoadTexture("assets/art/characters/struct_soccer_t2.png");
    Texture2D texturaJogadorControlado1 = LoadTexture("assets/art/props/1p.png");
    Texture2D texturaJogadorControlado2 = LoadTexture("assets/art/props/2p.png");
    Texture2D texturaGoleiro = LoadTexture("assets/art/characters/GOLEIRO.png");
    Texture2D goalMensagem = LoadTexture("assets/art/ui/goal.png");
    Music gameplayMusic = LoadMusicStream("assets/music/gameplay.mp3");
    Music tournamentMusic = LoadMusicStream("assets/music/tournament.mp3");

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
        .texturaTime2 = time2Textura,
        .txtJogadorControlado1 = texturaJogadorControlado1,
        .txtJogadorControlado2 = texturaJogadorControlado2,
        .txtGoleiro = texturaGoleiro,
        .srcParedeFundoCampoDir = srcParedeFundoCampoDir,
        .destParedeFundoCampoDir2 = destParedeFundoCampoDir2,
        .srcBarraEsquerda = srcBarraEsquerda,
        .destBarraEsquerda = destBarraEsquerda,
        .goalMensagemTex = goalMensagem,
        .musicPartida = gameplayMusic,
        .musicTorneio = tournamentMusic,

        .jogo = jogo,
        .bola1 = bola1,
        .camera = camera,
        .corVerdeGrama = corVerdeGrama,
        
        
        .j1 = jogador1,
        .j2 = jogador2,
        .j3 = jogador3,
        .j4 = jogador4,
        .j5 = jogador5,
        .j6 = jogador6,

        .goleiro1 = goleiro1,
        .goleiro2 = goleiro2,

        .rectangleCampo = rectangleCampo,

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
    UnloadMusicStream(ctx.musicPartida);

    CloseWindow();
    CloseAudioDevice();
}

void AtualizarPosJogador(Jogador * jogador, Jogador * head1 ,Jogador *tail1, Jogador * head2,Jogador *tail2, Jogo * jogo) {
    // 1) Se o time está congelado, não anda
    if (jogo->congeladoTimerTime1 > 0 && jogador->time == 1) {
        do{
            jogador->velocidadeJogador = (Vector2){0,0};
            jogador->isMovendo = 0;
            head2 = head2->prox;
        }while(head2 != (tail2)->prox);
        return;
    }
    if (jogo->congeladoTimerTime2 > 0 && jogador->time == 2) {
        do{
            jogador->velocidadeJogador = (Vector2){0,0};
            jogador->isMovendo = 0;
            head1 = head1->prox;
        }while(head1 != (tail1)->prox);
        return;
    }

    float vel = 3.0f + jogador->velocidadeBonus; 

    if(jogador->time == 1 && jogo->voltandoDoGol == 0){
        if (IsKeyDown(KEY_RIGHT) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) jogador->velocidadeJogador.x = vel;
        if (IsKeyDown(KEY_LEFT) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_LEFT)) jogador->velocidadeJogador.x = -vel;
        if (IsKeyDown(KEY_UP) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_UP)) jogador->velocidadeJogador.y = -vel;
        if (IsKeyDown(KEY_DOWN) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_DOWN)) jogador->velocidadeJogador.y = vel;
    }else if(jogador->time==2 && jogo->voltandoDoGol == 0){
        if (IsKeyDown(KEY_D) || IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) jogador->velocidadeJogador.x = vel;
        if (IsKeyDown(KEY_A) || IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_LEFT)) jogador->velocidadeJogador.x = -vel;
        if (IsKeyDown(KEY_W) || IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_UP)) jogador->velocidadeJogador.y = -vel;
        if (IsKeyDown(KEY_S) || IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_DOWN)) jogador->velocidadeJogador.y = vel;
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
        jogador->isMovendo = 1;
    }
    else{
        jogador->isMovendo = 0;
    }

    jogador->rectJogador.x = jogador->posJogador.x;
    jogador->rectJogador.y = jogador->posJogador.y;

    jogador->velocidadeJogador.x = 0;
    jogador->velocidadeJogador.y = 0;
}


void EstadoBola(Bola * bola, Jogador * jogador,Jogador ** jogadorControladoTime1, Jogador ** jogadorControladoTime2,Jogador * goleiro1, Jogador * goleiro2,Jogador  * head1, Jogador * tail1,Jogador * head2,Jogador * tail2, Jogo * jogo) {
    TratamentoColisaoJogadorBola(*jogadorControladoTime1,*jogadorControladoTime2,goleiro1,goleiro2,bola,head1,tail1,head2,tail2, jogo);
    if (!jogador->temDominio) {
        if (CheckCollisionCircleRec(bola->posBola, bola->raioBola, jogador->rectJogador)) {
            jogador->temDominio = 1; 
            bola->velocidadeAtual.x = 0.0f;
            bola->velocidadeAtual.y = 0.0f;
            jogo->timeComBola = jogador->time;
            if(jogador->time == 1){
                *jogadorControladoTime1 = jogador;
            }
            else if(jogador->time == 2){
                *jogadorControladoTime2 = jogador;
            }
            do{
                if(jogador!= head1 && jogador!=head2){
                    head1->temDominio = 0;
                    head2->temDominio = 0;
                }
                head1 = head1->prox;
                head2 = head2->prox;
            }while(head2!=tail2->prox || head1!=tail1->prox);
        }
    }
    else{//Tem dominio sendo true
        if(jogador->time == 1 && jogo->voltandoDoGol == 0){

            if(IsKeyDown(KEY_UP) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_UP)){
                bola->posBola.x = jogador->posJogador.x + jogador->rectJogador.width + bola->raioBola;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height;
                bola->ladoBola =2;
                if(IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_UP) || (IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_UP) && IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_LEFT))){
                    bola->ladoBola =4;//Diagonal superior esquerda;
                }
                else if(IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_UP) || (IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_UP) && IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_RIGHT))){
                    bola->ladoBola = 5; //Diagonal superior direita
                }
            }   
            else if (IsKeyDown(KEY_RIGHT) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
                bola->posBola.x = jogador->posJogador.x + jogador->rectJogador.width + bola->raioBola;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height;
                bola->ladoBola = 0;//Direita
               
            }
            else if (IsKeyDown(KEY_LEFT) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
                bola->posBola.x = jogador->posJogador.x - bola->raioBola;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height;
                bola->ladoBola = 1;//Esquerda
            }
            else if(IsKeyDown(KEY_DOWN) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_DOWN)){
                bola->posBola.x = jogador->posJogador.x - bola->raioBola;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height;
                bola->ladoBola = 3;
                if(IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_DOWN) || (IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_DOWN) && IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_LEFT))){
                    bola->ladoBola =6;//Diagonal inferior esquerda;
                }
                else if(IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_DOWN) || (IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_DOWN) && IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_RIGHT))){
                    bola->ladoBola = 7; //Diagonal inferior direita
                }
            }
        }
        else if(jogador->time== 2 && jogo->voltandoDoGol == 0){
            if(IsKeyDown(KEY_W) || IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_UP)){
                bola->posBola.x = jogador->posJogador.x + jogador->rectJogador.width + bola->raioBola;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height;
                bola->ladoBola =2;
                if(IsKeyDown(KEY_A) && IsKeyDown(KEY_W) || (IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_UP) && IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_LEFT))){
                    bola->ladoBola =4;//Diagonal superior esquerda;
                }
                else if(IsKeyDown(KEY_D) && IsKeyDown(KEY_W) || (IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_UP) && IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_RIGHT))){
                    bola->ladoBola = 5; //Diagonal superior direita
                }
            }   
            else if (IsKeyDown(KEY_D) || IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
                bola->posBola.x = jogador->posJogador.x + jogador->rectJogador.width + bola->raioBola;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height;
                bola->ladoBola = 0;//Direita
            }   
            else if (IsKeyDown(KEY_A) || IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
                bola->posBola.x = jogador->posJogador.x - bola->raioBola;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height;
                bola->ladoBola = 1;//Esquerda
            }
            else if(IsKeyDown(KEY_S) || IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_DOWN)){
                bola->posBola.x = jogador->posJogador.x - bola->raioBola;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height;
                bola->ladoBola =3;//baixo
                if(IsKeyDown(KEY_A) && IsKeyDown(KEY_S) || (IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_LEFT) && IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_DOWN))){
                    bola->ladoBola =6;//Diagonal inferior esquerda;
                }
                else if(IsKeyDown(KEY_D) && IsKeyDown(KEY_S)|| (IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_UP) && IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_RIGHT))){
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
    if(bola->posBola.x>780 || bola->posBola.x<55){
        bola->velocidadeAtual.x - bola->velocidadeAtual.x;
    }
    if(bola->posBola.y>325 || bola->posBola.x<10){
        bola->velocidadeAtual.y - bola->velocidadeAtual.y;
    }

    
}

void Passe(Bola * bola, Jogador * jogador, Jogo * jogo, Jogador ** jogadorControladoTime1,Jogador ** jogadorControladoTime2) {
    if((jogador->temDominio && jogador->time ==1 && IsKeyDown(KEY_SEMICOLON)) || (jogador->temDominio && jogador->time ==2 && IsKeyDown(KEY_C)) || IsGamepadButtonPressed(0,GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || IsGamepadButtonDown(1,GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
        if (bola->ladoBola == 0) {
            bola->velocidadeAtual.x += 10.0f;
        }
        else if (bola->ladoBola == 1) {
            bola->velocidadeAtual.x -= 10.0f;
        }
        else if (bola->ladoBola == 2) {
            bola->velocidadeAtual.y -= 10.0f;
        }
        else if (bola->ladoBola == 3) {
            bola->velocidadeAtual.y += 10.0f;
        }
        else if(bola->ladoBola == 4){
            bola->velocidadeAtual.x -= 6.0f;
            bola->velocidadeAtual.y -= 6.0f;
        }
        else if(bola->ladoBola == 5){
            bola->velocidadeAtual.x += 6.0f;
            bola->velocidadeAtual.y -= 12;
        }
        else if(bola->ladoBola ==  6){
            bola->velocidadeAtual.x -= 6.0f;
            bola->velocidadeAtual.y += 6.0f;
        }
        else if(bola->ladoBola == 7){
            bola->velocidadeAtual.x += 6.0f;
            bola->velocidadeAtual.y +=6.0f;
        }
        jogador->temDominio = 0;
        jogo->timeComBola = 0;

    }
}

void Chutar(Bola* bola, Jogador* jogador, Jogo * jogo) {
    if ((jogador->temDominio && jogador->time== 1 && IsKeyPressed(KEY_PERIOD)) ||
        (jogador->temDominio && jogador->time== 2 && IsKeyDown(KEY_LEFT_CONTROL)) ||
        IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) ||
        IsGamepadButtonPressed(1, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) {

        float mult = 1.0f;

        if (jogador->time == 1 && jogo->superChuteTimerTime1 > 0.0f) {
            mult = 2.0f;   
        } else if (jogador->time == 2 && jogo->superChuteTimerTime2 > 0.0f) {
            mult = 2.0f;   
        }

        if (bola->ladoBola == 0) {
            bola->velocidadeAtual.x += 20.0f * mult;
        }
        else if (bola->ladoBola == 1) {
            bola->velocidadeAtual.x -= 20.0f * mult;
        }
        else if (bola->ladoBola == 2) {
            bola->velocidadeAtual.y -= 20.0f * mult;
        }
        else if (bola->ladoBola == 3) {
            bola->velocidadeAtual.y += 20.0f * mult;
        }
        else if (bola->ladoBola == 4) {
            bola->velocidadeAtual.x -= 12.0f * mult;
            bola->velocidadeAtual.y -= 12.0f * mult;
        }
        else if (bola->ladoBola == 5) {
            bola->velocidadeAtual.x += 12.0f * mult;
            bola->velocidadeAtual.y -= 12.0f * mult;
        }
        else if (bola->ladoBola == 6) {
            bola->velocidadeAtual.x -= 12.0f * mult;
            bola->velocidadeAtual.y += 12.0f * mult;
        }
        else if (bola->ladoBola == 7) {
            bola->velocidadeAtual.x += 12.0f * mult;
            bola->velocidadeAtual.y += 12.0f * mult;
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
            while((!IsKeyPressed(KEY_RIGHT_SHIFT) && !IsGamepadButtonPressed(0,GAMEPAD_BUTTON_RIGHT_TRIGGER_1) ) && (*jogador1)->temDominio == 0);
            if((*jogador1)->temDominio == 0){
                pthread_mutex_lock(&lock);
                *jogador1 = (*jogador1)->prox;
                mudou =1;
                pthread_mutex_unlock(&lock);
            }
        }
        else if((*jogador1)->temDominio == 0 && (*jogador1)->time == 2){
            while((!IsKeyPressed(KEY_LEFT_SHIFT)&& !IsGamepadButtonPressed(1,GAMEPAD_BUTTON_RIGHT_TRIGGER_1) ) && (*jogador1)->temDominio == 0 );
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

void TratamentoColisaoJogadorBola(Jogador * jogadorControladoTime1,Jogador * jogadorControladoTime2, Jogador * goleiro1, Jogador * goleiro2,Bola * bola, Jogador *head1, Jogador * tail1,Jogador * head2, Jogador * tail2, Jogo * jogo){
    
    if(CheckCollisionCircleRec(bola->posBola,bola->raioBola,goleiro1->rectJogador)){
        bola->velocidadeAtual.x = -bola->velocidadeAtual.x;
        MudarPosicaoBola(bola);
    }
    if(CheckCollisionCircleRec(bola->posBola,bola->raioBola,goleiro2->rectJogador)){
        bola->velocidadeAtual.x = -bola->velocidadeAtual.x;
        MudarPosicaoBola(bola);
    }
    
    if(jogadorControladoTime1->temDominio == 1){
        do{
            if(CheckCollisionCircleRec(bola->posBola,bola->raioBola,head2->rectJogador)){
                if(head2->posJogador.x>bola->posBola.x){
                    bola->velocidadeAtual.x += 8;
                }
                else if(head2->posJogador.x<bola->posBola.x){
                    bola->velocidadeAtual.x -= 8;
                }
                if(head2->posJogador.y>bola->posBola.y){
                    bola->velocidadeAtual.y -= 8;
                }
                else if(head2->posJogador.y < bola->posBola.y){
                    bola->velocidadeAtual.y +=8;
                }
                MudarPosicaoBola(bola);
                jogadorControladoTime1->temDominio = 0;
                jogo->timeComBola = 0;
            }
            head2=head2->prox;
        }while(head2!=tail2->prox);
        
    }
    else if(jogadorControladoTime2->temDominio == 1){
        do{
            if(CheckCollisionCircleRec(bola->posBola,bola->raioBola,head1->rectJogador)){
                if(head1->posJogador.x>bola->posBola.x){
                    bola->velocidadeAtual.x += 8;
                }
                else if(head1->posJogador.x<bola->posBola.x){
                    bola->velocidadeAtual.x -= 8;
                }
                if(head1->posJogador.y>bola->posBola.y){
                    bola->velocidadeAtual.y -= 8;
                }
                else if(head1->posJogador.y < bola->posBola.y){
                    bola->velocidadeAtual.y +=8;
                }
                MudarPosicaoBola(bola);
                jogadorControladoTime2->temDominio = 0;
                jogo->timeComBola = 0;
            }
            head1 = head1->prox;
        }while(head1!=tail1->prox);

    }
}

void AtualizarCamera(Camera2D * camera, Jogo  * jogo, Jogador * jogadorControladoTime1, Jogador* jogadorControladoTime2,Bola * bola){
    if(jogo->voltandoDoGol == 1){
        camera->target =  jogadorControladoTime1->posJogador;  
    }
    else if(jogo->timeComBola == 0){
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

    if((jogadorControladoTime1->isMovendo && jogadorControladoTime1->temDominio) || (jogadorControladoTime2->isMovendo && jogadorControladoTime2->temDominio) || bola1->velocidadeAtual.x != 0.0f || bola1->velocidadeAtual.y != 0.0f){
        if(contadorFrames<=10){
            DrawTexturePro(bola,src1,dest,origin,0.0f,WHITE);
        }
        else if(contadorFrames<=20){
            DrawTexturePro(bola,src2,dest,origin,0.0f,WHITE);
        }
        else if(contadorFrames<=30){
            DrawTexturePro(bola,src3,dest,origin,0.0f,WHITE);
        }
        else if(contadorFrames<=40){
            DrawTexturePro(bola,src4,dest,origin,0.0f,WHITE);
        }
    }
    else{
        DrawTexturePro(bola,src1,dest,origin,0.0f,WHITE);
    }
    // if((bola1->velocidadeAtual.x == 0.0f && bola1->velocidadeAtual.y == 0.0f && jogadorControladoTime1->temDominio == 0 && jogadorControladoTime2->temDominio == 0) || ((jogadorControladoTime1->temDominio == 1 && jogadorControladoTime1->velocidadeJogador.x == 0.0f && jogadorControladoTime1->velocidadeJogador.y == 0.0f) || (jogadorControladoTime2->temDominio == 1 && jogadorControladoTime2->velocidadeJogador.x == 0.0f && jogadorControladoTime2->velocidadeJogador.y==0.0f))){
        
    // }
}

void desenharTexturaJogador(Texture2D jogador, Bola * bola1, Jogador * jogador1,RectangleSprites ** headSprites, int contadorFramesJogador, Jogo * jogo) {
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
            if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_DOWN) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_LEFT) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_DOWN)) && jogo->voltandoDoGol == 0) faceLeft = true;
        } else { // time 2
            if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_LEFT) || IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_DOWN)) && jogo->voltandoDoGol == 0) faceLeft = true;
        }
        if(jogador1->funcaoDoJogador == 4 && jogador1->time == 2){
            faceLeft = true;
        }
        if(jogador1->funcaoDoJogador == 4 && jogador1->time == 1){
            faceLeft = false;
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
    if(bola->posBola.x>790 ){
        bola->posBola.x = 790;
        bola->velocidadeAtual.x = -bola->velocidadeAtual.x;
    }
    else if(bola->posBola.x<60){
        bola->posBola.x=55;
        bola->velocidadeAtual.x = -bola->velocidadeAtual.x;
    }
    if(bola->posBola.y>325){
        bola->posBola.y = 325;
        bola->velocidadeAtual.y = - bola->velocidadeAtual.y;
    }
    else if(bola->posBola.y<10){
        bola->posBola.y = 10;
        bola->velocidadeAtual.y = - bola->velocidadeAtual.y;
    }

}

void TratarColisoesJogadorParede(Jogador * jogador, Rectangle rectangleParede ,Jogo * jogo){

    Rectangle rectangleLinhaColisao = (Rectangle){jogador->posJogador.x,jogador->posJogador.y+20,jogador->largura,2};

    if(CheckCollisionRecs(rectangleLinhaColisao,rectangleParede)){
        if(jogador->time == 1){
            if((IsKeyDown(KEY_UP) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_UP)) && (rectangleParede.x == jogo->rectangleParedeCima.x && rectangleParede.y == jogo->rectangleParedeCima.y )){
                jogador->velocidadeJogador.y = 0.0f;
            }
            else if((IsKeyDown(KEY_DOWN) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_DOWN)) && (rectangleParede.x == jogo->rectangleParedeBaixo.x && rectangleParede.y == jogo->rectangleParedeBaixo.y )){
                jogador->velocidadeJogador.y = 0.0f;
            } 
            else if((IsKeyDown(KEY_LEFT) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_LEFT)) && ((rectangleParede.x == jogo->rectangleParedeFundoEsq1.x && rectangleParede.y == jogo->rectangleParedeFundoEsq1.y ) || 
            (rectangleParede.x == jogo->rectangleParedeFundoEsq2.x && rectangleParede.y == jogo->rectangleParedeFundoEsq2.y ))){
                jogador->velocidadeJogador.x = 0.0f;
            }
            else if((IsKeyDown(KEY_RIGHT) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) && ((rectangleParede.x == jogo->rectangleParedeFundoDir1.x && rectangleParede.y == jogo->rectangleParedeFundoDir1.y ) || 
            (rectangleParede.x == jogo->rectangleParedeFundoDir2.x && rectangleParede.y == jogo->rectangleParedeFundoDir2.y ))){
                jogador->velocidadeJogador.x = 0.0f;
            }
            else if((IsKeyDown(KEY_LEFT) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_LEFT)) && (rectangleParede.x == jogo->linhaGolFim1.x && rectangleParede.y == jogo->linhaGolFim1.y)){
                jogador->velocidadeJogador.x = 0.0f;
            }
            else if((IsKeyDown(KEY_RIGHT) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) && (rectangleParede.x == jogo->linhaGolFim2.x && rectangleParede.y == jogo->linhaGolFim2.y)){
                jogador->velocidadeJogador.x = 0.0f;
            }
            
        }
        else if (jogador->time == 2){
            if((IsKeyDown(KEY_W) || IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_UP)) && (rectangleParede.x == jogo->rectangleParedeCima.x && rectangleParede.y == jogo->rectangleParedeCima.y )){
                jogador->velocidadeJogador.y = 0.0f;
            }
            else if((IsKeyDown(KEY_S) || IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_DOWN)) && (rectangleParede.x == jogo->rectangleParedeBaixo.x && rectangleParede.y == jogo->rectangleParedeBaixo.y )){
                jogador->velocidadeJogador.y = 0.0f;
            } 
            else if((IsKeyDown(KEY_A) || IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_LEFT)) && ((rectangleParede.x == jogo->rectangleParedeFundoEsq1.x && rectangleParede.y == jogo->rectangleParedeFundoEsq1.y ) || 
            (rectangleParede.x == jogo->rectangleParedeFundoEsq2.x && rectangleParede.y == jogo->rectangleParedeFundoEsq2.y ))){
                jogador->velocidadeJogador.x = 0.0f;
            }
            else if((IsKeyDown(KEY_D) || IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) && ((rectangleParede.x == jogo->rectangleParedeFundoDir1.x && rectangleParede.y == jogo->rectangleParedeFundoDir1.y ) || 
            (rectangleParede.x == jogo->rectangleParedeFundoDir2.x && rectangleParede.y == jogo->rectangleParedeFundoDir2.y ))){
                jogador->velocidadeJogador.x = 0.0f;
            }
            else if((IsKeyDown(KEY_D) || IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) && (rectangleParede.x == jogo->linhaGolFim1.x && rectangleParede.y == jogo->linhaGolFim1.y)){
                jogador->velocidadeJogador.x = 0.0f;
            }
            else if((IsKeyDown(KEY_D) || IsGamepadButtonDown(1,GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) && (rectangleParede.x == jogo->linhaGolFim2.x && rectangleParede.y == jogo->linhaGolFim2.y)){
                jogador->velocidadeJogador.x = 0.0f;
            }
        }

    }

    
}

void tratarGol(Jogo *jogo, Bola *bola,Jogador * jogadorControladoTime1,Jogador * jogadorControladoTime2, Jogador *head1, Jogador *tail1, Jogador *head2, Jogador *tail2)
{

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
            Jogador * aux = head1;
            Jogador * aux2 = head2;
            do{
          
                aux->temDominio = 0;
                aux2->temDominio = 0;

                aux = aux->prox;
                aux2 = aux2->prox;
            }while(aux!=tail1->prox && aux2!=tail2->prox);
            
            bola->velocidadeAtual.x = 0;
            bola->velocidadeAtual.y = 0;
            bola->posBola.x = bola->posInicialBola.x;
            bola->posBola.y = bola->posInicialBola.y;
            jogo->voltandoDoGol = 0;
        }
    }
    else if(CheckCollisionCircleRec(bola->posBola,bola->raioBola,jogo->linhaGol1) && jogo->voltandoDoGol == 0){
        jogo->placarTime2+=1;
        jogadorControladoTime2->gols +=1;
        jogo->voltandoDoGol = 1;

        jogo->tempoMostrarGol = 2.0f;

        Jogador * aux = head1;
        Jogador * aux2 = head2;
        do{
          
          aux->temDominio = 0;
          aux2->temDominio = 0;

          aux = aux->prox;
          aux2 = aux2->prox;
        }while(aux!=tail1->prox && aux2!=tail2->prox);
    
    }
    else if(CheckCollisionCircleRec(bola->posBola,bola->raioBola,jogo->linhaGol2) && jogo->voltandoDoGol == 0){
        jogo->placarTime1+=1;
        jogo->voltandoDoGol = 1;
        jogadorControladoTime1->gols +=1;

        jogo->tempoMostrarGol = 2.0f;

        Jogador * aux = head1;
        Jogador * aux2 = head2;
        do{
          
          aux->temDominio = 0;
          aux2->temDominio = 0;

          aux = aux->prox;
          aux2 = aux2->prox;
        }while(aux!=tail1->prox && aux2!=tail2->prox);
    
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

            head1->rectJogador.x = head1->posJogador.x;
            head1->rectJogador.y = head1->posJogador.y;


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
            head2->rectJogador.x = head2->posJogador.x;
            head2->rectJogador.y = head2->posJogador.y;

            head1 = head1->prox;
            head2 = head2->prox;
        }while(head1!=tail1->prox && head2!=tail2->prox);

    }
}

void movimentoAutomaticoJogo(Jogo * jogo,Bola * bola, Jogador * jogadorControladoTime1, Jogador * jogadorControladoTime2, Jogador * headDaVez,Jogador * tailDaVez){
    if (jogo->voltandoDoGol == 0 && ((headDaVez->time == 1 && jogo->congeladoTimerTime1 == 0.0f) || (headDaVez->time == 2 && jogo->congeladoTimerTime2 == 0.0f))){

        int timeComBola = 0;
        if(jogadorControladoTime1->temDominio == 1) timeComBola = 1;
        else if(jogadorControladoTime2->temDominio == 1) timeComBola = 2;


        do{
            if (headDaVez->funcaoDoJogador == 1 && headDaVez != jogadorControladoTime1 && headDaVez != jogadorControladoTime2){   
                if((timeComBola==0 || (headDaVez->time == 1 && timeComBola == 2) || (headDaVez->time == 2 && timeComBola == 1)) && ((headDaVez->time == 1 && headDaVez->posJogador.x>230)|| (headDaVez->time == 2 && headDaVez->posJogador.x<670)) && headDaVez->posJogador.y<330 && headDaVez->posJogador.y>50){
                    if(headDaVez->posJogador.x>bola->posBola.x){
                        headDaVez->velocidadeJogador.x -=2;
                    }
                    else if(headDaVez->posJogador.x<bola->posBola.x){
                        headDaVez->velocidadeJogador.x +=2;
                    }
                    else{
                        headDaVez->velocidadeJogador.x = 0.0f;
                    }
    
                    if(headDaVez->posJogador.y>bola->posBola.y){
                        headDaVez->velocidadeJogador.y -=2;
                    }
                    else if(headDaVez->posJogador.y<bola->posBola.y){
                        headDaVez->velocidadeJogador.y +=2;
                    }
                    else{
                        headDaVez->velocidadeJogador.y = 0.0f;
                    }
                    
                    mudarPosicaoJogadorVelocidade(headDaVez);
                }
                else if ( ((timeComBola == 1 && headDaVez->time == 1) || (timeComBola == 2 && headDaVez->time == 2) && ((headDaVez->time == 1 && headDaVez->posJogador.x > 320) || (headDaVez->time == 2 && headDaVez->posJogador.x < 670)) && headDaVez->posJogador.y < 330 && headDaVez->posJogador.y > 50)){
                    if(headDaVez->time == 1){
                        headDaVez->velocidadeJogador.x -=2;
                    }
                    else if(headDaVez->time == 2){
                        headDaVez->velocidadeJogador.x+=2;
                    }
                    else{
                        headDaVez->velocidadeJogador.x = 0.0f;
                    }
                    mudarPosicaoJogadorVelocidade(headDaVez);
                }
            }
            else if(headDaVez->funcaoDoJogador == 3 && headDaVez!=jogadorControladoTime1 && headDaVez != jogadorControladoTime2){
                if((headDaVez->time == 1 && timeComBola == 2 && headDaVez->posJogador.x<650) || (headDaVez->time == 2 && timeComBola ==1 && headDaVez->posJogador.x>300)){
                    if(headDaVez->time == 1){
                        headDaVez->velocidadeJogador.x +=2;
                    }
                    else if (headDaVez->time == 2){
                        headDaVez->velocidadeJogador.x -=2;
                    }
                    else{
                        headDaVez->velocidadeJogador.x = 0.0f;
                    }
                    mudarPosicaoJogadorVelocidade(headDaVez);
                }
                if(headDaVez->time == timeComBola){
                    if(headDaVez->time == 1 && headDaVez->posJogador.x>300){
                        headDaVez->velocidadeJogador.x -=2;
                    }
                    else if(headDaVez->time == 2 && headDaVez->posJogador.x<600){
                        headDaVez->velocidadeJogador.x +=2;
                    }  
                    else{
                        headDaVez->velocidadeJogador.x = 0.0f;
                    }
                    mudarPosicaoJogadorVelocidade(headDaVez);
                }
                if((headDaVez->time == 1 && bola->posBola.x>415) || (headDaVez->time == 2 && bola->posBola.x<415)){
                    if(headDaVez->posJogador.y>bola->posBola.y){
                        headDaVez->velocidadeJogador.y -=2;
                    }
                    else if(headDaVez->posJogador.y<bola->posBola.y){
                        headDaVez->velocidadeJogador.y +=2;
                    }
                    else{
                        headDaVez->velocidadeJogador.x = 0.0f;
                    }

                    mudarPosicaoJogadorVelocidade(headDaVez);
                }
            }
            else if(headDaVez->funcaoDoJogador == 2 && headDaVez!=jogadorControladoTime1 && headDaVez!=jogadorControladoTime2){
                if((headDaVez->time == 1 && headDaVez->posJogador.x<700) || (headDaVez->time == 2 && headDaVez->posJogador.x>230)){
                    if(headDaVez->time == 1){
                        headDaVez->velocidadeJogador.x +=2;
                    }
                    else if(headDaVez->time == 2){
                        headDaVez->velocidadeJogador.x -=2;
                    }
                    else{
                        headDaVez->velocidadeJogador.x = 0.0f;

                    }
                    
                    mudarPosicaoJogadorVelocidade(headDaVez);

                }
                else if((headDaVez->time == 1 && bola->posBola.x>650) || (headDaVez->time == 2 && bola->posBola.x<300)){
                    if(headDaVez->posJogador.x>bola->posBola.x){
                        headDaVez->velocidadeJogador.x-=2;
                    }
                    else if(headDaVez->posJogador.x<bola->posBola.x){
                        headDaVez->velocidadeJogador.x +=2;
                    }
                    else{
                        headDaVez->velocidadeJogador.x = 0.0f;
                    }

                    if(headDaVez->posJogador.y>bola->posBola.y){
                        headDaVez->velocidadeJogador.y -=2;
                    }
                    else if(headDaVez->posJogador.y<bola->posBola.y){
                        headDaVez->velocidadeJogador.y +=2;
                    }
                    else{
                        headDaVez->velocidadeJogador.y =0.0f;
                    }

                    mudarPosicaoJogadorVelocidade(headDaVez);
                }

            }

            headDaVez = headDaVez->prox;

        }while(headDaVez!=tailDaVez->prox);
    }
}

void mudarPosicaoJogadorVelocidade(Jogador * jogador){
    if(jogador->posJogador.x>770 || jogador->posJogador.x<60){
        jogador->velocidadeJogador.x = - jogador->velocidadeJogador.x;
    }


    jogador->posJogador.x += jogador->velocidadeJogador.x;
    jogador->posJogador.y += jogador->velocidadeJogador.y;

    if (jogador->velocidadeJogador.x == 0.0f && jogador->velocidadeJogador.y == 0.0f){
        jogador->isMovendo = 0;
    }
    else if(jogador->posJogador.x == jogador->rectJogador.x && jogador->posJogador.y == jogador->rectJogador.y){

        jogador->isMovendo = 0;
    }
    else if(jogador->velocidadeJogador.x != 0.0f || jogador->velocidadeJogador.y != 0.0f){
        jogador->isMovendo = 1;
    }

    jogador->rectJogador.x = jogador->posJogador.x;
    jogador->rectJogador.y = jogador->posJogador.y;
    jogador->velocidadeJogador.x = 0.0f;
    jogador->velocidadeJogador.y = 0.0f;
}
void movimentarGoleiro(Jogador * goleiro, Jogo * jogo, Bola * bola){
    if(goleiro->posJogador.y>bola->posBola.y && (goleiro->posJogador.y>jogo->linhaGol1.y)){
        if( goleiro->time == 1 && bola->posBola.x>650){
            goleiro->posJogador.y -=2;
        }
        else{
            goleiro->posJogador.y -=1;
        }
        if(goleiro->posJogador.y != goleiro->rectJogador.y){
            goleiro->isMovendo = 1;
        }
        goleiro->rectJogador.y = goleiro->posJogador.y;
    }
    else if(goleiro->posJogador.y<bola->posBola.y && (goleiro->posJogador.y<jogo->linhaGol1.y + jogo->linhaGol1.width)){
        if( goleiro->time == 1 && bola->posBola.x>650){
            goleiro->posJogador.y +=2;
            goleiro->isMovendo = 1;
        }
        else{
            goleiro->posJogador.y +=1;
            goleiro->isMovendo = 1;
        }
        if(goleiro->posJogador.y != goleiro->rectJogador.y){
            goleiro->isMovendo = 1;
        }
        goleiro->rectJogador.y = goleiro->posJogador.y;
    }


    if(goleiro->posJogador.y<bola->posBola.y && (goleiro->posJogador.y>jogo->linhaGol2.y + jogo->linhaGol2.height)){
        if( goleiro->time == 2 && bola->posBola.x<120){
            goleiro->posJogador.y +=2;
        }
        else{
            goleiro->posJogador.y +=1;
        }
        if(goleiro->posJogador.y != goleiro->rectJogador.y){
            goleiro->isMovendo = 1;
        }
        goleiro->rectJogador.y = goleiro->posJogador.y;
    }
    if(goleiro->posJogador.y>bola->posBola.y && (goleiro->posJogador.y>jogo->linhaGol2.y + jogo->linhaGol2.height)){
        if( goleiro->time == 2 && bola->posBola.x<120){
            goleiro->posJogador.y -=2;
        }
        else{
            goleiro->posJogador.y -=1;
        }
        if(goleiro->posJogador.y != goleiro->rectJogador.y){
            goleiro->isMovendo = 1;
        }
        goleiro->rectJogador.y = goleiro->posJogador.y;
    }

}

void ordernarPorGols(Jogador *head1, Jogador *tail1)
{
    Jogador *aux1 = head1;

    Jogador *aux = head1;
    do
    {
       Jogador * aux = head1;

        while (aux != tail1)
        {
            if (aux->gols < aux->prox->gols)
            {
                int temp = aux->gols;
                int temp1 = aux->funcaoDoJogador;

                aux->gols = aux->prox->gols;
                aux->funcaoDoJogador = aux->prox->funcaoDoJogador;

                aux->prox->gols = temp;
                aux->prox->funcaoDoJogador = temp1;
            }
            aux = aux->prox;
        }
        head1 = head1->prox;
    } while (aux != tail1->prox);
}

void DesenharPlacarHUD(const Jogo *jogo) {
    int sw = GetScreenWidth();

    int boxW = 280;
    int boxH = 70;
    int x = sw/2 - boxW/2;
    int y = 10;

    DrawRectangleRounded(
        (Rectangle){ x, y, boxW, boxH },
        0.3f, 8,
        (Color){ 10, 10, 20, 230 }
    );

    DrawRectangleRounded(
        (Rectangle){ x, y, boxW/2, boxH },
        0.3f, 8,
        (Color){ 0, 120, 255, 180 }  
    );


    DrawRectangleRounded(
        (Rectangle){ x + boxW/2, y, boxW/2, boxH },
        0.3f, 8,
        (Color){ 220, 30, 60, 180 }   
    );

    int fontLabel = 18;
    const char *lbl1 = "P2";
    const char *lbl2 = "P1";
    int lbl1W = MeasureText(lbl1, fontLabel);
    int lbl2W = MeasureText(lbl2, fontLabel);

    DrawText(lbl1,
             x + boxW/4 - lbl1W/2,
             y + 8,
             fontLabel,
             WHITE);

    DrawText(lbl2,
             x + (3*boxW)/4 - lbl2W/2,
             y + 8,
             fontLabel,
             WHITE);

    char txt1[8];
    char txt2[8];
    sprintf(txt1, "%d", jogo->placarTime1);
    sprintf(txt2, "%d", jogo->placarTime2);

    int fontScore = 32;
    int t1W = MeasureText(txt1, fontScore);
    int t2W = MeasureText(txt2, fontScore);

    DrawText(txt1,
             x + boxW/4 - t1W/2,
             y + 30,
             fontScore,
             WHITE);

    DrawText(txt2,
             x + (3*boxW)/4 - t2W/2,
             y + 30,
             fontScore,
             WHITE);
}

