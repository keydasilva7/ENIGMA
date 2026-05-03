#ifndef TETRIS_H_INCLUDED
#define TETRIS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "GBT/gbt.h"

#define PIXELES_X_LADO 8
#define PX_PADDING 4

#define N 15 // Transparente
#define A 14 // Amarillo
#define C 3  // Celeste
#define V 2  // Verde


//Nuestra estructura de pieza (I, J, L, O, S, T, Z)
//Permite mover copiar o colisionar
typedef struct
{
    int tipo;      // 0 a 6 (qué pieza es)
    int x, y;      // posición en el tablero
    int rotacion;  // 0 a 3

} Tetromino;

void dibujar(const uint8_t[][PIXELES_X_LADO], uint16_t oX, uint16_t oY);
void rotar(int [][4], int [][4]);

#endif // TETRIS_H_INCLUDED
