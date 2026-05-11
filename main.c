/*
Apellido(s), nombre(s): Kouvach, Marcos
DNI: 45013925
Usuario: mkouvach
Entrega: Si

Apellido(s), nombre(s): Alvarez da Silva, Keila
DNI: 45236512
Usuario: keydasilva7
Entrega: Si


Apellido(s), nombre(s): Crego, Agustina Rocio
DNI: 44689109
Usuario: Agusrc20
Entrega: Si

*/
#include "GBT/gbt.h"
#include "tetris.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ANCHO_VENTANA 128
#define ALTO_VENTANA 128
#define ESCALA_VENTANA 5
#define CANT_COLORES 16
#define TAM_GRILLA 11

tGBT_ColorRGB paletaCGA[CANT_COLORES] = {

    /// 0-15: Colores CGA (16 colores)
    {0x00, 0x00, 0x00}, // 0:   Negro
    {0x00, 0x00, 0xAA}, // 1:   Azul
    {0x00, 0xAA, 0x00}, // 2:   Verde
    {0x00, 0xAA, 0xAA}, // 3:   Cian
    {0xAA, 0x00, 0x00}, // 4:   Rojo
    {0xAA, 0x00, 0xAA}, // 5:   Magenta
    {0xAA, 0x55, 0x00}, // 6:   Marron
    {0xAA, 0xAA, 0xAA}, // 7:   Gris claro
    {0x55, 0x55, 0x55}, // 8:   Gris oscuro
    {0x55, 0x55, 0xFF}, // 9:   Azul brillante
    {0x55, 0xFF, 0x55}, // 10:  Verde brillante
    {0x55, 0xFF, 0xFF}, // 11:  Cian brillante
    {0xFF, 0x55, 0x55}, // 12:  Rojo brillante
    {0xFF, 0x55, 0xFF}, // 13:  Magenta brillante
    {0xFF, 0xFF, 0x55}, // 14:  Amarillo
    {0xFF, 0xFF, 0xFF}  // 15:  Usado como transparente por GBT
};

int main() {

  EstadoJuego estado;

  gbt_iniciar();
  gbt_crear_ventana("Hola mundo", ANCHO_VENTANA, ALTO_VENTANA, ESCALA_VENTANA);
  inicializar_juego(&estado)

      while (estado.game_over == 0) {}

  gbt_esperar(16);
  gbt_cerrar();
  return 0;
}
