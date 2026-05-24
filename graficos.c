#include "graficos.h"
#include "font8x8.h" // array: font8x8_basic[128][8]
#include "font8x16.h" // array: font8x16_basic[128][16]
#include <string.h>
#include <stdio.h>

//-- Colores por tipo de pieza
const int COLORES_PIEZAS[7] =
{
    3,   // 0: Pieza I (Cian)
    1,   // 1: Pieza J (Azul)
    6,   // 2: Pieza L (Naranja/Marron)
    14,  // 3: Pieza O (Amarillo)
    2,   // 4: Pieza S (Verde)
    5,   // 5: Pieza T (Magenta/Morado)
    4    // 6: Pieza Z (Rojo)
};

ResolucionVentana* g_ventana = NULL;

//--Inicialización

int inicializar_graficos(ResolucionVentana* ventana, int escala, int es_vga)
{
    if (gbt_iniciar() != 0)
    {
        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    ventana->ancho = es_vga ? 640 : 320;
    ventana->alto = es_vga ? 480 : 200;
    ventana->tamano_bloque = es_vga ? 16 : 8;
    ventana->escala = escala;
    ventana->es_vga = es_vga;

    sprintf(ventana->nombreVentana, "Tetris - %s %dx%d (escala %d)", es_vga ? "VGA" : "CGA", ventana->ancho, ventana->alto, escala);

    if (gbt_crear_ventana(ventana->nombreVentana, ventana->ancho, ventana->alto, ventana->escala) != 0)
    {
        fprintf(stderr, "Error al crear la ventana: %s\n", gbt_obtener_log());
        return -1;
    }

    g_ventana = ventana;
    return 0;
}

void dibujar_bloque_cuadrado(ResolucionVentana *ventana, int x_pantalla, int y_pantalla, int color)
{
    for(int i = 0; i < ventana->tamano_bloque; i++)
    {
        for(int j = 0; j < ventana->tamano_bloque; j++)
        {
            gbt_dibujar_pixel(x_pantalla + j, y_pantalla + i, color);
        }
    }
}

void dibujar_rectangulo(int x, int y, int ancho, int alto, int color)
{
    for (int i = 0; i < ancho; i++)
    {
        gbt_dibujar_pixel(x + i, y, color);
        gbt_dibujar_pixel(x + i, y + alto - 1, color);
    }
    // Bordes verticales
    for (int j = 0; j < alto; j++)
    {
        gbt_dibujar_pixel(x, y + j, color);
        gbt_dibujar_pixel(x + ancho - 1, y + j, color);
    }
}

void dibujar_caracter(char c, int oX, int oY, int color)
{
    if (c < 0 || c >= 128) return;

    for (int fila = 0; fila < 8; fila++)
    {
        for (int col = 0; col < 8; col++)
        {
            int pixel_encendido = (font8x8_basic[(int)c][fila] >> col) & 1;
            if (pixel_encendido)
            {
                gbt_dibujar_pixel(oX + col, oY + fila, color);
            }
        }
    }
}

void dibujar_caracter_grande(char c, int oX, int oY, int color)
{
    if (c < 0 || c >= 128) return;

    for (int fila = 0; fila < 16; fila++)
    {
        for (int col = 0; col < 8; col++)
        {
            int pixel_encendido = (font8x16_basic[(int)c][fila] >> col) & 1;
            if (pixel_encendido)
            {
                gbt_dibujar_pixel(oX + col, oY + fila, color);
            }
        }
    }
}

void dibujar_texto(const char* texto, int oX, int oY, int color)
{
    int x_actual = oX;

    while (*texto)
    {
        dibujar_caracter(*texto, x_actual, oY, color);
        x_actual += 8; // Avanzamos 8 pixeles a la derecha para el siguiente caracter
        texto++;
    }
}

void dibujar_texto_grande(const char* texto, int oX, int oY, int color)
{
    int x_actual = oX;

    while (*texto)
    {
        dibujar_caracter_grande(*texto, x_actual, oY, color);
        x_actual += 8;
        texto++;
    }
}

//--Menu con selector visual
void dibujar_menu(int opcion_seleccionada)
{
    if (!g_ventana) return;

    int cx = g_ventana->ancho / 2;   // centro horizontal
    int ancho_boton = g_ventana->es_vga ? 160 : 100;
    int alto_boton  = g_ventana->es_vga ?  28  :  16;
    int separacion  = g_ventana->es_vga ?  12  :   6;
    int x_boton     = cx - ancho_boton / 2;
    int y_inicio    = g_ventana->es_vga ? 160 :  70;

    // Título
    const char* titulo = "=== TETRIS ===";
    int x_titulo = cx - (int)(strlen(titulo) * 8) / 2;
    dibujar_texto_grande(titulo, x_titulo, g_ventana->es_vga ? 60 : 20, 14);

    const char* etiquetas[4] = {"  JUGAR  ", " AJUSTES ", " RANKING ", "  SALIR  "};
    // Colores: seleccionado=amarillo/blanco, normal=gris/gris oscuro
    int color_borde_sel = 14; // Amarillo
    int color_texto_sel = 15; // Blanco
    int color_borde_nor =  7; // Gris claro
    int color_texto_nor =  8; // Gris oscuro

    for (int i = 0; i < 4; i++)
    {
        int y_boton = y_inicio + i * (alto_boton + separacion);

        int cb = (i == opcion_seleccionada) ? color_borde_sel : color_borde_nor;
        int ct = (i == opcion_seleccionada) ? color_texto_sel : color_texto_nor;

        // Borde doble para el seleccionado
        dibujar_rectangulo(x_boton, y_boton, ancho_boton, alto_boton, cb);
        if (i == opcion_seleccionada)
            dibujar_rectangulo(x_boton - 1, y_boton - 1, ancho_boton + 2, alto_boton + 2, cb);

        dibujar_rectangulo(x_boton, y_boton, ancho_boton, alto_boton, cb);
        // Texto centrado dentro del boton
        int x_texto = x_boton + (ancho_boton - (int)(strlen(etiquetas[i]) * 8)) / 2;
        int y_texto = y_boton + (alto_boton - 8) / 2;
        dibujar_texto(etiquetas[i], x_texto, y_texto, ct);

        // Flecha indicadora
        if (i == opcion_seleccionada)
            dibujar_texto(">", x_boton - 12, y_texto, 14);
    }

    // Instrucciones al pie
    const char* ayuda = "Flechas + Enter   o   1/2/3";
    int x_ayuda = cx - (int)(strlen(ayuda) * 8) / 2;
    dibujar_texto(ayuda, x_ayuda, g_ventana->es_vga ? 380 : 170, 8);
}

//--Pantalla en Pausa
void dibujar_pantalla_pausa(ResolucionVentana* ventana)
{
    int cx = ventana->ancho / 2;
    int cy = ventana->alto  / 2;
    int sep = ventana->es_vga ? 14 : 10;

    const char* lineas[] =
    {
        "==================",
        "     EN PAUSA     ",
        "==================",
        " Enter/P: Continuar",
        "  ESC:    Menu    "
    };
    int n = 5;
    int y_base = cy - (n * sep) / 2;

    for (int i = 0; i < n; i++)
    {
        int x = cx - (int)(strlen(lineas[i]) * 8) / 2;
        int color = (i == 1) ? 11 : (i >= 3 ? 7 : 11);
        dibujar_texto(lineas[i], x, y_base + i * sep, color);
    }
}

//--Tablero
void dibujar_tablero(EstadoJuego *estado, ResolucionVentana *ventana)
{
    int ancho_tablero = COLUMNAS * (ventana->tamano_bloque + PX_PADDING);
    int alto_tablero = FILAS_VISIBLES * (ventana->tamano_bloque + PX_PADDING);

    // Centrado basado en la resolucion activa
    int offset_x = (ventana->ancho - ancho_tablero) / 2;
    int offset_y = (ventana->alto - alto_tablero) / 2;


    int color_borde = 12;

    // Recorremos verticalmente todo el alto del tablero visible
    for (int y = 0; y < alto_tablero; y++)
    {
        // Pared Izquierda: Un pixel a la izquierda del offset de inicio
        gbt_dibujar_pixel(offset_x - 1, offset_y + y, color_borde);

        // Pared Derecha: Un pixel a la derecha del ancho total del tablero
        gbt_dibujar_pixel(offset_x + ancho_tablero, offset_y + y, color_borde);
    }

    // Va desde la pared izquierda (-1) hasta la pared derecha (ancho_tablero)
    for (int x = -1; x <= ancho_tablero; x++)
    {
        gbt_dibujar_pixel(offset_x + x, offset_y + alto_tablero, color_borde);
    }

    for (int y = FILAS_OCULTAS; y < FILAS_TOTALES; y++)
    {
        for (int x = 0; x < COLUMNAS; x++)
        {
            int color = estado->tablero[y][x];

            if (color != 0)
            {
                int px = offset_x + x * (ventana->tamano_bloque + PX_PADDING);
                int py = offset_y + (y - FILAS_OCULTAS) * (ventana->tamano_bloque + PX_PADDING);

                dibujar_bloque_cuadrado(ventana, px, py, color);
            }
        }
    }
}

//--Pieza activa
void dibujar_pieza(EstadoJuego *estado, ResolucionVentana *ventana)
{
    Tetromino *pieza = &estado->pieza_actual;

    int offset_x = (ventana->ancho - (COLUMNAS * (ventana->tamano_bloque + PX_PADDING))) / 2;
    int offset_y = (ventana->alto - (FILAS_VISIBLES * (ventana->tamano_bloque + PX_PADDING))) / 2;
    int color = COLORES_PIEZAS[pieza->tipo];

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (pieza->forma[y][x])
            {
                int tablero_x = pieza->x + x;
                int tablero_y = pieza->y + y;

                if (tablero_y >= FILAS_OCULTAS)   // Solo dibujar si esta en area visible
                {
                    int px = offset_x + tablero_x * (ventana->tamano_bloque + PX_PADDING);
                    int py = offset_y + (tablero_y - FILAS_OCULTAS) * (ventana->tamano_bloque + PX_PADDING);

                    dibujar_bloque_cuadrado(ventana, px,py, color);
                }
            }
        }
    }
}

//-- UI (Estadisticas + siguiente pieza + velocidad)
void dibujar_ui(EstadoJuego *estado, ResolucionVentana *ventana)
{
    char buffer[64];

    // Configuracion dinamica de coordenadas segun el Modo Grafico
    int x_izquierda = ventana->es_vga ? 20 : 5;
    int x_derecha   = ventana->es_vga ? 490 : 228;

    int y_titulo    = ventana->es_vga ? 20 : 5;
    int y_puntos    = ventana->es_vga ? 60 : 25;
    int y_lineas    = ventana->es_vga ? 90 : 38;
    int y_nivel     = ventana->es_vga ? 120 : 51;
    int y_vel       = ventana->es_vga ? 150 : 64;

    // 1. Dibujamos Textos de Estadisticas
    dibujar_texto("TETRIS", x_izquierda, y_titulo, 14); // Amarillo

    sprintf(buffer, "Puntos: %ld", estado->puntos);
    dibujar_texto(buffer, x_izquierda, y_puntos, 7); // Gris claro

    sprintf(buffer, "Lineas: %d", estado->lineas);
    dibujar_texto(buffer, x_izquierda, y_lineas, 7);

    // Calculamos nivel dinamicamente
    sprintf(buffer, "Nivel: %d", (estado->piezas_caidas / 10) + 1);
    dibujar_texto(buffer, x_izquierda, y_nivel, 7);

    // Velocidad actual expresada en ms
    sprintf(buffer, "Vel:%dms", (int)estado->velocidad_caida_ms);
    dibujar_texto(buffer, x_izquierda, y_vel, 11);

    // 2. Dibujar Proxima Pieza (Previsualizacion)
    dibujar_texto("Siguiente:", x_derecha, y_puntos, 7);

    int p_offset_x = x_derecha;
    int p_offset_y = ventana->es_vga ? y_puntos + 30 : y_puntos + 15;
    int color = COLORES_PIEZAS[estado->pieza_siguiente.tipo];

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (estado->pieza_siguiente.forma[y][x])
            {
                // Calculamos pixeles usando los datos de estructura "ventana"
                int px = p_offset_x + x * (ventana->tamano_bloque + PX_PADDING);
                int py = p_offset_y + y * (ventana->tamano_bloque + PX_PADDING);

                // Reutilizamos funcion para pintar el bloque de la pieza
                dibujar_bloque_cuadrado(ventana, px, py, color);
            }
        }
    }

    // Controles al pie (lado izquierdo)
    int y_ctrl = ventana->es_vga ? 380 : 80;
    dibujar_texto("Z: rotar izq.", x_izquierda, y_ctrl,      8);
    dibujar_texto("^: rotar der.", x_izquierda, y_ctrl + 14, 8);
    dibujar_texto("P/Esc: Pausa",      x_izquierda, y_ctrl + 28, 8);
}

//--Game Over
void dibujar_interfaz_game_over(EstadoJuego* estado, ResolucionVentana* ventana, Jugador* jug)
{
    dibujar_tablero(estado, ventana);

    // Posicionar a la derecha en lugar del centro absoluto
    int cx       = ventana->es_vga ? 520 : 255;
    int cy       = ventana->alto  / 2;
    int sep      = 14;

    char buf_pts[32], buf_lin[32], buf_niv[32], buf_mejor[32];
    sprintf(buf_pts, "Puntos: %ld", estado->puntos);
    sprintf(buf_lin, "Lineas: %d",  estado->lineas);
    sprintf(buf_niv, "Nivel:  %d",  (estado->piezas_caidas / 10) + 1);
    sprintf(buf_mejor, "Record: %ld", jug->mejor_puntaje);

    const char* lineas[] =
    {
        "  GAME OVER  ",
        buf_pts,
        buf_lin,
        buf_niv,
        buf_mejor,
        " Enter: Menu "
    };
    int n = 6;

    int y_base = cy - (n * sep) / 2;
    for (int i = 0; i < n; i++)
    {
        int color = (i == 0) ? 12 : 7;
        if (i == n - 1) color = 11;
        int x = cx - (int)(strlen(lineas[i]) * 8) / 2;
        dibujar_texto(lineas[i], x, y_base + i * sep, color);
    }
}

// Jugador
int actualizar_ingreso_nombre(eGBT_Tecla tecla, Jugador* jug, ResolucionVentana* ventana)
{
    int cx = ventana->ancho / 2;
    int cy = ventana->alto  / 2;

    // Formulario
    dibujar_texto_grande("=== TETRIS ===", cx - 7*8, cy - 50, 14);
    dibujar_texto("Ingresa tu nombre:", cx - 9*8, cy - 20, 7);

    char display[36];
    sprintf(display, "[ %-16s]", jug->nombre);
    dibujar_texto(display, cx - 9*8, cy, 14);

    dibujar_texto("Enter: Confirmar", cx - 8*8, cy + 20, 8);
    dibujar_texto("Backspace: Borrar", cx - 8*8, cy + 32, 8);

    int len = strlen(jug->nombre);

    // Letras minúsculas
    if (tecla >= 'a' && tecla <= 'z' && len < 16)
    {
        jug->nombre[len]     = (char)(tecla - 'a' + 'A');
        jug->nombre[len + 1] = '\0';
    }
    // Letras mayúsculas (por si el SO envía mayúsculas directo)
    else if (tecla >= 'A' && tecla <= 'Z' && len < 16)
    {
        jug->nombre[len]     = (char)tecla;
        jug->nombre[len + 1] = '\0';
    }
    // Números
    else if (tecla >= '0' && tecla <= '9' && len < 16)
    {
        jug->nombre[len]     = (char)tecla;
        jug->nombre[len + 1] = '\0';
    }
    // Espacio
    else if (tecla == ' ' && len < 16)
    {
        jug->nombre[len]     = '_';
        jug->nombre[len + 1] = '\0';
    }

    // Retroceso (Backspace es ASCII 8)
    if (tecla == '\b' && len > 0)
        jug->nombre[len - 1] = '\0';

    // Enter (ASCII 13)
    if (tecla == '\r' && len > 0)
        return 1;

    return 0;
}

void dibujar_estadisticas(TablaPuntajes* t, ResolucionVentana* ventana)
{
    int cx = ventana->ancho / 2;
    dibujar_texto_grande("=== MEJORES PUNTAJES ===", cx - 12*8, ventana->es_vga ? 60 : 20, 11);

    int y_base = ventana->es_vga ? 100 : 45;
    for(int i=0; i < t->cantidad; i++) {
        char buffer[64];
        sprintf(buffer, "%d. %-15s %ld", i+1, t->jugadores[i].nombre, t->jugadores[i].mejor_puntaje);
        dibujar_texto(buffer, cx - 13*8, y_base + i * (ventana->es_vga ? 20 : 10), 14);
    }

    if(t->cantidad == 0) {
        dibujar_texto("No hay estadisticas aun", cx - 11*8, y_base, 7);
    }

    dibujar_texto("ESC: Volver al menu", cx - 9*8, ventana->es_vga ? 400 : 180, 8);
}

