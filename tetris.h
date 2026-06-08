#ifndef TETRIS_H_INCLUDED
#define TETRIS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GBT/gbt.h"

#define PIXELES_X_LADO 8 //medida de un bloque
#define PX_PADDING 1 //separacion entre bloques

#define FILAS_VISIBLES 20
#define FILAS_OCULTAS 4
#define FILAS_TOTALES 24
#define COLUMNAS 10
#define MAX_JUGADORES 10

#define VELOCIDAD_INICIAL_MS 1000.0f
#define FACTOR_VELOCIDAD 0.97f

#define ARCHIVO_JUGADOR "jugador.dat"

typedef enum
{
    ESTADO_PRESENTACION,
    ESTADO_MENU,
    ESTADO_INGRESO_NOMBRE,
    ESTADO_AJUSTES,
    ESTADO_ESTADISTICAS,
    ESTADO_JUGANDO,
    ESTADO_PAUSADO,
    ESTADO_GAMEOVER

} EstadoAplicacion;

//Nuestra estructura de pieza (I, J, L, O, S, T, Z)

typedef struct
{
    int tipo;      // Que pieza es I=0, J=1, L=2, O=3, S=4, T=5, Z=6
    int x, y;      // Posicion en el tablero
    int forma[4][4];

} Tetromino;


typedef struct
{
    int tablero[FILAS_TOTALES][COLUMNAS];
    Tetromino pieza_actual;
    Tetromino pieza_siguiente;
    long puntos;
    int lineas;
    int piezas_caidas;
    float velocidad_caida_ms;
    int game_over;

} EstadoJuego;

typedef struct
{
    char nombre[32];
    int  es_vga;
    int  escala;
    int  paleta;
    float velocidad_inicial;
    long mejor_puntaje;

} Jugador;


typedef struct
{
    Jugador jugadores[MAX_JUGADORES];
    int cantidad;
} TablaPuntajes;

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
void guardar_puntajes(const TablaPuntajes*);
int cargar_puntajes(TablaPuntajes*);
int buscar_jugador(const TablaPuntajes*, const char*);
void actualizar_record_existente(TablaPuntajes*, int, const Jugador*);
void insertar_nuevo_o_reemplazar_peor(TablaPuntajes*, const Jugador*);
void ordenar_tabla_burbuja(TablaPuntajes*);
void actualizar_o_agregar_jugador(TablaPuntajes*, Jugador*);




#endif // TETRIS_H_INCLUDED
