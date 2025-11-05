// include/modes.h
#pragma once
#include "raylib.h"
#include "../include/structsoccer.h"

typedef struct GameCtx {
    Texture2D campo;
    Texture2D jogadorTex;
    Texture2D bolaTex;
    Texture2D paredeFundoCampo;
    Texture2D paredeLadoCima;
    Texture2D paredeLadoBaixo;
    Texture2D barra;
    Texture2D barraTopo;

    Rectangle srcParedeFundoCampoDir;
    Rectangle destParedeFundoCampoDir2;
    Rectangle srcBarraEsquerda;
    Rectangle destBarraEsquerda;

    Jogo* jogo;
    Bola* bola1;
    Camera2D* camera;
    Color corVerdeGrama;

    Jogador* j1;
    Jogador* j2;
    Jogador* j3;
    Jogador* j4;

    Jogador * head1;
    Jogador * head2;

    Jogador * tail1;
    Jogador * tail2;
    
    Jogador** ctrl1;
    Jogador** ctrl2;

    RectangleSprites** headSprites;

} GameCtx;

#ifdef __cplusplus
extern "C" {
#endif

void RunModoClassico(GameCtx* ctx);
void RunModoPowerUps(GameCtx* ctx);

#ifdef __cplusplus
}
#endif
