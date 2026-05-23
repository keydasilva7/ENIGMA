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

#define VELOCIDAD_INICIAL_MS 1000.0f // Velocidad inicial en ms
#define FACTOR_VELOCIDAD 0.97f // Factor de reuccion cada 10 piezas (3%)

#define ARCHIVO_JUGADOR "jugador.dat"

//Nuestra estructura de pieza (I, J, L, O, S, T, Z)
//Permite mover copiar o colisionar
typedef struct
{
    int tipo;      // que pieza es? I=0, J=1, L=2, O=3, S=4, T=5, Z=6
    int x, y;      // es la posicion en el tablero, sería como coordenadas en un eje x e y pero en realidad seria x la columna e y la fila
    int forma[4][4];  // es la forma que va a tomar

}Tetromino;

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
    char nombre_jugador[32];
    long mejor_puntaje;

}EstadoJuego;

typedef struct {
    char nombre[32];
    int  es_vga;
    int  escala;
    long mejor_puntaje;
} Jugador;

extern const int COLORES_PIEZAS[7];


// Inicializacion
void inicializar_tablero(EstadoJuego*);
void inicializar_estructura(EstadoJuego*);
void inicializar_juego(EstadoJuego*);

// Movimiento y colision
int puede_mover_pieza(EstadoJuego*, int, int);
void mover_pieza(EstadoJuego*, int, int);
int colision(EstadoJuego*, Tetromino*);

// Rotacion
void rotar_matriz(int [][4], int [][4], int);
int rotar_pieza_actual(EstadoJuego*, int);

// Funciones de juego
void fijar_pieza(EstadoJuego*);
void generar_nueva_pieza(EstadoJuego*);
void borrar_lineas_completas(EstadoJuego*);

// Dificultad
void recalcular_velocidad(EstadoJuego*);

// Jugador
void guardar_jugador(const Jugador*);
int cargar_jugador(Jugador*);




#endif // TETRIS_H_INCLUDED
