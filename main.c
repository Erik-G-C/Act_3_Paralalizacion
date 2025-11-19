#include "imagenes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#ifdef _WIN32
#include <direct.h>   // para _mkdir
#define MKDIR(dir) _mkdir(dir)
#else
#include <sys/stat.h>
#define MKDIR(dir) mkdir(dir, 0777)
#endif

#define NUM_IMAGENES 30
#define NUM_THREADS 4

int main() {

    // Crear carpeta de salida si no existe
    struct stat st = {0};
    if (stat("imagenes_salidas", &st) == -1) {
        MKDIR("imagenes_salidas");
    }

    // Generar lista de nombres de imágenes automáticamente
    char imagenes[NUM_IMAGENES][64];
    for (int i = 0; i < NUM_IMAGENES; i++) {
        sprintf(imagenes[i], "Entradas_BMP/Imagen_%02d.bmp", i + 1);
    }

    printf("\n=== Procesamiento de %d imágenes ===\n\n", NUM_IMAGENES);

    omp_set_num_threads(NUM_THREADS);
    double t0 = omp_get_wtime();   // Tiempo inicial total

    // Paralelizar las imágenes con OpenMP
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < NUM_IMAGENES; i++) {

        printf("Procesando %s ... (thread %d)\n",
               imagenes[i], omp_get_thread_num());

        procesar_imagen(imagenes[i]);

        printf("→ %s terminado (thread %d).\n", 
               imagenes[i], omp_get_thread_num());
    }

    double t1 = omp_get_wtime();   // Tiempo final total

    printf("\n=== Procesamiento COMPLETO ===\n");
    printf("Tiempo TOTAL de ejecución: %.3f segundos\n\n", t1 - t0);

    return 0;
}
