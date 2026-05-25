#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "graficos.h"

#define CANT_COLORES 16

extern tGBT_ColorRGB paletaCGA[CANT_COLORES];
extern tGBT_ColorRGB paletaNeon[CANT_COLORES];


void actualizar_presentacion(EstadoAplicacion*, eGBT_Tecla, ResolucionVentana*);
void actualizar_menu(EstadoAplicacion*, eGBT_Tecla, int*, EstadoJuego*, tGBT_Temporizador**, tGBT_Temporizador**, int*, Jugador*, ResolucionVentana*);
void actualizar_ajustes(EstadoAplicacion*, eGBT_Tecla, ResolucionVentana*, int*, Jugador*, TablaPuntajes*);
void actualizar_jugando(EstadoAplicacion*, eGBT_Tecla, EstadoJuego*, ResolucionVentana*, tGBT_Temporizador**, tGBT_Temporizador**);
void actualizar_pausado(EstadoAplicacion*, eGBT_Tecla, ResolucionVentana*);
void actualizar_gameover(EstadoAplicacion*, eGBT_Tecla, EstadoJuego*, ResolucionVentana*, Jugador*, TablaPuntajes*);
void actualizar_estadisticas(EstadoAplicacion*, eGBT_Tecla, ResolucionVentana*, TablaPuntajes*);
int reiniciar_entorno_grafico(ResolucionVentana*);
void actualizar_velocidad(EstadoJuego*, tGBT_Temporizador**, tGBT_Temporizador**);
void procesar_ingreso_nombre(EstadoAplicacion*, eGBT_Tecla, Jugador*, ResolucionVentana*, TablaPuntajes*);

#endif // JUEGO_H_INCLUDED
