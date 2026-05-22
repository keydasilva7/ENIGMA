#include "graficos.h"
#include "font8x8.h" // array: font8x8_basic[128][8]
#include <string.h>
#include <stdio.h>


const int COLORES_PIEZAS[7] =
{
    3,   // 0: Pieza I (Cian)
    1,   // 1: Pieza J (Azul)
    6,   // 2: Pieza L (Naranja/Marrón)
    14,  // 3: Pieza O (Amarillo)
    2,   // 4: Pieza S (Verde)
    5,   // 5: Pieza T (Magenta/Morado)
    4    // 6: Pieza Z (Rojo)
};

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

    sprintf(ventana->nombreVentana, "Tetris - %dx%d", ventana->ancho, ventana->alto);

    if (gbt_crear_ventana(ventana->nombreVentana, ventana->ancho, ventana->alto, ventana->escala) != 0)
    {
        fprintf(stderr, "Error al crear la ventana: %s\n", gbt_obtener_log());
        return -1;
    }

    // Asegurarse de que el color N o 15 se interpreta como lo que deba ser
    // (Por defecto el profesor usaba una paleta CGA que esta en main.c)
    // El setup de la paleta se hara desde el main.c
    return 0;
}

void dibujar_bloque_cuadrado(ResolucionVentana *ventana, int x_pantalla, int y_pantalla, int color)
{
    for(int i=0; i< ventana->tamano_bloque; i++)
    {
        for(int j=0; j<ventana->tamano_bloque; j++)
        {
            gbt_dibujar_pixel(x_pantalla + j, y_pantalla + i, color);
        }
    }
}

void dibujar_tablero(EstadoJuego *estado, ResolucionVentana *ventana)
{

    int ancho_tablero = COLUMNAS * (ventana->tamano_bloque + PX_PADDING);
    int alto_tablero = FILAS_VISIBLES * (ventana->tamano_bloque + PX_PADDING);

    // Centrado basado en la resolución activa
    int offset_x = (ventana->ancho - ancho_tablero) / 2;
    int offset_y = (ventana->alto - alto_tablero) / 2;


    int color_borde = 12;

    // Recorremos verticalmente todo el alto del tablero visible
    for (int y = 0; y < alto_tablero; y++)
    {
        // Pared Izquierda: Un píxel a la izquierda del offset de inicio
        gbt_dibujar_pixel(offset_x - 1, offset_y + y, color_borde);

        // Pared Derecha: Un píxel a la derecha del ancho total del tablero
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

void dibujar_caracter(char c, uint16_t oX, uint16_t oY, uint8_t color)
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

void dibujar_texto(const char* texto, uint16_t oX, uint16_t oY, uint8_t color)
{
    uint16_t x_actual = oX;

    while (*texto)
    {
        dibujar_caracter(*texto, x_actual, oY, color);
        x_actual += 8; // Avanzamos 8 píxeles a la derecha para el siguiente carácter
        texto++;
    }
}

void dibujar_ui(EstadoJuego *estado, ResolucionVentana *ventana)
{
    char buffer[64];

    // Configuración dinámica de coordenadas según el Modo Gráfico
    int x_izquierda = ventana->es_vga ? 20 : 10;
    int x_derecha   = ventana->es_vga ? 480 : 220;

    int y_titulo    = ventana->es_vga ? 20 : 10;
    int y_puntos    = ventana->es_vga ? 60 : 30;
    int y_lineas    = ventana->es_vga ? 90 : 45;
    int y_nivel     = ventana->es_vga ? 120 : 60;

    // 1. Dibujar Textos de Estadísticas
    dibujar_texto("TETRIS", x_izquierda, y_titulo, 14); // Amarillo

    sprintf(buffer, "Puntos: %ld", estado->puntos);
    dibujar_texto(buffer, x_izquierda, y_puntos, 7); // Gris claro

    sprintf(buffer, "Lineas: %d", estado->lineas);
    dibujar_texto(buffer, x_izquierda, y_lineas, 7);

    // Tu estructura maneja piezas_caidas, calculamos nivel dinámicamente
    sprintf(buffer, "Nivel: %d", (estado->piezas_caidas / 10) + 1);
    dibujar_texto(buffer, x_izquierda, y_nivel, 7);

    // 2. Dibujar Próxima Pieza (Previsualización)
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
                // Calculamos píxeles usando los datos de tu estructura "ventana"
                int px = p_offset_x + x * (ventana->tamano_bloque + PX_PADDING);
                int py = p_offset_y + y * (ventana->tamano_bloque + PX_PADDING);

                // Reutilizamos tu función para pintar el bloque de la pieza
                dibujar_bloque_cuadrado(ventana, px, py, color);
            }
        }
    }
}

void dibujar_interfaz_game_over(EstadoJuego* estado, ResolucionVentana* ventana)
{
    // 1. Dibujamos el tablero de fondo estático para que se vea dónde perdió
    dibujar_tablero(estado, ventana);

    // 2. Calculamos los centros en base a la resolución activa de la ventana
    int y_centro = ventana->alto / 2;
    int x_cartel = (ventana->ancho - (19 * 8)) / 2;

    int x_info   = (ventana->ancho - (33 * 8)) / 2;

    // 3. Imprimimos el cartel con color Rojo brillante (12) e instrucciones en Gris claro (7)
    dibujar_texto("===================", x_cartel, y_centro - 20, 12);
    dibujar_texto("    GAME OVER      ", x_cartel, y_centro, 12);
    dibujar_texto("===================", x_cartel, y_centro + 20, 12);
    dibujar_texto("Presione Espacio / ESC para volver", x_info, y_centro + 50, 7);
}
