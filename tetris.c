#include "tetris.h"

int matI[4][4]=
{
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {1,1,1,1}
};

int matJ[4][4]=
{
    {0,0,0,0},
    {0,0,0,0},
    {1,0,0,0},
    {1,1,1,0}
};

int matL[4][4]=
{
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,1},
    {0,1,1,1}
};

int matO[4][4]=
{
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}
};

int matS[4][4]=
{
    {0,0,0,0},
    {0,0,1,1},
    {0,1,1,0},
    {0,0,0,0}
};

int matT[4][4]=
{
    {0,0,0,0},
    {0,0,0,0},
    {0,1,0,0},
    {1,1,1,0}
};

int matZ[4][4]=
{
    {0,0,0,0},
    {1,1,0,0},
    {0,1,1,0},
    {0,0,0,0}
};

void inicializar_juego(EstadoJuego* estado)
{
    inicializar_tablero(estado);
    inicializar_estructura(estado);
    generar_nueva_pieza(estado); // Crea la pieza que cae ahora
    generar_nueva_pieza(estado);
}

void inicializar_tablero(EstadoJuego* estado)
{
    int i, j;
    for (i = 0; i < FILAS_TOTALES; i++)
    {
        for (j = 0; j < COLUMNAS; j++)
        {
            estado->tablero[i][j] = 0;
        }
    }
}

void inicializar_estructura(EstadoJuego* estado)
{
    estado->puntos = 0;
    estado->lineas = 0;
    estado->piezas_caidas = 0;
    estado->velocidad_caida_ms = 1000; // 1 segundo inicial
    estado->game_over = 0;
}

void copiar_pieza(int mat_forma[4][4], int mat_letra[4][4])
{
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            mat_forma[i][j] = mat_letra[i][j];
        }
    }
}

int rotar_pieza_actual(EstadoJuego* estado, int direccion)
{
    Tetromino aux = estado->pieza_actual; // Hacemos una copia para probar
    int nueva_forma[4][4];

    rotar_matriz(aux.forma, nueva_forma, direccion > 0);

    // Asignar nueva forma a la copia
    for(int y=0; y<4; y++)
    {
        for(int x=0; x<4; x++)
        {
            aux.forma[y][x] = nueva_forma[y][x];
        }
    }

    if (!colision(estado, &aux))
    {
        estado->pieza_actual = aux;
        return 1;
    }
    return 0;
}

void dibujar(const uint8_t dibujo[][PIXELES_X_LADO], uint16_t oX, uint16_t oY)
{
    uint16_t offsetX = oX * (PIXELES_X_LADO + PX_PADDING);
    uint16_t offsetY = oY * (PIXELES_X_LADO + PX_PADDING);

    for (uint16_t y = 0; y < PIXELES_X_LADO; y++)
    {
        for (uint16_t x = 0; x < PIXELES_X_LADO; x++)
        {

            gbt_dibujar_pixel(offsetX + x, offsetY + y, dibujo[y][x]);
        }
    }
}

int puede_mover_pieza (EstadoJuego* estado, int movimiento_en_x, int movimiento_en_y)
{
    Tetromino* p= &estado->pieza_actual;//Puntero a pieza actual

    int nuevaX, nuevaY;

    for(int i=0; i < 4; i++)
    {
        for(int j=0; j < 4; j++)
        {
            if(p->forma[i][j])//si hay un bloque
            {
                //calculamos futura posicion
                nuevaX = p->x + j + movimiento_en_x;
                nuevaY = p->y + i + movimiento_en_y;

                if(nuevaX<0)//verificamos borde izquierdo
                    return 0;

                if(nuevaX>=COLUMNAS)//verificamos borde derecho
                    return 0;

                if(nuevaY>=FILAS_TOTALES)//verificamos fondo
                    return 0;

                if(estado->tablero[nuevaY][nuevaX])//verificamos colision (si ya hay un bloque en esa posicion)
                    return 0;

            }
        }
    }

    return 1; //no hubo problema, puede moverse la pieza
}

void mover_pieza(EstadoJuego* estado, int movimiento_en_x, int movimiento_en_y)
{
    if(puede_mover_pieza(estado, movimiento_en_x, movimiento_en_y))
    {
        estado->pieza_actual.x += movimiento_en_x;
        estado->pieza_actual.y += movimiento_en_y;
    }
}

void dibujar_pieza(EstadoJuego* estado)
{
    Tetromino *p = &estado->pieza_actual;
    int fila, columna, pixelX, pixelY;

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            if(p->forma[i][j])//¿hay bloque?
            {
                //calculamos la pos real en el tablero
                fila=p->y+i;
                columna=p->x+j;

                //convertimos de tablero a pixeles
                pixelX = columna * (PIXELES_X_LADO + PX_PADDING);
                pixelY = fila * (PIXELES_X_LADO + PX_PADDING);

                //dibujamos el bloque
                for(int y=0; y<PIXELES_X_LADO; y++)
                {
                    for(int x=0; x<PIXELES_X_LADO; x++)
                    {
                        gbt_dibujar_pixel(pixelX+x, pixelY+y, V);//dibujamos el pixel en pantalla
                    }
                }
            }
        }
    }

}

void fijar_pieza(EstadoJuego* estado)
{
    Tetromino *p=&estado->pieza_actual;

    int fila, columna;

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4 ; j++)
        {
            if(p->forma[i][j])
            {
                fila=p->y+i;
                columna=p->x+j; //Para saber la posicion real en el tablero

                estado->tablero[fila][columna]=1; //Pintamos tablero
            }
        }
    }


}

void generar_nueva_pieza(EstadoJuego* estado)
{
    int nueva_pieza_tipo;

    estado->pieza_actual = estado->pieza_siguiente;

    // Configuramos la posición en el tablero de la pieza actual
    estado->pieza_actual.x = 4;
    estado->pieza_actual.y = 0;

    nueva_pieza_tipo = rand()%7;
    estado->pieza_siguiente.tipo = nueva_pieza_tipo;

    switch(nueva_pieza_tipo)
    {
    case 0:
        copiar_pieza(estado->pieza_siguiente.forma, matI);
        break;
    case 1:
        copiar_pieza(estado->pieza_siguiente.forma, matJ);
        break;
    case 2:
        copiar_pieza(estado->pieza_siguiente.forma, matL);
        break;
    case 3:
        copiar_pieza(estado->pieza_siguiente.forma, matO);
        break;
    case 4:
        copiar_pieza(estado->pieza_siguiente.forma, matS);
        break;
    case 5:
        copiar_pieza(estado->pieza_siguiente.forma, matT);
        break;
    case 6:
        copiar_pieza(estado->pieza_siguiente.forma, matZ);
        break;
    }

}

void borrar_lineas_completas(EstadoJuego* estado)
{
    int lineas_borradas_ahora = 0;


    for (int fila = FILAS_TOTALES - 1; fila >= 0; fila--)// Recorremos el tablero de abajo hacia arriba
    {
        // Verificamos si la fila actual está completamente llena

        int fila_llena = 1;
        for (int col = 0; col < COLUMNAS; col++)
        {
            if (estado->tablero[fila][col] == 0) // Encontramos un espacio vacío
            {
                fila_llena = 0;
                break;          // Dejamos de revisar esta fila
            }
        }


        if (fila_llena)
        {
            lineas_borradas_ahora++;

            // Hacer caer todas las filas que están por encima de la que borramos
            for (int fila_arriba = fila; fila_arriba > 0; fila_arriba--)
            {
                for (int col = 0; col < COLUMNAS; col++)
                {
                    // Copiamos el color de la celda de arriba en la celda actual
                    estado->tablero[fila_arriba][col] = estado->tablero[fila_arriba - 1][col];
                }
            }

            // La fila superior (índice 0) ya no tiene a nadie arriba, se vacía (se llena de ceros)
            for (int col = 0; col < COLUMNAS; col++)
            {
                estado->tablero[0][col] = 0;
            }

            // ¡TRUCO CLAVE! Como todas las filas cayeron, la fila que acaba de ocupar este
            // lugar también podría estar llena. Por lo tanto, incrementamos 'fila' para
            // que el 'fila--' del for nos vuelva a dejar en el mismo lugar y la reevalúe.
            fila++;
        }
    }

    // 3. Si borramos al menos una línea, actualizamos las estadísticas del jugador
    if (lineas_borradas_ahora > 0)
    {
        estado->lineas += lineas_borradas_ahora;

        // Sistema de puntos clásico (mientras más líneas rompes juntas, más puntos ganas)
        if (lineas_borradas_ahora == 1) estado->puntos += 100;
        else if (lineas_borradas_ahora == 2) estado->puntos += 300;
        else if (lineas_borradas_ahora == 3) estado->puntos += 500;
        else if (lineas_borradas_ahora == 4) estado->puntos += 800; // ¡TETRIS!
    }
}


