#include <stdio.h>
#include <stdlib.h>
#include "GBT/gbt.h"
#include "tetris.h"
#include <time.h>
#include <stdint.h>

#define ANCHO_VENTANA 128
#define ALTO_VENTANA 128
#define ESCALA_VENTANA 5
#define CANT_COLORES 16
#define TAM_GRILLA 11

#define FILAS_VISIBLES 20
#define FILAS_OCULTAS 4
#define COLUMNAS 10



const uint8_t sonrisa[8][8] =
{
    {N, N, A, A, A, A, N, N},
    {N, A, A, A, A, A, A, N},
    {A, A, N, A, A, N, A, A},
    {A, A, A, A, A, A, A, A},
    {A, A, A, A, A, A, A, A},
    {A, N, A, A, A, A, N, A},
    {N, A, N, N, N, N, A, N},
    {N, N, A, A, A, A, N, N}
};

const uint8_t triste[8][8] =
{
    {N, N, C, C, C, C, N, N},
    {N, C, C, C, C, C, C, N},
    {C, C, N, C, C, N, C, C},
    {C, C, C, C, C, C, C, C},
    {C, C, N, N, N, N, C, C},
    {C, N, C, C, C, C, N, C},
    {N, C, C, C, C, C, C, N},
    {N, N, C, C, C, C, N, N}

};

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

int main(int argc, char* argv[])
{
    int tablero[FILAS_VISIBLES + FILAS_OCULTAS][COLUMNAS];


    int matI[4][4]= {{0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {1,1,1,1}
    };

    int matJ[4][4]= {{0,0,0,0},
        {0,0,0,0},
        {1,0,0,0},
        {1,1,1,0}
    };

    int matL[4][4]= {{0,0,0,0},
        {0,0,0,0},
        {0,0,0,1},
        {0,1,1,1}
    };

    int mat0[4][4]= {{0,0,0,0},
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0}
    };

    int matS[4][4]= {{0,0,0,0},
        {0,0,1,1},
        {0,1,1,0},
        {0,0,0,0}
    };

    int matT[4][4]= {{0,0,0,0},
        {0,0,0,0},
        {0,1,0,0},
        {1,1,1,0}
    };

    int matZ[4][4]= {{0,0,0,0},
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0}
    };































    if (gbt_iniciar() != 0)
    {
        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    char nombreVentana[128];
    sprintf(nombreVentana, "Ventana %dx%d", ANCHO_VENTANA, ALTO_VENTANA);

    if (gbt_crear_ventana(nombreVentana, ANCHO_VENTANA, ALTO_VENTANA, ESCALA_VENTANA) != 0)
    {
        fprintf(stderr, "Error al iniciar el modulo de graficos de GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    if (gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888) != 0)
    {
        fprintf(stderr, "Error al aplicar la nueva paleta de colores: %s\n", gbt_obtener_log());
        return -1;
    }

    tGBT_Temporizador *temporizador = gbt_temporizador_crear(1.0);
    if (!temporizador)
    {
        fprintf(stderr, "Error al crear el temporizador para los dibujos: %s\n", gbt_obtener_log());
        return -1;
    }

    srand(time(0));

    uint8_t corriendo = 1;
    uint16_t offsetX =  0;
    uint16_t offsetY = 0;

    printf("Pulse 'A' para borrar la pantalla con un color aleatorio\n"
           "Pulse 'D' para colocar una cara aleatoria en un lugar aleatorio\n");

    while (corriendo)
    {

        gbt_procesar_entrada();
        eGBT_Tecla tecla = gbt_obtener_tecla_presionada();

        if (tecla == GBTK_ESCAPE)
        {

            corriendo = 0;
            printf("Saliendo del ejemplo\n");
        }
        else if (tecla != GBTK_DESCONOCIDA)
        {

            offsetX = rand() % TAM_GRILLA;
            offsetY = rand() % TAM_GRILLA;

            if (tecla == GBTK_d)
            {

                printf("Pulsaste 'D' poniendo un dibujo en la posicion aleatoria [%d,%d]\n", offsetX, offsetY);
                dibujar(rand() & 1 ? sonrisa : triste, offsetX, offsetY);
            }
            else if (tecla == GBTK_a)
            {

                uint8_t color = rand() % CANT_COLORES;
                printf("Pulsaste 'A' borrando la pantalla con el indice de color: %d\n", color);
                gbt_borrar_backbuffer(color);
            }
        }

        if (gbt_temporizador_consumir(temporizador))
        {

            offsetX = rand() % TAM_GRILLA;
            offsetY = rand() % TAM_GRILLA;
            printf("Se cumplio el temporizador poniendo un dibujo en la posicion aleatoria [%d,%d]\n", offsetX, offsetY);
            dibujar(rand() & 1 ? sonrisa : triste, offsetX, offsetY);
        }

        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }

    gbt_temporizador_destruir(temporizador);
    gbt_destruir_ventana();
    gbt_cerrar();


    return 0;
}
