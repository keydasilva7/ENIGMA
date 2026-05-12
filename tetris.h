#ifndef TETRIS_H_INCLUDED
#define TETRIS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "GBT/gbt.h"

#define PIXELES_X_LADO 8 //medida de un bloque
#define PX_PADDING 4 //separacion entre bloques

#define N 15 // Transparente
#define A 14 // Amarillo
#define C 3  // Celeste
#define V 2  // Verde

#define FILAS_VISIBLES 20
#define FILAS_OCULTAS 4
#define FILAS_TOTALES 24
#define COLUMNAS 10


//Nuestra estructura de pieza (I, J, L, O, S, T, Z)
//Permite mover copiar o colisionar
typedef struct
{
    int tipo;      // qué pieza es? I=0, J=1, L=2, O=3, S=4, T=5, Z=6
    int x, y;      // es la posición en el tablero, sería como coordenadas en un eje x e y pero en realidad sería x la columna e y la fila
    int forma[4][4];  // es la forma que va a tomar

} Tetromino;

// Estado del juego
typedef struct
{
    int tablero[FILAS_TOTALES][COLUMNAS]; // 0 si esta vacio, color si esta ocupado

    Tetromino pieza_actual;
    Tetromino pieza_siguiente;

    long puntos;
    int lineas;
    int piezas_caidas;

    float velocidad_caida_ms; // milisegundos por cada caida de fila
    int game_over;

} EstadoJuego;

int rotar_pieza_actual(EstadoJuego* estado, int direccion);
void dibujar(const uint8_t dibujo[][PIXELES_X_LADO], uint16_t oX, uint16_t oY);
void inicializar_tablero(EstadoJuego* estado);
void inicializar_estructura(EstadoJuego* estado);
void inicializar_juego(EstadoJuego* estado);
int puede_mover_pieza(EstadoJuego* estado, int dx, int dy);
void mover_pieza(EstadoJuego* estado, int dx, int dy);
void dibujar_pieza(EstadoJuego* estado);
void fijar_pieza(EstadoJuego* estado);
void generar_nueva_pieza(EstadoJuego* estado);
void borrar_lineas_completas(EstadoJuego* estado);




#endif // TETRIS_H_INCLUDED
