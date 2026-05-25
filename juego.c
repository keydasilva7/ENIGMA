#include "juego.h"

tGBT_ColorRGB paletaCGA[CANT_COLORES] =
{
    {0x00, 0x00, 0x00}, {0x00, 0x00, 0xAA}, {0x00, 0xAA, 0x00}, {0x00, 0xAA, 0xAA},
    {0xAA, 0x00, 0x00}, {0xAA, 0x00, 0xAA}, {0xAA, 0x55, 0x00}, {0xAA, 0xAA, 0xAA},
    {0x55, 0x55, 0x55}, {0x55, 0x55, 0xFF}, {0x55, 0xFF, 0x55}, {0x55, 0xFF, 0xFF},
    {0xFF, 0x55, 0x55}, {0xFF, 0x55, 0xFF}, {0xFF, 0xFF, 0x55}, {0xFF, 0xFF, 0xFF}
};

tGBT_ColorRGB paletaNeon[CANT_COLORES] =
{
    {0x0A, 0x0A, 0x0A}, {0xFF, 0x00, 0x55}, {0x00, 0xFF, 0xAA}, {0x55, 0x00, 0xFF},
    {0xFF, 0xAA, 0x00}, {0x00, 0xAA, 0xFF}, {0xFF, 0x00, 0xFF}, {0xAA, 0xAA, 0xAA},
    {0x33, 0x33, 0x33}, {0xFF, 0x55, 0x88}, {0x55, 0xFF, 0xCC}, {0x88, 0x55, 0xFF},
    {0xFF, 0xCC, 0x55}, {0x55, 0xCC, 0xFF}, {0xFF, 0x55, 0xFF}, {0xFF, 0xFF, 0xFF}
};

void actualizar_presentacion(EstadoAplicacion* estado_app, eGBT_Tecla tecla, ResolucionVentana* ventana)
{
    int cx = ventana->ancho / 2;
    int cy = ventana->alto  / 2;

    // Contador estatico de frames
    static int contador_frames = 0;
    contador_frames++;

    char* texto_tetro = "TETRIS";
    int colores_tetro[] = {11, 13, 9, 14, 10, 12};
    for (int i = 0; i < 6; i++)
    {
        dibujar_caracter_escalado(texto_tetro[i], cx - 58 + (i * 20), cy - 75, colores_tetro[i], 2);
    }

    char* texto_minos = "ENIGMA";
    int colores_minos[] = {12, 14, 10, 11, 13, 9};
    for (int i = 0; i < 6; i++)
    {
        dibujar_caracter_escalado(texto_minos[i], cx - 58 + (i * 20), cy - 35, colores_minos[i], 2);
    }

    dibujar_texto_grande("UNLaM", cx - 20, cy + 10, 7);

    dibujar_texto("UNIVERSIDAD NACIONAL DE LA MATANZA", cx - 136, cy + 30, 7);


    int colores_blink[] = {10, 11, 14, 12, 13};
    int idx_color = (contador_frames / 15) % 5;
    int color_actual = colores_blink[idx_color];

    if ((contador_frames % 45) < 30)
    {
        dibujar_texto("< PRESS START <", cx - 60, cy + 55, color_actual);
    }

    char buffer_pie[24];
    sprintf(buffer_pie, "%d - UNLaM", 2026);
    int x_pie = ventana->ancho - (int)(strlen(buffer_pie) * 8) - 10;
    int y_pie = ventana->alto - 15;
    dibujar_texto(buffer_pie, x_pie, y_pie, 7);

    // Transicion de estado al presionar Enter o Espacio
    if (tecla == GBTK_ENTER || tecla == GBTK_ESPACIO)
    {
        *estado_app = ESTADO_INGRESO_NOMBRE;
    }
}

void procesar_ingreso_nombre(EstadoAplicacion* estado_app, eGBT_Tecla tecla, Jugador* jug, ResolucionVentana* ventana, TablaPuntajes* tabla)
{
    // Llamamos a la funcion que dibuja y lee el teclado
    if (actualizar_ingreso_nombre(tecla, jug, ventana))
    {
        // Inicializamos valores por defecto por si es un jugador nuevo
        jug->mejor_puntaje = 0;
        jug->paleta = 0;

        // Buscamos si el usuario ya existe usando nuestra funcion limpia
        int index = buscar_jugador(tabla, jug->nombre);

        if (index != -1)
        {
            jug->mejor_puntaje = tabla->jugadores[index].mejor_puntaje;
            jug->paleta = tabla->jugadores[index].paleta;
        }

        // Aplicamos la paleta correspondiente
        if (jug->paleta == 1)
            gbt_aplicar_paleta(paletaNeon, CANT_COLORES, GBT_FORMATO_888);
        else
            gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888);

        // Hacemos el cambio de estado de forma interna, igual que las demas
        *estado_app = ESTADO_MENU;
    }
}

//-----Velocidad: recalcula ambos timers segun piezas_caidas
//cada 10 piezas la velocidad sube un 3% (multiplicamos por 0.97)

void actualizar_velocidad(EstadoJuego* ej, tGBT_Temporizador** tc, tGBT_Temporizador** tf)
{

    if(*tc)
    {
        gbt_temporizador_destruir(*tc);
        *tc = NULL;
    }
    if(*tf)
    {
        gbt_temporizador_destruir(*tf);
        *tf = NULL;
    }

    double seg_caida = ej->velocidad_caida_ms / 1000.0;
    *tc = gbt_temporizador_crear(seg_caida);
}

//--MENU - selector visual con flechas
void actualizar_menu(EstadoAplicacion* estado_app, eGBT_Tecla tecla, int* corriendo,EstadoJuego* estado_juego,
                     tGBT_Temporizador** timer_caida, tGBT_Temporizador** timer_fijacion, int* opcion_menu, Jugador* jug, ResolucionVentana* ventana)
{
    // Mover selector con flechas
    if (tecla == GBTK_ARRIBA) *opcion_menu = (*opcion_menu + 3) % 4;
    if (tecla == GBTK_ABAJO)  *opcion_menu = (*opcion_menu + 1) % 4;

    // Atajos numericos
    if (tecla == GBTK_1) *opcion_menu = 0;
    if (tecla == GBTK_2) *opcion_menu = 1;
    if (tecla == GBTK_3) *opcion_menu = 2;
    if (tecla == GBTK_4) *opcion_menu = 3;

    dibujar_menu(*opcion_menu, ventana);

    // Confirmar
    int confirmar = (tecla == GBTK_ENTER || tecla == GBTK_ESPACIO ||
                     tecla == GBTK_1 || tecla == GBTK_2 || tecla == GBTK_3 || tecla == GBTK_4);

    if (confirmar)
    {
        if (*opcion_menu == 0)
        {
            inicializar_juego(estado_juego);
            estado_juego->velocidad_caida_ms = jug->velocidad_inicial;

            if (*timer_caida) gbt_temporizador_destruir(*timer_caida);
            *timer_fijacion = NULL;

            double seg = estado_juego->velocidad_caida_ms / 1000.0;
            *timer_caida = gbt_temporizador_crear(seg);

            *estado_app = ESTADO_JUGANDO;
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
    int cx = ventana->ancho / 2;

    int ancho_boton = ventana->es_vga ? 200 : 160;
    int alto_boton  = ventana->es_vga ?  26  :  15;
    int separacion  = ventana->es_vga ?  12  :   6;
    int x_boton     = cx - ancho_boton / 2;
    int y_inicio    = ventana->es_vga ? 165 :  85;

    static int sel = 0;

    if (*sub_menu == 0)
    {
        int max_opciones = 4;
        if (tecla == GBTK_ARRIBA) sel = (sel + max_opciones - 1) % max_opciones;
        if (tecla == GBTK_ABAJO)  sel = (sel + 1) % max_opciones;

        dibujar_texto_grande("=== AJUSTES ===", cx - 7*8, ventana->es_vga ? 60 : 20, 14);

        sprintf(buffer_texto, "Paleta: %s", jug->paleta == 0 ? "Clasica" : "Moderna");
        dibujar_texto(buffer_texto, cx - (int)(strlen(buffer_texto)*8)/2, ventana->es_vga ? 100 : 42, 7);

        sprintf(buffer_texto, "Resolucion: %s", ventana->es_vga ? "VGA (640x480)" : "CGA (320x200)");
        dibujar_texto(buffer_texto, cx - (int)(strlen(buffer_texto)*8)/2, ventana->es_vga ? 118 : 54, 7);

        sprintf(buffer_texto, "Velocidad: %.0f ms", jug->velocidad_inicial);
        dibujar_texto(buffer_texto, cx - (int)(strlen(buffer_texto)*8)/2, ventana->es_vga ? 136 : 66, 7);

        const char* opciones[4] = {"Cambiar Paleta", "Cambiar Resolucion", "Cambiar Velocidad", "Volver al Menu"};
        for (int i = 0; i < max_opciones; i++)
        {
            int y_btn = y_inicio + i * (alto_boton + separacion);
            int es_sel = (i == sel);

            int cb = es_sel ? 14 : 7;
            int ct = es_sel ? 14 : 7;

            dibujar_rectangulo(x_boton, y_btn, ancho_boton, alto_boton, cb);
            if (es_sel)
                dibujar_rectangulo(x_boton - 1, y_btn - 1, ancho_boton + 2, alto_boton + 2, cb);

            int x_txt = x_boton + (ancho_boton - (int)(strlen(opciones[i]) * 8)) / 2;
            int y_txt = y_btn + (alto_boton - 8) / 2;
            dibujar_texto(opciones[i], x_txt, y_txt, ct);

            if (es_sel)
                dibujar_texto(">", x_boton - 12, y_txt, 14);
        }

        if (tecla == GBTK_ENTER)
        {
            if      (sel == 0)
            {
                *sub_menu = 1;
                sel = 0;
            }
            else if (sel == 1)
            {
                *sub_menu = 3;
                sel = 0;
            }
            else if (sel == 2)
            {
                *sub_menu = 2;
                sel = 0;
            }
            else if (sel == 3)
            {
                *estado_app = ESTADO_MENU;
                sel = 0;
            }
        }
        if (tecla == GBTK_ESCAPE)
        {
            *estado_app = ESTADO_MENU;
            sel = 0;
        }
    }
    else if (*sub_menu == 1) //Submenu paletas
    {
        int max_opciones = 3;
        if (tecla == GBTK_ARRIBA) sel = (sel + max_opciones - 1) % max_opciones;
        if (tecla == GBTK_ABAJO)  sel = (sel + 1) % max_opciones;

        dibujar_texto_grande("=== PALETAS ===", cx - 7*8, ventana->es_vga ? 60 : 20, 14);

        const char* opciones_pal[3] = {"Paleta Clasica", "Paleta Neon", "Volver"};
        for (int i = 0; i < max_opciones; i++)
        {
            int y_btn = y_inicio + i * (alto_boton + separacion);
            int es_sel = (i == sel);

            int cb = es_sel ? 14 : ((i == 2) ? 12 : 7);
            int ct = es_sel ? 14 : ((i == 2) ? 12 : 7);

            dibujar_rectangulo(x_boton, y_btn, ancho_boton, alto_boton, cb);
            if (es_sel)
                dibujar_rectangulo(x_boton - 1, y_btn - 1, ancho_boton + 2, alto_boton + 2, cb);

            int x_txt = x_boton + (ancho_boton - (int)(strlen(opciones_pal[i]) * 8)) / 2;
            int y_txt = y_btn + (alto_boton - 8) / 2;
            dibujar_texto(opciones_pal[i], x_txt, y_txt, ct);

            if (es_sel)
                dibujar_texto(">", x_boton - 12, y_txt, 14);
        }

        if (tecla == GBTK_ENTER)
        {
            if (sel == 0 || sel == 1)
            {
                jug->paleta = sel;
                if(jug->paleta == 1) gbt_aplicar_paleta(paletaNeon, CANT_COLORES, GBT_FORMATO_888);
                else gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888);
                actualizar_o_agregar_jugador(tabla, jug);
                guardar_puntajes(tabla);
                *sub_menu = 0;
                sel = 0;
            }
            else if (sel == 2)
            {
                *sub_menu = 0;
                sel = 0;
            }
        }
        if (tecla == GBTK_ESCAPE)
        {
            *sub_menu = 0;
            sel = 0;
        }
    }
    else if (*sub_menu == 2) // Submenu velocidades
    {
        int max_opciones = 4;
        if (tecla == GBTK_ARRIBA) sel = (sel + max_opciones - 1) % max_opciones;
        if (tecla == GBTK_ABAJO)  sel = (sel + 1) % max_opciones;

        dibujar_texto_grande("=== VELOCIDAD ===", cx - 9*8, ventana->es_vga ? 60 : 20, 14);

        int y_vel_inicio = ventana->es_vga ? 165 : 85;

        const char* opciones_vel[4] = {"Lento (1500ms)", "Normal (1000ms)", "Rapido (500ms)", "Volver"};
        for (int i = 0; i < max_opciones; i++)
        {
            int y_btn = y_vel_inicio + i * (alto_boton + separacion);
            int es_sel = (i == sel);

            int cb = es_sel ? 14 : ((i == 3) ? 12 : 7);
            int ct = es_sel ? 14 : ((i == 3) ? 12 : 7);

            dibujar_rectangulo(x_boton, y_btn, ancho_boton, alto_boton, cb);
            if (es_sel)
                dibujar_rectangulo(x_boton - 1, y_btn - 1, ancho_boton + 2, alto_boton + 2, cb);

            int x_txt = x_boton + (ancho_boton - (int)(strlen(opciones_vel[i]) * 8)) / 2;
            int y_txt = y_btn + (alto_boton - 8) / 2;
            dibujar_texto(opciones_vel[i], x_txt, y_txt, ct);

            if (es_sel)
                dibujar_texto(">", x_boton - 12, y_txt, 14);
        }

        if (tecla == GBTK_ENTER)
        {
            float vel = 0;
            if      (sel == 0) vel = 1500.0f;
            else if (sel == 1) vel = 1000.0f;
            else if (sel == 2) vel = 500.0f;
            else if (sel == 3)
            {
                *sub_menu = 0;
                sel = 0;
                return;
            }

            if (vel > 0)
            {
                jug->velocidad_inicial = vel;
                *sub_menu = 0;
                sel = 0;
            }
        }
        if (tecla == GBTK_ESCAPE)
        {
            *sub_menu = 0;
            sel = 0;
        }
    }
    else if (*sub_menu == 3) //Submenu resoluciones
    {
        int max_opciones = 3;
        if (tecla == GBTK_ARRIBA) sel = (sel + max_opciones - 1) % max_opciones;
        if (tecla == GBTK_ABAJO)  sel = (sel + 1) % max_opciones;

        dibujar_texto_grande("=== RESOLUCION ===", cx - 9*8, ventana->es_vga ? 60 : 20, 14);

        const char* opciones_res[3] = {"Modo CGA (320x200)", "Modo VGA (640x480)", "Volver"};
        for (int i = 0; i < max_opciones; i++)
        {
            int y_btn = y_inicio + i * (alto_boton + separacion);
            int es_sel = (i == sel);

            int cb = es_sel ? 14 : ((i == 2) ? 12 : 7);
            int ct = es_sel ? 14 : ((i == 2) ? 12 : 7);

            dibujar_rectangulo(x_boton, y_btn, ancho_boton, alto_boton, cb);
            if (es_sel)
                dibujar_rectangulo(x_boton - 1, y_btn - 1, ancho_boton + 2, alto_boton + 2, cb);

            int x_txt = x_boton + (ancho_boton - (int)(strlen(opciones_res[i]) * 8)) / 2;
            int y_txt = y_btn + (alto_boton - 8) / 2;
            dibujar_texto(opciones_res[i], x_txt, y_txt, ct);

            if (es_sel)
                dibujar_texto(">", x_boton - 12, y_txt, 14);
        }

        if (tecla == GBTK_ENTER)
        {
            if (sel == 0 || sel == 1)
            {
                ventana->es_vga = sel;
                jug->es_vga = sel;

                reiniciar_entorno_grafico(ventana);
                if (jug->paleta == 1) gbt_aplicar_paleta(paletaNeon, CANT_COLORES, GBT_FORMATO_888);
                else gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888);

                *sub_menu = 0;
                sel = 0;
            }
            else if (sel == 2)
            {
                *sub_menu = 0;
                sel = 0;
            }
        }
        if (tecla == GBTK_ESCAPE)
        {
            *sub_menu = 0;
            sel = 0;
        }
    }
}
//--Jugando
void actualizar_jugando(EstadoAplicacion* estado_app, eGBT_Tecla tecla, EstadoJuego* estado_juego, ResolucionVentana* ventana, tGBT_Temporizador** timer_caida, tGBT_Temporizador** timer_fijacion)
{
    static int frames_abajo_mantenido = 0;

    // Logica de Pausa
    if (tecla == GBTK_ESCAPE || tecla == GBTK_p)
    {
        *estado_app = ESTADO_PAUSADO;
        return;
    }
    if (tecla == GBTK_IZQUIERDA)
        mover_pieza(estado_juego, -1, 0);

    else if (tecla == GBTK_DERECHA)
        mover_pieza(estado_juego, 1, 0);

    else if (tecla == GBTK_ARRIBA || tecla == GBTK_ESPACIO)
        rotar_pieza_actual(estado_juego, 1);

    else if (tecla == GBTK_z)
        rotar_pieza_actual(estado_juego, -1);

    if (tecla == GBTK_ABAJO)
    {
        mover_pieza(estado_juego, 0, 1);
        estado_juego->puntos++;
        frames_abajo_mantenido = 0;
    }
    else if (gbt_tecla_sostenida(GBTK_ABAJO))
    {
        frames_abajo_mantenido++;
        if (frames_abajo_mantenido >= 12 && (frames_abajo_mantenido - 12) % 3 == 0)
        {
            mover_pieza(estado_juego, 0, 1);
            estado_juego->puntos++;
        }
    }
    else
    {
        frames_abajo_mantenido = 0;
    }

    if (gbt_temporizador_consumir(*timer_caida))
    {
        if (puede_mover_pieza(estado_juego, 0, 1))
        {
            mover_pieza(estado_juego, 0, 1);
        }
        else
        {
            int piezas_antes = estado_juego->piezas_caidas;
            estado_juego->piezas_caidas++;

            fijar_pieza(estado_juego);
            borrar_lineas_completas(estado_juego);
            estado_juego->pieza_actual = estado_juego->pieza_siguiente;
            generar_nueva_pieza(estado_juego);

            // Bonus de puntos por velocidad
            long bonus_velocidad = (long)(1000.0f / estado_juego->velocidad_caida_ms * 10);
            estado_juego->puntos += bonus_velocidad;

            // Cada 10 piezas subimos la dificultad un 3%
            if ((estado_juego->piezas_caidas / 10) > (piezas_antes / 10))
            {
                recalcular_velocidad(estado_juego);
                actualizar_velocidad(estado_juego, timer_caida, timer_fijacion);
            }

            // Si la nueva pieza aparece colisionando, se declara el Game Over
            if (!puede_mover_pieza(estado_juego, 0, 0))
            {
                estado_juego->game_over = 1;
            }
        }
    }

    // Control de salida por fin de juego
    if (estado_juego->game_over)
    {
        *estado_app = ESTADO_GAMEOVER;
        return;
    }

    // Renderizado en el Backbuffer
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

    if (tecla == GBTK_ESCAPE || tecla == GBTK_ENTER)
    {
        // Actualiza mejor puntaje en caso de corresponder
        if (estado_juego->puntos > jug->mejor_puntaje)
        {
            jug->mejor_puntaje = estado_juego->puntos;
        }

        // Solo guardar en el ranking si hizo mas de 0 puntos
        if (jug->mejor_puntaje > 0)
        {
            actualizar_o_agregar_jugador(tabla, jug);
            guardar_puntajes(tabla);
        }

        if(tecla == GBTK_ENTER)
        {
            inicializar_juego(estado_juego);
            *estado_app = ESTADO_JUGANDO;
        }
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
