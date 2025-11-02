#ifndef TEMPO_H
#define TEMPO_H

#include <math.h>
#include "raylib.h" 
#include "structsoccer.h" 

// Reinicia o cronômetro para "segundos"
static inline void ReiniciarCronometro(Jogo *jogo, int segundos) {
    jogo->tempoInicial  = segundos;
    jogo->tempoRestante = (float)segundos;
    jogo->jogoPausado   = 0;
}

// Atualiza o cronômetro (GetFrameTime())
static inline void AtualizarCronometro(Jogo *jogo, float dt) {
    if (!jogo->jogoPausado && jogo->tempoRestante > 0.0f) {
        jogo->tempoRestante -= dt;
        if (jogo->tempoRestante < 0.0f) jogo->tempoRestante = 0.0f;
    }
}

// Alterna pausa/rodando
static inline void AlternarPausa(Jogo *jogo) {
    jogo->jogoPausado = !jogo->jogoPausado;
}

// Desenha "MM:SS" e uma etiqueta [PAUSADO] se estiver pausado
static inline void DesenharCronometroHUD(const Jogo *jogo, int x, int y) {
    int total = (int)ceilf(jogo->tempoRestante);
    if (total < 0) total = 0;
    int mm = total / 60;
    int ss = total % 60;

    DrawText(TextFormat("%02d:%02d", mm, ss), x, y, 40, YELLOW);
    if (jogo->jogoPausado) {
        DrawText("[PAUSADO]", x, y + 42, 20, LIGHTGRAY);
    }
}

#endif 
