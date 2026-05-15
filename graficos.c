#include "graficos.h"
#include "font8x8.h" // array: font8x8_basic[128][8]
#include <string.h>


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

int inicializar_graficos(int escala, int es_vga)
{
    if (gbt_iniciar() != 0)
    {
        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    int ancho = es_vga ? 640 : 320;
    int alto = es_vga ? 480 : 200;

    char nombreVentana[128];
    sprintf(nombreVentana, "Tetris - %dx%d", ancho, alto);

    if (gbt_crear_ventana(nombreVentana, ancho, alto, escala) != 0)
    {
        fprintf(stderr, "Error al crear la ventana: %s\n", gbt_obtener_log());
        return -1;
    }

    // Asegurarse de que el color N o 15 se interpreta como lo que deba ser
    // (Por defecto el profesor usaba una paleta CGA que esta en main.c)
    // El setup de la paleta se hara desde el main.c
    return 0;
}

void dibujar_bloque_cuadrado(int x_pantalla, int y_pantalla, int color)
{
    for(int i=0; i<PIXELES_X_LADO; i++)
    {
        for(int j=0; j<PIXELES_X_LADO; j++)
        {
            gbt_dibujar_pixel(x_pantalla + j, y_pantalla + i, color);
        }
    }
}

void dibujar_pieza(EstadoJuego *estado)
{
    Tetromino *pieza = &estado->pieza_actual;

    int offset_x = (320 - (COLUMNAS * (PIXELES_X_LADO + PX_PADDING))) / 2;
    int offset_y = (200 - (FILAS_VISIBLES * (PIXELES_X_LADO + PX_PADDING))) / 2;
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
                    int px = offset_x + tablero_x * (PIXELES_X_LADO + PX_PADDING);
                    int py = offset_y + (tablero_y - FILAS_OCULTAS) * (PIXELES_X_LADO + PX_PADDING);

                    dibujar_bloque_cuadrado(px, py, color);
                }
            }
        }
    }
}

