#ifndef GRAFICOS_H_INCLUDED
#define GRAFICOS_H_INCLUDED

#include <stdint.h>
#include "tetris.h"

// Estado del juego
typedef struct
{
    int ancho;
    int alto;
    int escala;
    int es_vga;
    char nombreVentana[128];
    int tamano_bloque;

}ResolucionVentana;

// Inicializacion
int inicializar_graficos(ResolucionVentana*, int, int);


void dibujar_bloque_cuadrado(ResolucionVentana*, int, int, int);
void dibujar_rectangulo(int, int, int, int, int);
void dibujar_caracter(char, uint16_t, uint16_t, uint8_t);
void dibujar_texto(const char*, uint16_t, uint16_t, uint8_t);

// Pantallas
void dibujar_tablero(EstadoJuego*, ResolucionVentana* );
void dibujar_pieza(EstadoJuego *, ResolucionVentana* );
void dibujar_ui(EstadoJuego *, ResolucionVentana *);
void dibujar_menu(int);
void dibujar_pantalla_pausa(ResolucionVentana*);
void dibujar_interfaz_game_over(EstadoJuego*, ResolucionVentana*, Jugador*);

//Var Global de Ventana (utilizada para funciones que no reciben ventana)
extern ResolucionVentana* g_ventana;

//Jugador
int actualizar_ingreso_nombre(eGBT_Tecla, Jugador*, ResolucionVentana*);

#endif // GRAFICOS_H_INCLUDED
