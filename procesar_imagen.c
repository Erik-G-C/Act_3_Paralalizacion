// procesar_imagen.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "imagenes.h"

void procesar_imagen(const char *filename)
{
    char base[128];
    char outfolder[] = "imagenes_salidas";

    // Crear carpeta si no existe (Windows y Linux)
    struct stat st = {0};
    if (stat(outfolder, &st) == -1) {
        mkdir(outfolder);
    }

    // -----------------------------------------------------------
    //   Extraer solo el nombre base (compatible con Windows y Linux)
    // -----------------------------------------------------------
    const char *fname_only = strrchr(filename, '\\');  // Windows (ruta con '\')

    if (!fname_only)
        fname_only = strrchr(filename, '/');           // Linux/Mac (ruta con '/')

    if (fname_only)
        fname_only++;          // avanzar después del separador
    else
        fname_only = filename; // era un nombre simple

    // Extraer nombre sin extensión
    size_t len = strlen(fname_only);
    if (len > 4 && strcmp(fname_only + len - 4, ".bmp") == 0)
        len -= 4; // quitar ".bmp"

    strncpy(base, fname_only, len);
    base[len] = '\0';

    // -----------------------------------------------------------
    //   PROCESOS
    // -----------------------------------------------------------

    // === Escala de grises ===
    {
        char outname[256];
        snprintf(outname, sizeof(outname),
                 "%s/%s_ByN.bmp", outfolder, base);

        imagenes_ByN(outname, filename);
    }

    // === Invertir Vertical ===
    {
        char outname[256];
        snprintf(outname, sizeof(outname),
                 "%s/%s_inv-vert.bmp", outfolder, base);

        inv_img_color_vertical(outname, filename);
    }

    // === Invertir Horizontal ===
    {
        char outname[256];
        snprintf(outname, sizeof(outname),
                 "%s/%s_inv-hor.bmp", outfolder, base);

        inv_img_color_horizontal(outname, filename);
    }

    // === Blurring 57x57 ===
    {
        char outname[256];
        snprintf(outname, sizeof(outname),
                 "%s/%s_blur.bmp", outfolder, base);

        blurring_57x57(57, base, filename, outfolder);
    }

    printf("Procesamiento finalizado para: %s\n", filename);
}
