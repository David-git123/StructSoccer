#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include <stdbool.h>
#include <math.h>
#include "../include/structsoccer.h"

#ifdef __cplusplus
extern "C" {
#endif

void UpdateAndDrawMainMenu(Menu* menu, EstadoDoJogo* estado, Texture2D bg, Texture2D titleTex, float dt);

#ifdef __cplusplus
}
#endif

#endif // MENU_H
