#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include <stdbool.h>
#include <math.h>                 // sinf
#include "../include/structsoccer.h"  // tipos: Menu, EstadoDoJogo

#ifdef __cplusplus
extern "C" {
#endif

// Desenha/atualiza a tela do menu e altera *estado quando a pessoa escolhe algo.
void UpdateAndDrawMainMenu(Menu* menu, EstadoDoJogo* estado, Texture2D campo, Texture2D bola, float dt);

#ifdef __cplusplus
}
#endif

#endif // MENU_H
