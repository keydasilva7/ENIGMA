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
void dibujar_pieza(EstadoJuego *estado, ResolucionVentana* );
void dibujar_tablero(EstadoJuego* estado, ResolucionVentana* );
void dibujar_caracter(char c, uint16_t oX, uint16_t oY, uint8_t color);
void dibujar_texto(const char* texto, uint16_t oX, uint16_t oY, uint8_t color);
void dibujar_ui(EstadoJuego *estado);



#endif // GRAFICOS_H_INCLUDED
