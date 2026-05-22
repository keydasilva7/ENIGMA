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
#include <string.h>
#include <time.h>

#define CANT_COLORES 16

typedef enum {
    ESTADO_MENU,
    ESTADO_AJUSTES,
    ESTADO_JUGANDO,
    ESTADO_GAMEOVER
} EstadoAplicacion;

// Prototipos modificados (ahora actualizar_ajustes recibe un puntero a entero)
void actualizar_menu(EstadoAplicacion* estado_app, eGBT_Tecla tecla, int* corriendo, EstadoJuego* estado_juego, tGBT_Temporizador** timer_caida);
void actualizar_ajustes(EstadoAplicacion* estado_app, eGBT_Tecla tecla, ResolucionVentana* ventana, int* sub_menu);
void actualizar_jugando(EstadoAplicacion* estado_app, eGBT_Tecla tecla, EstadoJuego* estado_juego, ResolucionVentana* ventana, tGBT_Temporizador* timer_caida);
void actualizar_gameover(EstadoAplicacion* estado_app, eGBT_Tecla tecla, EstadoJuego* estado_juego, ResolucionVentana* ventana);
int reiniciar_entorno_grafico(ResolucionVentana* ventana);

tGBT_ColorRGB paletaCGA[CANT_COLORES] = {
    {0x00, 0x00, 0x00}, {0x00, 0x00, 0xAA}, {0x00, 0xAA, 0x00}, {0x00, 0xAA, 0xAA},
    {0xAA, 0x00, 0x00}, {0xAA, 0x00, 0xAA}, {0xAA, 0x55, 0x00}, {0xAA, 0xAA, 0xAA},
    {0x55, 0x55, 0x55}, {0x55, 0x55, 0xFF}, {0x55, 0xFF, 0x55}, {0x55, 0xFF, 0xFF},
    {0xFF, 0x55, 0x55}, {0xFF, 0x55, 0xFF}, {0xFF, 0xFF, 0x55}, {0xFF, 0xFF, 0xFF}
};

int main(int argc, char* argv[]) {
    ResolucionVentana ventana;
    ventana.es_vga = 0;
    ventana.escala = 2;

    if (inicializar_graficos(&ventana, ventana.escala, ventana.es_vga) == -1) {
        fprintf(stderr, "Error al iniciar entorno grafico base.\n");
        return -1;
    }
    gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888);
    srand((unsigned)time(NULL));

    EstadoAplicacion estado_app = ESTADO_MENU;
    EstadoJuego estado_juego;
    tGBT_Temporizador* timer_caida = NULL;
    int corriendo = 1;

    int sub_menu_ajustes = 0;

    while (corriendo) {
        gbt_procesar_entrada();
        eGBT_Tecla tecla = gbt_obtener_tecla_presionada();
        gbt_borrar_backbuffer(0);

        switch (estado_app) {
            case ESTADO_MENU:
                actualizar_menu(&estado_app, tecla, &corriendo, &estado_juego, &timer_caida);
                break;
            case ESTADO_AJUSTES:
                // Pasamos la variable por referencia para que la función pueda leerla y modificarla
                actualizar_ajustes(&estado_app, tecla, &ventana, &sub_menu_ajustes);
                break;
            case ESTADO_JUGANDO:
                actualizar_jugando(&estado_app, tecla, &estado_juego, &ventana, timer_caida);
                break;
            case ESTADO_GAMEOVER:
                actualizar_gameover(&estado_app, tecla, &estado_juego, &ventana);
                break;
        }

        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }

    if (timer_caida) gbt_temporizador_destruir(timer_caida);
    gbt_destruir_ventana();
    gbt_cerrar();

    puts("Programa finalizado correctamente.\n");
    return 0;
}

void actualizar_ajustes(EstadoAplicacion* estado_app, eGBT_Tecla tecla, ResolucionVentana* ventana, int* sub_menu) {
    char buffer_texto[64];

    // Ahora usamos el operador de desreferencia (*) para acceder al valor original
    if (*sub_menu == 0) {
        dibujar_texto("--- AJUSTES ---", 40, 20, 11);
        sprintf(buffer_texto, "Modo actual: %s", ventana->es_vga ? "VGA" : "CGA");
        dibujar_texto(buffer_texto, 20, 50, 7);
        sprintf(buffer_texto, "Escala actual: %d", ventana->escala);
        dibujar_texto(buffer_texto, 20, 65, 7);
        dibujar_texto("1. Cambiar Modo Grafico", 20, 95, 14);
        dibujar_texto("2. Cambiar Escala", 20, 115, 14);
        dibujar_texto("3. Volver al Menu", 20, 135, 12);

        if (tecla == GBTK_1) *sub_menu = 1;
        else if (tecla == GBTK_2) *sub_menu = 2;
        else if (tecla == GBTK_3 || tecla == GBTK_ESCAPE) {
            *estado_app = ESTADO_MENU;
        }
    }
    else if (*sub_menu == 1) {
        dibujar_texto("--- MODO GRAFICO ---", 40, 20, 11);
        dibujar_texto("0. CGA (320x200)", 20, 70, 14);
        dibujar_texto("1. VGA (640x480)", 20, 90, 14);
        dibujar_texto("Presione 0 o 1 (ESC atras)", 20, 140, 8);

        if (tecla == GBTK_0) {
            ventana->es_vga = 0;
            reiniciar_entorno_grafico(ventana);
            *sub_menu = 0;
        } else if (tecla == GBTK_1) {
            ventana->es_vga = 1;
            reiniciar_entorno_grafico(ventana);
            *sub_menu = 0;
        } else if (tecla == GBTK_ESCAPE) {
            *sub_menu = 0;
        }
    }
    else if (*sub_menu == 2) {
        dibujar_texto("--- CAMBIAR ESCALA ---", 40, 20, 11);
        dibujar_texto("1. Escala 1", 20, 60, 14);
        dibujar_texto("2. Escala 2", 20, 80, 14);
        dibujar_texto("3. Escala 3", 20, 100, 14);
        dibujar_texto("Presione 1, 2 o 3 (ESC atras)", 20, 140, 8);

        if (tecla == GBTK_1) { ventana->escala = 1; reiniciar_entorno_grafico(ventana); *sub_menu = 0; }
        else if (tecla == GBTK_2) { ventana->escala = 2; reiniciar_entorno_grafico(ventana); *sub_menu = 0; }
        else if (tecla == GBTK_3) { ventana->escala = 3; reiniciar_entorno_grafico(ventana); *sub_menu = 0; }
        else if (tecla == GBTK_ESCAPE) *sub_menu = 0;
    }
}

void actualizar_menu(EstadoAplicacion* estado_app, eGBT_Tecla tecla, int* corriendo, EstadoJuego* estado_juego, tGBT_Temporizador** timer_caida) {
    dibujar_texto("=== TETRIS ===", 40, 20, 14);
    dibujar_texto("1. JUGAR", 40, 60, 10);
    dibujar_texto("2. AJUSTES", 40, 80, 11);
    dibujar_texto("3. SALIR", 40, 100, 12);
    dibujar_texto("Presione el numero [1-3]", 30, 150, 7);

    if (tecla == GBTK_1) {
        inicializar_juego(estado_juego);
        if (*timer_caida) gbt_temporizador_destruir(*timer_caida);
        *timer_caida = gbt_temporizador_crear(1.0);
        *estado_app = ESTADO_JUGANDO;
    } else if (tecla == GBTK_2) {
        *estado_app = ESTADO_AJUSTES;
    } else if (tecla == GBTK_3 || tecla == GBTK_ESCAPE) {
        *corriendo = 0;
    }
}

void actualizar_jugando(EstadoAplicacion* estado_app, eGBT_Tecla tecla, EstadoJuego* estado_juego, ResolucionVentana* ventana, tGBT_Temporizador* timer_caida) {
    if (gbt_tecla_presionada(GBTK_ESCAPE)) {
        *estado_app = ESTADO_MENU;
        return;
    }

    if (tecla == GBTK_IZQUIERDA)       mover_pieza(estado_juego, -1, 0);
    else if (tecla == GBTK_DERECHA)    mover_pieza(estado_juego, 1, 0);
    else if (tecla == GBTK_ABAJO)
    {
        mover_pieza(estado_juego, 0, 1);
        estado_juego->puntos++;
    }
    else if (tecla == GBTK_ARRIBA || tecla == GBTK_ESPACIO) rotar_pieza_actual(estado_juego, 1);

    if (gbt_temporizador_consumir(timer_caida)) {
        if (puede_mover_pieza(estado_juego, 0, 1)) {
            mover_pieza(estado_juego, 0, 1);
        } else {
            fijar_pieza(estado_juego);
            borrar_lineas_completas(estado_juego);
            estado_juego->pieza_actual = estado_juego->pieza_siguiente;
            generar_nueva_pieza(estado_juego);

            if (!puede_mover_pieza(estado_juego, 0, 0)) {
                estado_juego->game_over = 1;
            }
        }
    }

    if (estado_juego->game_over) {
        *estado_app = ESTADO_GAMEOVER;
    }

    dibujar_tablero(estado_juego, ventana);
    dibujar_pieza(estado_juego, ventana);
    dibujar_ui(estado_juego, ventana);
}

void actualizar_gameover(EstadoAplicacion* estado_app, eGBT_Tecla tecla, EstadoJuego* estado_juego, ResolucionVentana* ventana) {
    dibujar_interfaz_game_over(estado_juego, ventana);

    if (tecla == GBTK_ESPACIO || tecla == GBTK_ESCAPE) {
        *estado_app = ESTADO_MENU;
    }
}

int reiniciar_entorno_grafico(ResolucionVentana* ventana) {
    gbt_destruir_ventana();
    gbt_cerrar();
    if (inicializar_graficos(ventana, ventana->escala, ventana->es_vga) == -1) return -1;
    if (gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888) != 0) return -1;
    return 0;
}
