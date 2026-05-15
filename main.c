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
#include "graficos.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ANCHO_VENTANA 128
#define ALTO_VENTANA 128
#define ESCALA_VENTANA 5
#define CANT_COLORES 16
#define TAM_GRILLA 11

tGBT_ColorRGB paletaCGA[CANT_COLORES] =
{

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

int main()
{

    EstadoJuego estado;
    eGBT_Tecla tecla_tocada;

    if(inicializar_graficos(ESCALA_VENTANA,0)==-1) //Cerramos programa
        return -1;

    gbt_aplicar_paleta(paletaCGA, 16, GBT_FORMATO_888);
    inicializar_juego(&estado);

    tGBT_Temporizador*timer_caida;
    timer_caida=gbt_temporizador_crear(1.0);

    while (estado.game_over == 0)//GAME LOOP
    {
        // 1.Procesar input

        gbt_procesar_entrada();

        tecla_tocada=gbt_obtener_tecla_presionada();

        if (tecla_tocada == GBTK_IZQUIERDA) //Verificamos si se puede mover
        {
            puede_mover_pieza(&estado,-1,0);


        }
        else if (tecla_tocada == GBTK_DERECHA)  //Verificamos si se puede mover
        {
            puede_mover_pieza(&estado,1,0);

        }
        else if (tecla_tocada == GBTK_ARRIBA || tecla_tocada == GBTK_ESPACIO) //Rotamos la pieza
        {
            rotar_pieza_actual(&estado,1);

            if(tecla_tocada == GBTK_ABAJO)
            {

            }

        }

        // 2. Gravedad

        if(gbt_temporizador_consumir(timer_caida))
        {
            if(puede_mover_pieza(&estado, 0, 1))
            {
                mover_pieza(&estado, 0, 1);
            }
            else
            {
                fijar_pieza(&estado);

                borrar_lineas_completas(&estado);

                generar_nueva_pieza(&estado);

                if (!puede_mover_pieza(&estado, 0, 0)) //Si la pieza nueva choca con algo significa que ya se lleno
                {
                    estado.game_over = 1;
                }
            }

        }

        // 3.Dibujado

        gbt_borrar_backbuffer(N); // Limpiar pantalla

        dibujar_pieza(&estado);

        // (Opcional) Dibujar el puntaje o la pieza siguiente aquí

        gbt_volcar_backbuffer();//Mandamos el nuevo dibujo al backbuffer

        gbt_esperar(16);
    }


        gbt_cerrar();


    return 0;
}
