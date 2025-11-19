
#include <direct.h>   // para _mkdir
#define MKDIR(dir) _mkdir(dir)

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <omp.h>
#include "imagenes.h"

#define NUM_THREADS 30

int main()
{
    // Crear carpeta de salida
    struct stat st = {0};
    if (stat("imagenes_salidas", &st) == -1) {
        MKDIR("imagenes_salidas"); 
    }

    omp_set_num_threads(NUM_THREADS);

    printf("\n=== Procesamiento con SECTIONS (%d hilos) ===\n\n", NUM_THREADS);
    double start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp sections
        {

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_01.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_02.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_03.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_04.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_05.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_06.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_07.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_08.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_09.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_10.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_11.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_12.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_13.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_14.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_15.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_16.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_17.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_18.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_19.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_20.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_21.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_22.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_23.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_24.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_25.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_26.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_27.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_28.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_29.bmp");

            #pragma omp section
            procesar_imagen("Entradas_BMP/Imagen_30.bmp");
        }
    }

    double end = omp_get_wtime();
    printf("\n=== Procesamiento COMPLETO ===\n");
    printf("Tiempo TOTAL de ejecucion: %.3f segundos\n\n", end - start);


    return 0;
}
