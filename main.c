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
#include "juego.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main(int argc, char* argv[])
{
    ResolucionVentana ventana;
    Jugador jug;
    TablaPuntajes tabla;
    tabla.cantidad = 0;

    memset(&jug, 0, sizeof(jug));
    jug.paleta = 0;
    jug.velocidad_inicial = 1000.0f;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-vga") == 0)
        {
            ventana.es_vga = 1;
            jug.es_vga = 1;
        }
        else if (strcmp(argv[i], "-cga") == 0)
        {
            ventana.es_vga = 0;
            jug.es_vga = 0;
        }
        else if (strcmp(argv[i], "-escala") == 0 && i + 1 < argc)
        {
            int esc = atoi(argv[i + 1]);
            if (esc > 0)
            {
                ventana.escala = esc;
                jug.escala = esc;
            }
            i++; // saltar el numero
        }
    }

    if (inicializar_graficos(&ventana, ventana.escala, ventana.es_vga) == -1)
    {
        fprintf(stderr, "Error al iniciar entorno grafico base.\n");
        return -1;
    }

    cargar_puntajes(&tabla);

    // Procesar argumentos de linea de comandos (sobreescriben los ajustes guardados)


    if (jug.paleta == 1) gbt_aplicar_paleta(paletaNeon, CANT_COLORES, GBT_FORMATO_888);
    else gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888);

    srand((unsigned)time(NULL));

    EstadoAplicacion estado_app = ESTADO_PRESENTACION;
    EstadoJuego estado_juego;
    tGBT_Temporizador* timer_caida = NULL;
    tGBT_Temporizador* timer_fijacion = NULL;
    int corriendo = 1;
    int sub_menu_ajustes = 0;
    int opcion_menu = 0;

    while (corriendo)
    {
        gbt_procesar_entrada();
        eGBT_Tecla tecla = gbt_obtener_tecla_presionada();
        gbt_borrar_backbuffer(0);

        switch (estado_app)
        {
        case ESTADO_PRESENTACION:
            actualizar_presentacion(&estado_app, tecla, &ventana);
            break;
        case ESTADO_MENU:
            actualizar_menu(&estado_app, tecla, &corriendo, &estado_juego, &timer_caida, &timer_fijacion, &opcion_menu, &jug, &ventana);
            break;
        case ESTADO_AJUSTES:
            // Pasamos la variable por referencia para que la funcion pueda leerla y modificarla
            actualizar_ajustes(&estado_app, tecla, &ventana, &sub_menu_ajustes, &jug, &tabla);
            break;
        case ESTADO_ESTADISTICAS:
            actualizar_estadisticas(&estado_app, tecla, &ventana, &tabla);
            break;
        case ESTADO_JUGANDO:
            actualizar_jugando(&estado_app, tecla, &estado_juego, &ventana, &timer_caida, &timer_fijacion);
            break;
        case ESTADO_PAUSADO:
            actualizar_pausado(&estado_app, tecla, &ventana);
            break;
        case ESTADO_GAMEOVER:
            actualizar_gameover(&estado_app, tecla, &estado_juego, &ventana, &jug, &tabla);
            break;
        case ESTADO_INGRESO_NOMBRE:
            procesar_ingreso_nombre(&estado_app, tecla, &jug, &ventana, &tabla);
            break;
        }

        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }

    if (timer_caida) gbt_temporizador_destruir(timer_caida);
    if (timer_fijacion) gbt_temporizador_destruir(timer_fijacion);
    gbt_destruir_ventana();
    gbt_cerrar();

    puts("Programa finalizado correctamente.\n");
    return 0;
}

