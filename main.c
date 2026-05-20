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
    ResolucionVentana ventana;
    char opcion;
    int opcion_final;


    //Mostramos el menu para elegir si es vga o cga
    printf("Seleccione la resolucion:\n");
    printf("0. CGA (320x200)\n");
    printf("1. VGA (640x480)\n");
    printf("Opcion: ");
    do
    {
        scanf("%c", &opcion);
        while (getchar() != '\n');
        if (opcion != '0' && opcion != '1')
        {
            printf("Opcion invalida. Ingresar nuevamente: ");
        }
    }
    while(opcion != '0' && opcion != '1');

    opcion_final = opcion - '0';

    if(inicializar_graficos(&ventana,1, opcion_final)== -1) //Cerramos programa
        return -1;

    if (gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888) != 0)
    {
        fprintf(stderr, "Error al aplicar la nueva paleta de colores: %s\n", gbt_obtener_log());
        return -1;
    }



    tGBT_Temporizador*timer_caida;
    timer_caida=gbt_temporizador_crear(1.0);
    if (!timer_caida)
    {
        fprintf(stderr, "Error al crear el temporizador para los dibujos: %s\n", gbt_obtener_log());
        return -1;
    }

    puts("\nAbriendo el Juego! Mucha suerte!\n\nPara salir del juego presione ESC.");
    inicializar_juego(&estado);

    while (estado.game_over == 0)//GAME LOOP
    {
        // 1.Procesar input

        gbt_procesar_entrada();

        tecla_tocada=gbt_obtener_tecla_presionada();

        if (gbt_tecla_presionada(GBTK_ESCAPE))
        {
            estado.game_over = 1;
            printf("Saliendo del juego. Gracias por jugar! :) \n");
        }

        if (tecla_tocada == GBTK_IZQUIERDA) //Verificamos si se puede mover
        {
            mover_pieza(&estado,-1,0);
        }
        else if (tecla_tocada == GBTK_DERECHA)  //Verificamos si se puede mover
        {
            mover_pieza(&estado,1,0);
        }
        else if (tecla_tocada == GBTK_ARRIBA || tecla_tocada == GBTK_ESPACIO) //Rotamos la pieza
        {
            rotar_pieza_actual(&estado,1);
        }
        else if(tecla_tocada == GBTK_ABAJO)
        {
            mover_pieza(&estado, 0, 1);
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

        gbt_borrar_backbuffer(0); // Limpiar pantalla

        dibujar_tablero(&estado, &ventana);
        dibujar_pieza(&estado, &ventana);

        // (Opcional) Dibujar el puntaje o la pieza siguiente aquí

        gbt_volcar_backbuffer();//Mandamos el nuevo dibujo al backbuffer

        gbt_esperar(16);//Es para limitar el uso de cpu
    }

    gbt_temporizador_destruir(timer_caida);
    gbt_destruir_ventana();
    gbt_cerrar();

    puts("Programa finalizado correctamente.\n");

    return 0;
}
