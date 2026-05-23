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

typedef enum
{
    ESTADO_MENU,
    ESTADO_INGRESO_NOMBRE,
    ESTADO_AJUSTES,
    ESTADO_ESTADISTICAS,
    ESTADO_JUGANDO,
    ESTADO_PAUSADO,
    ESTADO_GAMEOVER
} EstadoAplicacion;

// Prototipos modificados (ahora actualizar_ajustes recibe un puntero a entero)
void actualizar_menu(EstadoAplicacion* estado_app, eGBT_Tecla tecla, int* corriendo, EstadoJuego* estado_juego, tGBT_Temporizador** timer_caida, tGBT_Temporizador** timer_fijacion, int* opcion_menu, Jugador* jug);
void actualizar_ajustes(EstadoAplicacion* estado_app, eGBT_Tecla tecla, ResolucionVentana* ventana, int* sub_menu, Jugador* jug, TablaPuntajes* tabla);
void actualizar_jugando(EstadoAplicacion* estado_app, eGBT_Tecla tecla, EstadoJuego* estado_juego, ResolucionVentana* ventana, tGBT_Temporizador** timer_caida, tGBT_Temporizador** timer_fijacion);
void actualizar_pausado(EstadoAplicacion* estado_app, eGBT_Tecla tecla, ResolucionVentana* ventana);
void actualizar_gameover(EstadoAplicacion* estado_app, eGBT_Tecla tecla, EstadoJuego* estado_juego, ResolucionVentana* ventana, Jugador* jug, TablaPuntajes* tabla);
void actualizar_estadisticas(EstadoAplicacion* estado_app, eGBT_Tecla tecla, ResolucionVentana* ventana, TablaPuntajes* tabla);
int reiniciar_entorno_grafico(ResolucionVentana* ventana);
void actualizar_velocidad(EstadoJuego* estado_juego, tGBT_Temporizador** timer_caida, tGBT_Temporizador** timer_fijacion);

tGBT_ColorRGB paletaCGA[CANT_COLORES] =
{
    {0x00, 0x00, 0x00}, {0x00, 0x00, 0xAA}, {0x00, 0xAA, 0x00}, {0x00, 0xAA, 0xAA},
    {0xAA, 0x00, 0x00}, {0xAA, 0x00, 0xAA}, {0xAA, 0x55, 0x00}, {0xAA, 0xAA, 0xAA},
    {0x55, 0x55, 0x55}, {0x55, 0x55, 0xFF}, {0x55, 0xFF, 0x55}, {0x55, 0xFF, 0xFF},
    {0xFF, 0x55, 0x55}, {0xFF, 0x55, 0xFF}, {0xFF, 0xFF, 0x55}, {0xFF, 0xFF, 0xFF}
};

int main(int argc, char* argv[])
{
    ResolucionVentana ventana;
    ventana.es_vga = 0;
    ventana.escala = 2;
    Jugador jug;
    TablaPuntajes tabla;
    tabla.cantidad = 0;

    memset(&jug, 0, sizeof(jug));
    if(cargar_puntajes(&tabla) && tabla.cantidad > 0)
    {
        ventana.es_vga = tabla.jugadores[0].es_vga;
        ventana.escala = tabla.jugadores[0].escala;
        jug.es_vga = tabla.jugadores[0].es_vga;
        jug.escala = tabla.jugadores[0].escala;
    }

    // Procesar argumentos de linea de comandos (sobreescriben los ajustes guardados)
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
    gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888);
    srand((unsigned)time(NULL));

    EstadoAplicacion estado_app = ESTADO_MENU;
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
        case ESTADO_MENU:
            actualizar_menu(&estado_app, tecla, &corriendo, &estado_juego, &timer_caida, &timer_fijacion, &opcion_menu, &jug);
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
            if (actualizar_ingreso_nombre(tecla, &jug, &ventana))
            {
                // Jugador confirmo nombre -> inicia juego
                jug.mejor_puntaje = 0;
                for(int i = 0; i < tabla.cantidad; i++) {
                    if(strcmp(tabla.jugadores[i].nombre, jug.nombre) == 0) {
                        jug.mejor_puntaje = tabla.jugadores[i].mejor_puntaje;
                        break;
                    }
                }
                strncpy(estado_juego.nombre_jugador, jug.nombre, 31);
                inicializar_juego(&estado_juego);
                if (timer_caida) gbt_temporizador_destruir(timer_caida);
                timer_fijacion = NULL;
                double seg = estado_juego.velocidad_caida_ms / 1000.0;
                timer_caida = gbt_temporizador_crear(seg);
                estado_app = ESTADO_JUGANDO;
            }
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

//-----Velocidad: recalcula ambos timers según piezas_caidas
//cada 10 piezas la velocidad sube un 3% (multiplicamos por 0.97)

void actualizar_velocidad(EstadoJuego* ej, tGBT_Temporizador** tc, tGBT_Temporizador** tf)
{
    //actualiza la velocidad_caida_ms cada vez que cae una pieza
    if(*tc)
    {
        gbt_temporizador_destruir(*tc);
    }
    else
    {
        *tc = NULL;
    }
    if(*tf)
    {
        gbt_temporizador_destruir(*tf);
    }
    else
    {
        *tf = NULL;
    }

    double seg_caida = ej->velocidad_caida_ms / 1000.0;
    *tc = gbt_temporizador_crear(seg_caida);
}
//--MENU - selector visual con flechas

void actualizar_menu(EstadoAplicacion* estado_app, eGBT_Tecla tecla, int* corriendo,EstadoJuego* estado_juego, tGBT_Temporizador** timer_caida, tGBT_Temporizador** timer_fijacion, int* opcion_menu, Jugador* jug)
{
    // Mover selector con flechas
    if (tecla == GBTK_ARRIBA) *opcion_menu = (*opcion_menu + 3) % 4;
    if (tecla == GBTK_ABAJO)  *opcion_menu = (*opcion_menu + 1) % 4;

    // Atajos numericos
    if (tecla == GBTK_1) *opcion_menu = 0;
    if (tecla == GBTK_2) *opcion_menu = 1;
    if (tecla == GBTK_3) *opcion_menu = 2;
    if (tecla == GBTK_4) *opcion_menu = 3;

    dibujar_menu(*opcion_menu);

    // Confirmar
    int confirmar = (tecla == GBTK_ENTER || tecla == GBTK_ESPACIO ||
                     tecla == GBTK_1 || tecla == GBTK_2 || tecla == GBTK_3 || tecla == GBTK_4);

    if (confirmar)
    {
        if (*opcion_menu == 0)
        {
            // JUGAR -> pedir nombre primero
            jug->nombre[0]='\0';
            *estado_app = ESTADO_INGRESO_NOMBRE;
        }
        else if (*opcion_menu == 1)
        {
            // AJUSTES
            *estado_app = ESTADO_AJUSTES;
        }
        else if (*opcion_menu == 2)
        {
            // ESTADISTICAS
            *estado_app = ESTADO_ESTADISTICAS;
        }
        else
        {
            // SALIR
            *corriendo = 0;
        }
    }

    if (tecla == GBTK_ESCAPE) *corriendo = 0;
}

//--Ajustes

void actualizar_ajustes(EstadoAplicacion* estado_app, eGBT_Tecla tecla, ResolucionVentana* ventana, int* sub_menu, Jugador* jug, TablaPuntajes* tabla)
{
    char buffer_texto[64];

    if (*sub_menu == 0)
    {
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
        else if (tecla == GBTK_3 || tecla == GBTK_ESCAPE)
            *estado_app = ESTADO_MENU;
    }
    else if (*sub_menu == 1)
    {
        dibujar_texto("--- MODO GRAFICO ---", 40, 20, 11);
        dibujar_texto("0. CGA (320x200)", 20, 70, 14);
        dibujar_texto("1. VGA (640x480)", 20, 90, 14);
        dibujar_texto("Presione 0 o 1 (ESC atras)", 20, 140, 8);

        if (tecla == GBTK_0)
        {
            ventana->es_vga = 0;
            reiniciar_entorno_grafico(ventana);
            jug->es_vga = 0;
            if(tabla->cantidad > 0) { tabla->jugadores[0].es_vga = 0; guardar_puntajes(tabla); }
            *sub_menu = 0;
        }
        else if (tecla == GBTK_1)
        {
            ventana->es_vga = 1;
            reiniciar_entorno_grafico(ventana);
            jug->es_vga = 1;
            if(tabla->cantidad > 0) { tabla->jugadores[0].es_vga = 1; guardar_puntajes(tabla); }
            *sub_menu = 0;
        }
        else if (tecla == GBTK_ESCAPE)
            *sub_menu = 0;
    }
    else if (*sub_menu == 2)
    {
        dibujar_texto("--- CAMBIAR ESCALA ---", 40, 20, 11);
        dibujar_texto("1. Escala 1", 20, 60, 14);
        dibujar_texto("2. Escala 2", 20, 80, 14);
        dibujar_texto("3. Escala 3", 20, 100, 14);
        dibujar_texto("4. Escala 4", 20, 120, 14);
        dibujar_texto("Presione 1, 2, 3 o 4 (ESC atras)", 20, 140, 8);

        int nueva_escala = 0;
        if      (tecla == GBTK_1) nueva_escala = 1;
        else if (tecla == GBTK_2) nueva_escala = 2;
        else if (tecla == GBTK_3) nueva_escala = 3;
        else if (tecla == GBTK_4) nueva_escala = 4;
        else if (tecla == GBTK_ESCAPE) { *sub_menu = 0; return; }

        if (nueva_escala > 0)
        {
            ventana->escala = nueva_escala;
            reiniciar_entorno_grafico(ventana);
            jug->escala = nueva_escala;
            if(tabla->cantidad > 0) { tabla->jugadores[0].escala = nueva_escala; guardar_puntajes(tabla); }
            *sub_menu = 0;
        }
    }
}

//--Jugando
void actualizar_jugando(EstadoAplicacion* estado_app, eGBT_Tecla tecla, EstadoJuego* estado_juego, ResolucionVentana* ventana, tGBT_Temporizador** timer_caida, tGBT_Temporizador** timer_fijacion)
{
    //Pausa
    if (tecla == GBTK_ESCAPE)
    {
        *estado_app = ESTADO_MENU;
        return;
    }
    if (tecla == GBTK_p)
    {
        *estado_app = ESTADO_PAUSADO;  // P → pausa
        return;
    }

    //--Movimiento horizontal
    if (tecla == GBTK_IZQUIERDA)       mover_pieza(estado_juego, -1, 0);
    else if (tecla == GBTK_DERECHA)    mover_pieza(estado_juego, 1, 0);

    //--Rotacion izquierda y derecha
    else if (tecla == GBTK_ARRIBA || tecla == GBTK_ESPACIO) rotar_pieza_actual(estado_juego, 1);
    else if (tecla == GBTK_z) rotar_pieza_actual(estado_juego, -1);

    //--Bajar Rapido
    else if (tecla == GBTK_ABAJO )
    {
        if(puede_mover_pieza(estado_juego, 0, 1))
        {
            mover_pieza(estado_juego, 0, 1);

            //A mayor velocidad, la pieza cae mas rapido sola, mayor puntos por jugador
            estado_juego->puntos += (long)(1000.0f / estado_juego -> velocidad_caida_ms);
            
            // Si al bajar chocamos con el suelo, iniciamos el timer de fijación inmediatamente
            if(!puede_mover_pieza(estado_juego, 0, 1) && *timer_fijacion == NULL)
            {
                // Fijacion ultra rapida si el jugador forzo la caida al fondo
                *timer_fijacion = gbt_temporizador_crear(0.001);
            }
        }
    }

    if (gbt_temporizador_consumir(*timer_caida))
    {
        if (puede_mover_pieza(estado_juego, 0, 1))
        {
            mover_pieza(estado_juego, 0, 1);
            // La pieza bajo -> destruimos timer de fijación si es que existia
            {
                gbt_temporizador_destruir(*timer_fijacion);
                *timer_fijacion = NULL;
            }
        }
        else
        {
            // La pieza NO puede bajar -> activamos timer de fijacion si no esta corriendo
            if (*timer_fijacion == NULL)
            {
                double seg_fij = (estado_juego->velocidad_caida_ms * 0.5) / 1000.0;
                *timer_fijacion = gbt_temporizador_crear(seg_fij);
            }
        }
    }
    //--Timer de fijacion
    //Solo fija si la pieza NO puede seguir bajando porque toca la base o choca con otra pieza
    if(*timer_fijacion && gbt_temporizador_consumir(*timer_fijacion))
    {
        gbt_temporizador_destruir(*timer_fijacion);
        *timer_fijacion = NULL;

        if(!puede_mover_pieza(estado_juego, 0, 1))
        {
            int piezas_antes = estado_juego->piezas_caidas;
            estado_juego->piezas_caidas++; // Sumamos la pieza que acaba de caer al contador

            fijar_pieza(estado_juego);
            borrar_lineas_completas(estado_juego);
            estado_juego->pieza_actual = estado_juego->pieza_siguiente;
            generar_nueva_pieza(estado_juego);

            //bonus de puntos por velocidad: mas velocidad mas puntos al fijar
            long bonus_velocidad = (long)(1000.0f / estado_juego->velocidad_caida_ms * 10);
            estado_juego->puntos += bonus_velocidad;

            //Cada 10 piezas subimos dificultad
            if((estado_juego->piezas_caidas / 10) > (piezas_antes / 10))
            {
                recalcular_velocidad(estado_juego);
                actualizar_velocidad(estado_juego, timer_caida, timer_fijacion);
            }

            if(!puede_mover_pieza(estado_juego, 0, 0))
            {
                estado_juego->game_over = 1;
            }
        }
    }

    if(estado_juego->game_over)
    {
        *estado_app = ESTADO_GAMEOVER;
        return;
    }

    dibujar_tablero(estado_juego, ventana);
    dibujar_pieza(estado_juego, ventana);
    dibujar_ui(estado_juego, ventana);
}

//--Pausa
void actualizar_pausado(EstadoAplicacion* estado_app, eGBT_Tecla tecla, ResolucionVentana* ventana)
{
    dibujar_pantalla_pausa(ventana);
    if(tecla == GBTK_p || tecla == GBTK_ENTER)
    {
        *estado_app = ESTADO_JUGANDO;
    }
    else if(tecla == GBTK_ESCAPE)
    {
        *estado_app = ESTADO_MENU;
    }
}

//--Game Over

void actualizar_gameover(EstadoAplicacion* estado_app, eGBT_Tecla tecla, EstadoJuego* estado_juego, ResolucionVentana* ventana, Jugador* jug, TablaPuntajes* tabla)
{
    dibujar_interfaz_game_over(estado_juego, ventana, jug);

    if (tecla == GBTK_ESPACIO || tecla == GBTK_ESCAPE || tecla == GBTK_ENTER)
    {
        // Actualiza mejor puntaje en caso de corresponder
        if (estado_juego->puntos > jug->mejor_puntaje)
        {
            jug->mejor_puntaje = estado_juego->puntos;
            jug->es_vga  = ventana->es_vga;
            jug->escala  = ventana->escala;
        }
        
        // Solo guardar en el ranking si hizo mas de 0 puntos
        if (jug->mejor_puntaje > 0)
        {
            actualizar_o_agregar_jugador(tabla, jug);
            guardar_puntajes(tabla);
        }
        
        *estado_app = ESTADO_MENU;
    }
}

//--Estadisticas
void actualizar_estadisticas(EstadoAplicacion* estado_app, eGBT_Tecla tecla, ResolucionVentana* ventana, TablaPuntajes* tabla)
{
    dibujar_estadisticas(tabla, ventana);
    if(tecla == GBTK_ESCAPE || tecla == GBTK_ENTER)
    {
        *estado_app = ESTADO_MENU;
    }
}

int reiniciar_entorno_grafico(ResolucionVentana* ventana)
{
    gbt_destruir_ventana();
    gbt_cerrar();
    if (inicializar_graficos(ventana, ventana->escala, ventana->es_vga) == -1) return -1;
    if (gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888) != 0) return -1;
    return 0;
}
