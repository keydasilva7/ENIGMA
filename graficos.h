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

int inicializar_graficos(ResolucionVentana*,int,int);
void dibujar_bloque_cuadrado(ResolucionVentana*, int,int,int);
void dibujar_pieza(EstadoJuego *, ResolucionVentana* );
void dibujar_tablero(EstadoJuego*, ResolucionVentana* );
void dibujar_caracter(char, uint16_t, uint16_t, uint8_t);
void dibujar_texto(const char*, uint16_t, uint16_t, uint8_t);
void dibujar_ui(EstadoJuego *, ResolucionVentana *);
void dibujar_interfaz_game_over(EstadoJuego*, ResolucionVentana*);



#endif // GRAFICOS_H_INCLUDED
