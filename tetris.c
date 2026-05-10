#include "tetris.h"

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


void dibujar(const uint8_t dibujo[][PIXELES_X_LADO], uint16_t oX, uint16_t oY)
{
    uint16_t offsetX = oX * (PIXELES_X_LADO + PX_PADDING);
    uint16_t offsetY = oY * (PIXELES_X_LADO + PX_PADDING);

    for (uint16_t y = 0; y < PIXELES_X_LADO; y++) {
        for (uint16_t x = 0; x < PIXELES_X_LADO; x++) {

            gbt_dibujar_pixel(offsetX + x, offsetY + y, dibujo[y][x]);
        }
    }
}

void rotar(int origen[4][4], int destino[4][4])
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            destino[j][3 - i] = origen[i][j];
        }
    }
}
