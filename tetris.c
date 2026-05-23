#include "tetris.h"
#include <time.h>
#include <math.h>

//--Formas de los 7 tetrominos

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

//--Inicializacion

void inicializar_juego(EstadoJuego* estado)
{
    inicializar_tablero(estado);
    inicializar_estructura(estado);
    generar_nueva_pieza(estado); // Crea la pieza que cae ahora
    estado->pieza_actual = estado->pieza_siguiente;
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

//--Dificultad

//Recalculamos velocidad de caida aplicando 0.97 tantas veces como niveles de 10 piezas hayan sido alcanzados
void recalcular_velocidad(EstadoJuego* estado)
{
    int niveles = estado->piezas_caidas / 10;
    float v = VELOCIDAD_INICIAL_MS;
    for(int i = 0; i < niveles; i++)
    {
        v *= FACTOR_VELOCIDAD;
    }
    if(v < 100.0f) v = 100.0f;
    estado->velocidad_caida_ms = v;

}

//--Colision
int colision(EstadoJuego *estado, Tetromino *pieza)
{
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (pieza->forma[y][x])
            {
                int tablero_x = pieza->x + x;
                int tablero_y = pieza->y + y;

                // Fuera de limites izquierdo, derecho, o fondo
                if (tablero_x < 0 || tablero_x >= COLUMNAS || tablero_y >= FILAS_TOTALES)
                {
                    return 1;
                }

                // Colision con otra pieza en el tablero
                if (tablero_y >= 0 && estado->tablero[tablero_y][tablero_x] != 0)
                {
                    return 1;
                }
            }
        }
    }
    return 0; // No hay colision
}


int puede_mover_pieza(EstadoJuego* estado, int movimiento_en_x, int movimiento_en_y)
{
    Tetromino aux = estado->pieza_actual;

    aux.x += movimiento_en_x;
    aux.y += movimiento_en_y;


    if (colision(estado, &aux)) //Preguntamos si en ese lugar hay colision
    {
        return 0;
    }

    return 1;
}

void mover_pieza(EstadoJuego* estado, int movimiento_en_x, int movimiento_en_y)
{
    if(puede_mover_pieza(estado, movimiento_en_x, movimiento_en_y))
    {
        estado->pieza_actual.x += movimiento_en_x;
        estado->pieza_actual.y += movimiento_en_y;
    }
}

//--Rotacion

void rotar_matriz(int origen[][4], int destino[][4], int a_la_derecha)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if (a_la_derecha)
            {
                destino[j][3 - i] = origen[i][j];
            }
            else
            {
                destino[3 - j][i] = origen[i][j];
            }
        }
    }
}

//Si rota y colisiona, probamos desplazarla 1 o 2 posiciones a izq/der antes de rechazar la rotacion
//Termino denominado: wall kick
int rotar_pieza_actual(EstadoJuego* estado, int direccion)
{
    Tetromino aux = estado->pieza_actual; // Hacemos una copia para probar

    int nueva_forma[4][4];

    rotar_matriz(aux.forma, nueva_forma, direccion > 0);

    // Asignamos nueva forma a la copia
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            aux.forma[y][x] = nueva_forma[y][x];
        }
    }

    if (!colision(estado, &aux))
    {
        estado->pieza_actual = aux;
        return 1;
    }

    int kicks[] = {1, -1, 2, -2};
    for (int k = 0; k < 4; k++)
    {
        aux.x = estado->pieza_actual.x + kicks[k];
        if (!colision(estado, &aux))
        {
            estado->pieza_actual = aux;
            return 1;
        }
    }


    return 0;//rotación imposible
}

//--Generar pieza

void generar_nueva_pieza(EstadoJuego* estado)
{
    estado->piezas_caidas ++;
    estado->pieza_siguiente.tipo = rand() % 7;
    estado->pieza_siguiente.x = 3; // Medio del tablero
    estado->pieza_siguiente.y = 0; // Parte invisible superior

    const int (*origen)[4] = NULL;
    switch(estado->pieza_siguiente.tipo)
    {
    case 0:
        origen = matI;
        break;
    case 1:
        origen = matJ;
        break;
    case 2:
        origen = matL;
        break;
    case 3:
        origen = matO;
        break;
    case 4:
        origen = matS;
        break;
    case 5:
        origen = matT;
        break;
    case 6:
        origen = matZ;
        break;
    }

    // Copiamos la forma original a la pieza nueva
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            estado->pieza_siguiente.forma[y][x] = origen[y][x];
        }
    }
}

//--Fijar pieza

void fijar_pieza(EstadoJuego* estado)
{
    Tetromino* p = &estado->pieza_actual;

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4 ; j++)
        {
            if(p->forma[i][j])
            {
                int fila = p->y+i;
                int columna = p->x+j; //Para saber la posicion real en el tablero

                estado->tablero[fila][columna] = COLORES_PIEZAS[estado->pieza_actual.tipo]; //Pintamos tablero
            }
        }
    }
}

//--Borrar lineas
void borrar_lineas_completas(EstadoJuego* estado)
{
    int lineas_borradas_ahora = 0;

    for (int fila = FILAS_TOTALES - 1; fila >= 0; fila--)// Recorremos el tablero de abajo hacia arriba
    {
        // Verificamos si la fila actual está completamente llena

        int fila_llena = 1;
        for (int col = 0; col < COLUMNAS; col++)
        {
            if (estado->tablero[fila][col] == 0) // Encontramos un espacio vacio
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

            // La fila superior (indice 0) ya no tiene a nadie arriba, se vacia (se llena de ceros)
            for (int col = 0; col < COLUMNAS; col++)
            {
                estado->tablero[0][col] = 0;
            }

            //Como todas las filas cayeron, la fila que acaba de ocupar este
            // lugar también podria estar llena. Por lo tanto, incrementamos 'fila' para
            // que el 'fila--' del for nos vuelva a dejar en el mismo lugar y la reevalue.
            fila++;
        }
    }

    // 3. Si borramos al menos una linea, actualizamos las estadisticas del jugador
    if (lineas_borradas_ahora > 0)
    {
        estado->lineas += lineas_borradas_ahora;

        // Sistema de puntos clasico (mientras mas líneas rompes juntas, mas puntos ganas)
        // A mayor velocidad (ms más bajo) más puntos
        float multiplicador = VELOCIDAD_INICIAL_MS / estado->velocidad_caida_ms;
        long base = 0;
        if (lineas_borradas_ahora == 1) base = 100;
        else if (lineas_borradas_ahora == 2) base = 300;
        else if (lineas_borradas_ahora == 3) base = 500;
        else if (lineas_borradas_ahora == 4) base = 800; // TETRIS!!!!!!

        estado->puntos += (long)(base * multiplicador);
    }
}

//Jugador

void guardar_jugador(const Jugador* p)
{
    FILE* f = fopen(ARCHIVO_JUGADOR, "wb");
    if (!f) return;
    fwrite(p, sizeof(Jugador), 1, f);
    fclose(f);
}

int cargar_jugador(Jugador* p)
{
    FILE* f = fopen(ARCHIVO_JUGADOR, "rb");
    if (!f) return 0;
    fread(p, sizeof(Jugador), 1, f);
    fclose(f);
    return 1;
}





