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

typedef struct Jogador {
    float   forcaChute;
    float   largura;
    float   altura;
    int     temDominio;
    int     time;           
    int     isMovendo;
    struct Jogador* prox;  
    Vector2 velocidadeJogador;
    Vector2 posJogador;
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
    Vector2 posBola;
} Bola;

typedef struct Jogo {
    int       timeComBola; 
    int       placarTime1;
    int       placarTime2;
    int   tempoInicial;    
    float tempoRestante; 
    int   jogoPausado; 
    Rectangle rectangleParedeCima;
    Rectangle rectangleGol1;
    Rectangle rectangleGol2;
} Jogo;

/* ─── Variáveis globais (somente declaração) ───────────────────────────── */

extern pthread_mutex_t lock;

/* ─── Funções (protótipos) ─────────────────────────────────────────────── */

void* DefinirJogadorControlado(void* jogadorAtual);
void   AtualizarPosJogador(Jogador* jogador, Jogador* head1, Jogador* head2);
void   EstadoBola(Bola* bola, Jogador* jogador, Jogador* head1, Jogador* head2, Jogo* jogo);
void   Atrito(Bola* bola);
void   Passe(Bola* bola, Jogador* jogador, Jogo* jogo);
void   MudarPosicaoBola(Bola* bola);
void   Chutar(Bola* bola, Jogador* jogador, Jogo* jogo);
void   TratamentoColisaoJogadorBola(Jogador* jogador, Bola* bola, Jogador* head1, Jogador* head2, Jogo* jogo);
void   TratamentoColisaoJogadorJogador(Jogador* head1, Jogador* head2);
void   AtualizarCamera(Camera2D* camera, Jogo* jogo, Jogador* jogadorControladoTime1, Jogador* jogadorControladoTime2, Bola* bola);
void   desenharTexturaBola(Texture2D bola, Bola* bola1, int contadorFrames, Jogador* jogadorControladoTime1, Jogador* jogadorControladoTime2);
void   desenharTexturaJogador(Texture2D jogador, Bola* bola1, Jogador* jogador1, RectangleSprites** headSprites, int contadorFramesJogador);

#ifdef __cplusplus
}
#endif
#endif
