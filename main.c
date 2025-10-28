#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>



//Controles: JOGADOR 1: UP, DOWN , LEFT RIGTH. SHIT DIREITO (TROCA E CHUTE). 0 PASSE 
//Controles: JOGADOR 2: W, S , A ,D. SHIT ESQUERDO (TROCA E CHUTE). C PASSE

typedef struct Jogador {
    float forcaChute;
    float largura;
    float altura;
    int temDominio;
    int time;
    int isMovendo;
    struct Jogador* prox;
    Vector2 velocidadeJogador;
    Vector2 posJogador;
    Rectangle rectJogador;
}Jogador;

typedef struct RectangleSprites{
    Rectangle Rectangle;
    struct RectangleSprites * prox;
}RectangleSprites;

typedef struct Bola {
    float raioBola;
    int ladoBola;
    Vector2 velocidadeAtual;
    Vector2 posBola;
}Bola;

typedef struct Jogo{
    int timeComBola;
    int placarTime1;
    int placarTime2;
    Rectangle rectangleGol1;
    Rectangle rectangleGol2;
}Jogo;

void * DefinirJogadorControlado(void * jogadorAtual);
void AtualizarPosJogador(Jogador * jogador, Jogador * head1, Jogador * head2);
void EstadoBola(Bola * bola, Jogador * jogador,Jogador * head1, Jogador * head2, Jogo * jogo);
void Atrito(Bola * bola);
void Passe(Bola * bola,Jogador * jogador,Jogo * jogo);
void MudarPosicaoBola(Bola * bola);
void Chutar(Bola* bola, Jogador* jogador,Jogo * jogo);
void TratamentoColisaoJogadorBola(Jogador * jogador, Bola * bola, Jogador *head1, Jogador * head2,Jogo * jogo);
void TratamentoColisaoJogadorJogador(Jogador * head1, Jogador * head);
void AtualizarCamera(Camera2D * camera, Jogo  * jogo, Jogador * jogadorControladoTime1, Jogador* jogadorControladoTime2,Bola * bola);
void desenharTexturaBola(Texture2D bola, Bola * bola1, int contadorFrames, Jogador * jogadorControladoTime1, Jogador * jogadorControladoTime2);
void desenharTexturaJogador(Texture2D jogador, Bola * bola1, Jogador * jogador1, RectangleSprites ** headSprites, int contador);



pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


void main() {


    Jogo * jogo = (Jogo *)malloc(sizeof(Jogo));
    jogo->timeComBola =0;
    jogo->placarTime1 =0;
    jogo->placarTime2 =0;
    jogo->rectangleGol1.x= 1720.0f;
    jogo->rectangleGol1.y = 540.0f;
    jogo->rectangleGol1.height = 300.0f;
    jogo->rectangleGol1.width = 200.0f;

    jogo->rectangleGol2.x= 0;
    jogo->rectangleGol2.y = 540.0f;
    jogo->rectangleGol2.height = 300.0f;
    jogo->rectangleGol2.width = 200.0f;


    Jogador* head1Jogador = NULL;
    Jogador* tail1Jogador = NULL;

    Jogador* head2Jogador = NULL;
    Jogador* tail2Jogador = NULL;

    Jogador* jogador1 = (Jogador*)malloc(sizeof(Jogador));
    Jogador* jogador2 = (Jogador*)malloc(sizeof(Jogador));
    Jogador* jogador3 = (Jogador*)malloc(sizeof(Jogador));
    Jogador* jogador4 = (Jogador*)malloc(sizeof(Jogador));
    
    
    head1Jogador = jogador1;
    jogador2->prox = head1Jogador;
    tail1Jogador = jogador2;
    Rectangle src1 = {0,0,24,32};
    jogador1->prox = jogador2;  
    
    head2Jogador = jogador3;
    jogador3->prox = jogador4;
    jogador4->prox = jogador3;
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
        jogador1->posJogador = (Vector2){ 500,180};
        jogador1->largura = 24;
        jogador1->altura = 30;
        jogador1->velocidadeJogador = (Vector2){0.0f,0.0f};
        jogador1->rectJogador = (Rectangle){ jogador1->posJogador.x, jogador1->posJogador.y,jogador1->largura,jogador1->altura };
        jogador1->time =1;
    }
    if (jogador2) {
        jogador2->temDominio = 0;
        jogador2->forcaChute = 10.0f;
        jogador2->posJogador = (Vector2){ 200,500.0f };
        jogador2->velocidadeJogador = (Vector2){0.0f,0.0f};
        jogador2->largura = 20.0f;
        jogador2->altura = 20.0f;
        jogador2->rectJogador = (Rectangle){ jogador2->posJogador.x, jogador2->posJogador.y,jogador2->largura,jogador2->altura };
        jogador2->time =1;
    }

    if (jogador3) {
        jogador3->temDominio = 0;
        jogador3->forcaChute = 10.0f;
        jogador3->posJogador = (Vector2){ 1000.0f,500.0f };
        jogador3->velocidadeJogador = (Vector2){0.0f,0.0f};
        jogador3->largura = 20.0f;
        jogador3->altura = 20.0f;
        jogador3->rectJogador = (Rectangle){ jogador3->posJogador.x, jogador3->posJogador.y,jogador3->largura,jogador3->altura };
        jogador3->time =2;
    }
    if (jogador4) {
        jogador4->temDominio = 0;
        jogador4->forcaChute = 10.0f;
        jogador4->posJogador = (Vector2){ 1200,500.0f };
        jogador4->velocidadeJogador = (Vector2){0.0f,0.0f};
        jogador4->largura = 20.0f;
        jogador4->altura = 20.0f;
        jogador4->rectJogador = (Rectangle){ jogador4->posJogador.x, jogador4->posJogador.y,jogador4->largura,jogador4->altura };
        jogador4->time =2;
    }

 
    
    Jogador* jogadorControladoTime1 = jogador1;
    Jogador* jogadorControladoTime2 = jogador3;
    
    
    pthread_t threadChecarControlado;

    
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    Color corVerdeGrama;
    corVerdeGrama.r = 124;
    corVerdeGrama.g = 252;
    corVerdeGrama.b = 0;
    corVerdeGrama.a = 1;
    
    InitWindow(screenWidth, screenHeight, "Fut");

    Texture2D campo = LoadTexture("soccer-course-assets-main/assets/art/backgrounds/pitch-lines.png");
    Texture2D jogador = LoadTexture("soccer-course-assets-main/assets/art/characters/soccer-player.png");
    Texture2D bola = LoadTexture("soccer-course-assets-main/assets/art/props/soccer-ball.png");

    
    Bola* bola1 = (Bola*)malloc(sizeof(Bola));
    if (bola1) {
        bola1->posBola = (Vector2){ 425,180 };
        bola1->raioBola = 5;
        bola1->velocidadeAtual = (Vector2){ 0.0f,0.0f };
    }

    Camera2D *camera = (Camera2D *)malloc(sizeof(Camera2D));
    camera->offset = (Vector2){screenWidth/2, screenHeight/2};
    camera->rotation = 0.0f;
    camera->zoom =  3.5f;

    pthread_create(&threadChecarControlado,NULL,DefinirJogadorControlado,&jogadorControladoTime1);
    pthread_create(&threadChecarControlado,NULL,DefinirJogadorControlado,&jogadorControladoTime2);
    
    //contadores de frames:
    int contFramesBola = 0;
    int contadorFramesJogador =0;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        //reset dos contadores de frames
        if(contFramesBola == 60){
            contFramesBola = 0;
        }
        if (contadorFramesJogador == 60){
            contadorFramesJogador =0;
        }

        pthread_mutex_lock(&lock);
        AtualizarPosJogador(jogadorControladoTime1,head1Jogador,head2Jogador);
        AtualizarPosJogador(jogadorControladoTime2,head1Jogador, head2Jogador);
        EstadoBola(bola1,jogadorControladoTime1,head1Jogador,head2Jogador, jogo);
        EstadoBola(bola1,jogadorControladoTime2,head1Jogador,head2Jogador, jogo);
        if(jogo->timeComBola == 1 || jogo->timeComBola == 0){
            Passe(bola1,jogadorControladoTime1, jogo);
            Chutar(bola1, jogadorControladoTime1, jogo);   
        }
        else if(jogo->timeComBola == 2 || jogo->timeComBola == 0){
            Passe(bola1,jogadorControladoTime2, jogo);
            Chutar(bola1, jogadorControladoTime2, jogo); 
        }
        pthread_mutex_unlock(&lock);
        Atrito(bola1);
        MudarPosicaoBola(bola1);

    
        AtualizarCamera(camera,jogo,jogadorControladoTime1,jogadorControladoTime2,bola1);
        
        BeginDrawing();
            ClearBackground(corVerdeGrama);
            BeginMode2D(*camera);
                DrawTexture(campo,0,0,WHITE);
                // DrawRectangle(jogador1->posJogador.x, jogador1->posJogador.y, jogador1->rectJogador.width, jogador1->rectJogador.height, RED);
                // DrawRectangle(jogador2->posJogador.x, jogador2->posJogador.y, jogador2->rectJogador.width, jogador2->rectJogador.height, RED);
                // DrawRectangle(jogador3->posJogador.x, jogador3->posJogador.y, jogador3->rectJogador.width, jogador3->rectJogador.height, YELLOW);
                // DrawRectangle(jogador4->posJogador.x, jogador4->posJogador.y, jogador4->rectJogador.width, jogador4->rectJogador.height, YELLOW);
                desenharTexturaJogador(jogador,bola1,jogador1,&headSpritesJogador,contadorFramesJogador);
                DrawRectangleLines(jogador1->posJogador.x,jogador1->posJogador.y,jogador1->rectJogador.width,jogador1->rectJogador.height,WHITE);
                desenharTexturaBola(bola,bola1,contFramesBola,jogadorControladoTime1,jogadorControladoTime2);
                DrawCircleLines(bola1->posBola.x,bola1->posBola.y,bola1->raioBola,WHITE);
                // DrawCircleV(bola1->posBola, bola1->raioBola, BLUE);
                // DrawCircleV(jogadorControladoTime1->posJogador, 5.0f,GREEN);
            EndMode2D();
        EndDrawing();
        //Incremento dos contadores de frames
        contFramesBola++;
        contadorFramesJogador++;
    }
    CloseWindow();




}

void AtualizarPosJogador(Jogador * jogador, Jogador * head1 , Jogador * head2) {
    if(jogador->time == 1){
        if (IsKeyDown(KEY_RIGHT)) jogador->velocidadeJogador.x = 5.0f;
        if (IsKeyDown(KEY_LEFT)) jogador->velocidadeJogador.x = -5.0f;
        if (IsKeyDown(KEY_UP)) jogador->velocidadeJogador.y = -5.0f;
        if (IsKeyDown(KEY_DOWN)) jogador->velocidadeJogador.y = 5.0f;
    }else if(jogador->time==2){
        if (IsKeyDown(KEY_D)) jogador->velocidadeJogador.x = 5.0f;
        if (IsKeyDown(KEY_A)) jogador->velocidadeJogador.x = -5.0f;
        if (IsKeyDown(KEY_W)) jogador->velocidadeJogador.y = -5.0f;
        if (IsKeyDown(KEY_S)) jogador->velocidadeJogador.y = 5.0f;
    }

    jogador->posJogador.x += jogador->velocidadeJogador.x;
    jogador->posJogador.y += jogador->velocidadeJogador.y;
    if(jogador->velocidadeJogador.x != 0.0f || jogador->velocidadeJogador.y != 0.0f){
        jogador->isMovendo =1;
    }
    else{
        jogador->isMovendo = 0;
    }
    jogador->rectJogador.x = jogador->posJogador.x;
    jogador->rectJogador.y = jogador->posJogador.y;

    jogador->velocidadeJogador.x = 0;
    jogador->velocidadeJogador.y = 0;
}

void EstadoBola(Bola * bola, Jogador * jogador,Jogador * head1, Jogador * head2, Jogo * jogo) {
    if (!jogador->temDominio) {
        if (CheckCollisionCircleRec(bola->posBola, bola->raioBola, jogador->rectJogador)) {
            jogador->temDominio = 1; 
            bola->velocidadeAtual.x = 0.0f;
            bola->velocidadeAtual.y = 0.0f;
            jogo->timeComBola = jogador->time;
            // TratamentoColisaoJogadorBola(jogador,bola,head1,head2, jogo);
        }
    }
    else{//Tem dominio sendo true
        if(jogador->time == 1){
            if (IsKeyDown(KEY_UP)) {//Adicionar o + velocidade bola. Diminuir a velocidade da bola quando estiver em movimento;
                bola->posBola.x = jogador->posJogador.x + jogador->rectJogador.width / 2.0f;
                bola->posBola.y = jogador->posJogador.y - bola->raioBola;
                bola->ladoBola = 2;//Cima
            }
            else if (IsKeyDown(KEY_DOWN)) {
                bola->posBola.x = jogador->posJogador.x+ jogador->rectJogador.width / 2.0f;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height + bola->raioBola;
                bola->ladoBola = 3;//Baixo
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
        }
        else if(jogador->time== 2){
            if (IsKeyDown(KEY_W)) {//Adicionar o + velocidade bola. Diminuir a velocidade da bola quando estiver em movimento;
                bola->posBola.x = jogador->posJogador.x + jogador->rectJogador.width / 2.0f;
                bola->posBola.y = jogador->posJogador.y - bola->raioBola;
                bola->ladoBola = 2;//Cima
            }
            else if (IsKeyDown(KEY_S)) {
                bola->posBola.x = jogador->posJogador.x+ jogador->rectJogador.width;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height + bola->raioBola;
                bola->ladoBola = 3;//Baixo
            }   
            else if (IsKeyDown(KEY_D)) {
                bola->posBola.x = jogador->posJogador.x + jogador->rectJogador.width + bola->raioBola;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height;
                bola->ladoBola = 0;//Direita
            }
            else if (IsKeyDown(KEY_A)) {
                bola->posBola.x = jogador->posJogador.x - bola->raioBola;
                bola->posBola.y = jogador->posJogador.y + jogador->rectJogador.height / 2.0f;
                bola->ladoBola = 1;//Esquerda
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
            while(IsKeyUp(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT));
            pthread_mutex_lock(&lock);
            *jogador1 = (*jogador1)->prox;
            mudou =1;
            pthread_mutex_unlock(&lock);
        }
        else if((*jogador1)->temDominio == 0 && (*jogador1)->time == 2){
            while(IsKeyUp(KEY_LEFT_SHIFT) || IsKeyDown(KEY_LEFT_SHIFT));
            pthread_mutex_lock(&lock);
            *jogador1 = (*jogador1)->prox;
            mudou =1;
            pthread_mutex_unlock(&lock);
        }
        if(mudou){  
            usleep(500000);
        }
    }
    
}

void TratamentoColisaoJogadorBola(Jogador * jogador, Bola * bola, Jogador *head1, Jogador * head2, Jogo * jogo){
    if(jogo->timeComBola != 0 ){
        if(jogador->time == 1){
            do{
                if(head2->temDominio == 1){
                    break;
                }
                head2 = head2->prox;
            }while(head2!=head2);
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
            }while(head1!=head1);
            head1->temDominio = 0;
            jogador->temDominio = 1;
            jogo->timeComBola = 2;
        }
    }
}

void TratamentoColisaoJogadorJogador(Jogador * head1, Jogador * head2){
    do{
        do{
            if(CheckCollisionRecs(head1->rectJogador,head2->rectJogador)){
                head1->velocidadeJogador.x = 0.0f;
                head1->velocidadeJogador.y = 0.0f;
                head2->velocidadeJogador.x = 0.0f;
                head2->velocidadeJogador.y = 0.0f;
            }
            head2 = head2->prox;
        }while(head2!=head2);
        head1 = head1->prox;
    }while(head1!=head1);
 
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

void desenharTexturaJogador(Texture2D jogador, Bola * bola1, Jogador * jogador1, RectangleSprites ** headSprites, int contadorFramesJogador){
    int mudouSprites = 0;
    Vector2 origin = {0,0};
    Rectangle dest = {jogador1->posJogador.x,jogador1->posJogador.y,24,32};
    Rectangle src1 = {0,0,24,32};
    
    Rectangle srcDaVez;
    if (jogador1->isMovendo == 0){
        srcDaVez = src1;
    }
    else{
        srcDaVez = (*headSprites)->Rectangle;
        if(contadorFramesJogador%10== 0){
          *headSprites = (*headSprites)->prox;
          
        }
    }
    DrawTexturePro(jogador,srcDaVez,dest,origin,0.0f,WHITE);
}