#ifndef STRUCTSOCCER_H
#define STRUCTSOCCER_H

#include "raylib.h"    
#include <pthread.h>    

#ifdef __cplusplus
extern "C" {
#endif

/* ─── Estado do jogo ─────────────────────────────────────────────────────────────── */

typedef enum {
    ST_MENU,
    ST_CLASSICO,   
    ST_ARCADE
} EstadoDoJogo;

/* ─── Dados do menu ─────────────────────────────────────────────────────────────── */

typedef struct {
    const char *itens[2];  
    int cont;             
    int selecionar;         
} Menu;

/* ─── Tipos ─────────────────────────────────────────────────────────────── */
//Função do jogador: 1 = Atacante. 2 = Defensor.

typedef struct Jogador {
    float   forcaChute;
    float   largura;
    float   altura;
    int     temDominio;
    int     time;           
    int     isMovendo;
    struct Jogador* prox;  
    Vector2 velocidadeJogador;
    int  funcaoDoJogador;
    Vector2 posJogador;
    Vector2 posJogadorInicial;
    Rectangle rectJogador;
} Jogador;

typedef struct RectangleSprites {
    Rectangle Rectangle;
    struct RectangleSprites* prox;
} RectangleSprites;

typedef struct Bola {
    float   raioBola;
    int     ladoBola;      
    Vector2 velocidadeAtual;
    Vector2 posInicialBola;
    Vector2 posBola;
} Bola;

typedef struct Jogo {
    int       timeComBola; 
    int       placarTime1;
    int       placarTime2;
    int   tempoInicial;    
    float tempoRestante; 
    int   jogoPausado;
    int voltandoDoGol;
    Rectangle rectangleParedeCima;
    Rectangle rectangleParedeBaixo;
    Rectangle rectangleParedeFundoDir1;
    Rectangle rectangleParedeFundoDir2;
    Rectangle rectangleParedeFundoEsq1;
    Rectangle rectangleParedeFundoEsq2;
    Rectangle linhaGolFim1;
    Rectangle linhaGolFim2;
    Rectangle linhaGol1;
    Rectangle linhaGol2;
    Rectangle rectangleGol1;
    Rectangle rectangleGol2;
} Jogo;

/* ─── Variáveis globais (somente declaração) ───────────────────────────── */

extern pthread_mutex_t lock;

/* ─── Funções (protótipos) ─────────────────────────────────────────────── */

void* DefinirJogadorControlado(void* jogadorAtual);
void   AtualizarPosJogador(Jogador* jogador, Jogador* head1, Jogador* head2,Jogo * jogo);
void   EstadoBola(Bola* bola, Jogador* jogador, Jogador * jogadorControladoTime1, Jogador * jogadorControladoTime2, Jogador* head1, Jogador * tail1, Jogador* head2, Jogador * tail2, Jogo* jogo);
void   Atrito(Bola* bola);
void Passe(Bola * bola, Jogador * jogador, Jogo * jogo, Jogador ** jogadorControladoTime1,Jogador ** jogadorControladoTime2);
void   MudarPosicaoBola(Bola* bola);
void   Chutar(Bola* bola, Jogador* jogador, Jogo* jogo);
void   TratamentoColisaoJogadorBola(Jogador* jogadorControladoTime1,Jogador * jogadorControladoTime2, Bola* bola, Jogador* head1,Jogador * tail1, Jogador* head2, Jogador * tail2, Jogo* jogo);
void   AtualizarCamera(Camera2D* camera, Jogo* jogo, Jogador* jogadorControladoTime1, Jogador* jogadorControladoTime2, Bola* bola);
void   desenharTexturaBola(Texture2D bola, Bola* bola1, int contadorFrames, Jogador* jogadorControladoTime1, Jogador* jogadorControladoTime2);
void   desenharTexturaJogador(Texture2D jogador, Bola* bola1, Jogador* jogador1, RectangleSprites** headSprites, int contadorFramesJogador, Jogo * jogo);
void TratarColisoesParedeBola(Bola * bola, Rectangle rectangleParede, Jogo * jogo);
void TratarColisoesJogadorParede(Jogador * jogador, Rectangle rectangleParede ,Jogo * jogo);
void tratarGol(Jogo * jogo, Bola * bola, Jogador * head1,Jogador * tail1, Jogador *head2, Jogador * tail2);
void movimentoAutomatico(Jogo * jogo, Jogador * head1, Jogador * tail1,Jogador * head2, Jogador * tail2);
void movimentoAutomaticoJogo(Jogo * jogo,Bola * bola, Jogador * jogadorControladoTime1, Jogador * jogadorControladoTime2, Jogador * head1,Jogador * tail1, Jogador * head2,Jogador * tail2);
void movimentarGoleiro(Jogador * goleiro, Jogo * jogo, Bola * bola);

#ifdef __cplusplus
}
#endif
#endif
