================================================================================
           TRABAJO PRACTICO INTEGRADOR - ENIGMA TETRIS
           Universidad Nacional de la Matanza (UNLaM) - 2026
================================================================================

INTEGRANTES DEL EQUIPO:
- Kouvach, Marcos (DNI: 45013925) - Usuario: mkouvach
- Alvarez da Silva, Keila (DNI: 45236512) - Usuario: keydasilva7
- Crego, Agustina Rocio (DNI: 44689109) - Usuario: Agusrc20

--------------------------------------------------------------------------------
Este documento detalla paso a paso como compilar, ejecutar y utilizar el juego
Tetris, desarrollado como Trabajo Practico Integrador.
--------------------------------------------------------------------------------


[1] REQUISITOS Y LINKS DE DESCARGA
--------------------------------------------------------------------------------
- Sistema Operativo: Windows (10 o superior recomendado).
- Entorno de desarrollo: Code::Blocks (Version con MinGW incluido).
  Link de descarga oficial: https://www.codeblocks.org/downloads/binaries/
  (Descargar obligatoriamente el archivo: codeblocks-XX.XXmingw-setup.exe)

- Biblioteca Grafica: GBT_v2026 (Game Boy Template).
  Link de descarga oficial del repositorio de la biblioteca: 
  https://gitlab.com/RodrigoMaranzana/libgbt-dist


[2] CONFIGURACION DEL ENTORNO Y COMPILACION (Paso a Paso)
--------------------------------------------------------------------------------
Siga estas instrucciones de forma secuencial para armar el entorno de desarrollo:

1. Instale Code::Blocks utilizando el instalador que incluye MinGW.
2. Descargue el archivo .zip con el Release de la biblioteca grafica GBT provisto 
   en el repositorio oficial compartido en el punto [1].
3. Descomprima el paquete utilizando el software 7-Zip seleccionando la opcion 
   "Extract Here" (Extraer aqui).
4. Abra Code::Blocks y cree un nuevo proyecto: 
   File -> New -> Project -> Console Application -> C. Asigne el titulo de su preferencia.
5. Dentro del directorio de su nuevo proyecto, cree la ruta de carpetas:
   lib\GBT_v2026.1C\ 
6. Copie todo el contenido descomprimido de la biblioteca adentro de esa carpeta creada.
7. Copie sus archivos fuentes propios del Tetris (main.c, juego.c, juego.h, tetris.c, 
   tetris.h, graficos.c, graficos.h, font8x16.h) directamente en la carpeta raiz del proyecto.
8. En Code::Blocks, añada sus archivos al proyecto haciendo clic derecho sobre el nombre 
   del proyecto (panel izquierdo) -> "Add files...". Seleccione sus archivos .c y .h.
9. Haga clic derecho sobre el nombre del proyecto y pulse "Build options...".
10. Asegurese de estar situado sobre el Build target "Debug" en el panel izquierdo.
11. Vaya a la solapa "Linker settings" y en el cuadro "Other linker options" agregue:
    -lgbt
12. Acceda a la solapa "Search Directories" y configure las rutas de busqueda:
    - En la sub-solapa "Compiler": Pulse el boton "Add" y seleccione la ruta a la 
      carpeta "include/" de la biblioteca que guardo en el paso 5.
    - En la sub-solapa "Linker": Pulse el boton "Add" y seleccione la ruta a la 
      carpeta "lib/" de la biblioteca que guardo en el paso 5.
13. Pulse "OK" para guardar y cerrar todas las configuraciones.
14. Para compilar el programa, presione la tecla F9 (o el boton "Build and Run").
15. REQUISITO DE EJECUCION: Una vez que el programa se compile, vaya a la carpeta 
    donde se genero el archivo ejecutable (.exe). Copie el archivo "gbt.dll" (incluido 
    en la descarga de la biblioteca) y pegelo exactamente en esa misma ubicacion, 
    al lado de su "EnigmaTetris.exe".


[3] EJECUCION Y ARGUMENTOS DE CONSOLA
--------------------------------------------------------------------------------
El juego puede ejecutarse haciendo doble clic en el .exe (con la gbt.dll al lado),
o bien a traves de la linea de comandos (CMD de Windows) para usar parametros:

Formato de ejecucion:
  > EnigmaTetris.exe [-vga / -cga] [-escala <n>]

Argumentos:
-vga       : Inicia el juego en resolucion VGA (640x480).
-cga       : Inicia el juego en resolucion CGA (320x200).
-escala N  : Define el multiplicador de tamaño de ventana (ej: -escala 2).


[4] COMO JUGAR (CONTROLES)
--------------------------------------------------------------------------------
Controles en Juego:
- Flecha Izquierda : Mover pieza a la izquierda.
- Flecha Derecha   : Mover pieza a la derecha.
- Flecha Abajo     : Caida rapida de la pieza (Soft Drop).
- Tecla Z          : Rotar la pieza en sentido anti-horario.
- Flecha Arriba / ESPACIO: Rotar la pieza en sentido horario.
- P o ESC          : Pausar el juego.

Sistema de Puntuacion:
El jugador acumula puntos al limpiar lineas. Cada 10 piezas caidas, la dificultad 
aumenta, lo que incrementa la velocidad de caida base. Al tocar el suelo, el juego 
brinda una tolerancia de tiempo del 50% de la velocidad actual, la cual se resetea 
si la pieza se mantiene en movimiento (con un limite maximo anti-trampa).


[5] ESTRUCTURA DE ARCHIVOS DEL PROYECTO
--------------------------------------------------------------------------------
Para garantizar una correcta compilacion, la estructura de archivos en el disco 
debe verse de la siguiente manera:

/Directorio_Tu_Proyecto/
  ├── TuProyecto.cbp      (Archivo de proyecto de Code::Blocks)
  ├── main.c              (Ciclo principal y maquina de estados)
  ├── juego.c / juego.h   (Controladores de los estados de la aplicacion)
  ├── tetris.c / tetris.h (Logica de juego: colisiones, rotacion, puntajes)
  ├── graficos.c / .h     (Capa de abstraccion de renderizado y UI)
  ├── font8x16.h          (Datos binarios del set de caracteres para los textos)
  ├── jugador.dat         (Archivo binario auto-generado con highscores y paletas)
  ├── lib/
  │    └── GBT_v2026.1C/  (Carpeta de la biblioteca provista)
  │         ├── include/
  │         │    └── GBT/
  │         │         └── gbt.h
  │         └── lib/
  │              └── libgbt.a
  └── bin/Debug/
       ├── EnigmaTetris.exe (Ejecutable generado tras compilar con F9)
       └── gbt.dll          (Biblioteca dinamica copiada de forma manual aqui)


[6] SOLUCON DE PROBLEMAS FRECUENTES (Para el dia de la defensa)
--------------------------------------------------------------------------------
Si al mover el proyecto de computadora el entorno deja de compilar, verifique:

- Error: "fatal error: GBT/gbt.h: No such file or directory"
  Solucion: Las carpetas cambiaron de lugar. Vuelva a realizar el paso 12 del 
  punto [2] para actualizar las rutas en "Search Directories -> Compiler".

- Error: "cannot find -lgbt"
  Solucion: Code::Blocks no encuentra la libreria estatica. Vuelva a realizar 
  el paso 12 del punto [2] para actualizar "Search Directories -> Linker".

- El programa compila pero se cierra inmediatamente al abrir o tira error de DLL.
  Solucion: Asegurese de haber copiado el archivo "gbt.dll" exactamente adentro 
  de la carpeta donde se encuentra el archivo .exe ejecutable (Paso 15).

- Modificaciones no impactan en el juego.
  Solucion: Vaya al menu superior de Code::Blocks: Build -> Rebuild. Esto borrara 
  la cache vieja de la otra PC y forzara una compilacion limpia.