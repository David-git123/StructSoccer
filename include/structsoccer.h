#ifndef STRUCTSOCCER_H
#define STRUCTSOCCER_H
/* Interface pública do jogo */

#include "raylib.h"     // Vector2, Rectangle, Texture2D, Camera2D
#include <pthread.h>    // pthread_mutex_t

#ifdef __cplusplus
extern "C" {
#endif

/* ─── Tipos ─────────────────────────────────────────────────────────────── */

typedef struct Jogador {
    float   forcaChute;
    float   largura;
    float   altura;
    int     temDominio;
    int     time;           /* 1 ou 2 */
    int     isMovendo;
    struct Jogador* prox;   /* lista circular por time */
    Vector2 velocidadeJogador;
    Vector2 posJogador;
    Rectangle rectJogador;
} Jogador;

typedef struct RectangleSprites {
    Rectangle Rectangle;
    struct RectangleSprites* prox; /* lista circular de frames */
} RectangleSprites;

typedef struct Bola {
    float   raioBola;
    int     ladoBola;       /* 0 dir, 1 esq, 2 cima, 3 baixo */
    Vector2 velocidadeAtual;
    Vector2 posBola;
} Bola;

typedef struct Jogo {
    int       timeComBola;  /* 0 sem dono, 1 T1, 2 T2 */
    int       placarTime1;
    int       placarTime2;
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
#endif /* STRUCTSOCCER_H */
