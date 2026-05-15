#ifndef TETRIS_H_INCLUDED
#define TETRIS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "GBT/gbt.h"

#define PIXELES_X_LADO 8 //medida de un bloque
#define PX_PADDING 1 //separacion entre bloques

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

extern const int COLORES_PIEZAS[7];

int rotar_pieza_actual(EstadoJuego *, int);
int colision(EstadoJuego *,Tetromino *);
void rotar_matriz(int [][4], int [][4], int );
void inicializar_tablero(EstadoJuego *);
void inicializar_estructura(EstadoJuego *);
void inicializar_juego(EstadoJuego *);
int puede_mover_pieza(EstadoJuego *, int,int);
void mover_pieza(EstadoJuego *,int,int);
void dibujar_pieza(EstadoJuego *);
void fijar_pieza(EstadoJuego *);
void generar_nueva_pieza(EstadoJuego *);
void borrar_lineas_completas(EstadoJuego *);




#endif // TETRIS_H_INCLUDED
