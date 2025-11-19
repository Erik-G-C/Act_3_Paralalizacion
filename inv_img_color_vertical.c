// inv_img_color_vertical.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// 'mask' es la ruta de salida, 'path' es la ruta de entrada
void inv_img_color_vertical(const char *mask, const char *path) {

    FILE *fin = NULL;
    FILE *fout = NULL;
    unsigned char header[54];
    uint32_t dataOffset;
    int32_t width, height;
    uint16_t bpp;
    
    // 1. ABRIR IMAGEN DE ENTRADA
    fin = fopen(path, "rb");
    if (!fin) { 
        perror("Error abriendo imagen de entrada para Inversión Vertical"); 
        return; 
    }

    // 2. LEER HEADER Y VALIDAR
    if (fread(header, 1, 54, fin) != 54) {
        printf("No se pudo leer el header BMP\n");
        fclose(fin);
        return;
    }

    if (header[0] != 'B' || header[1] != 'M') {
        printf("Archivo no es BMP\n");
        fclose(fin);
        return;
    }

    dataOffset = *(uint32_t*)&header[10];
    width      = *(int32_t*)&header[18];
    height     = *(int32_t*)&header[22];
    bpp        = *(uint16_t*)&header[28];

    if (bpp != 24) {
        printf("Solo se aceptan BMP 24 bits.\n");
        fclose(fin); 
        return;
    }

    int absHeight = (height > 0 ? height : -height);
    int bytesPP   = 3;
    int rowSize   = ((width * bytesPP + 3) / 4) * 4;

    // 3. LEER HEADER COMPLETO Y PIXELES
    unsigned char *fullHeader = malloc(dataOffset);
    fseek(fin, 0, SEEK_SET);
    fread(fullHeader, 1, dataOffset, fin);

    // Cargar toda la imagen en RAM
    unsigned char *img = malloc(rowSize * absHeight);

    fseek(fin, dataOffset, SEEK_SET);
    fread(img, 1, rowSize * absHeight, fin);
    fclose(fin);

    // 4. ABRIR Y ESCRIBIR IMAGEN DE SALIDA
    fout = fopen(mask, "wb");
    if (!fout) {
        perror("Error creando archivo de salida para Inversión Vertical");
        free(img);
        free(fullHeader);
        return;
    }
    fwrite(fullHeader, 1, dataOffset, fout);

    // Escribir filas en orden invertido
    for (int r = 0; r < absHeight; r++) {
        int srcRow = absHeight - 1 - r;
        fwrite(img + srcRow * rowSize, 1, rowSize, fout);
    }

    // 5. LIMPIEZA
    free(img);
    free(fullHeader);
    fclose(fout);

    printf("Imagen guardada: %s (VERTICAL)\n", mask);
}