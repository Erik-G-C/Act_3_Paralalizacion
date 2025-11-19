// inv_img_color_horizontal.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// 'mask' es la ruta de salida, 'path' es la ruta de entrada
void inv_img_color_horizontal(const char *mask, const char *path)
{
    FILE *fin = NULL;
    FILE *fout = NULL;
    unsigned char header[54];
    uint32_t dataOffset;
    int32_t width, height;
    uint16_t bpp;

    // 1. ABRIR IMAGEN DE ENTRADA
    fin = fopen(path, "rb");
    if (!fin) { 
        perror("Error abriendo imagen de entrada para Inversión Horizontal"); 
        return; 
    }

    // 2. LEER HEADER Y VALIDAR
    if (fread(header, 1, 54, fin) != 54) {
        printf("No se pudo leer header BMP\n");
        fclose(fin); return;
    }

    if (header[0] != 'B' || header[1] != 'M') {
        printf("No es BMP\n");
        fclose(fin); return;
    }

    dataOffset = *(uint32_t*)&header[10];
    width      = *(int32_t*)&header[18];
    height     = *(int32_t*)&header[22];
    bpp        = *(uint16_t*)&header[28];

    if (bpp != 24) {
        printf("Solo BMP 24 bits.\n");
        fclose(fin); return;
    }

    int absHeight = (height > 0 ? height : -height);
    int rowSize   = ((width * 3 + 3) / 4) * 4;

    // 3. LEER HEADER COMPLETO
    unsigned char *fullHeader = malloc(dataOffset);
    fseek(fin, 0, SEEK_SET);
    fread(fullHeader, 1, dataOffset, fin);

    // Buffers para filas
    unsigned char *row    = malloc(rowSize);
    unsigned char *rowInv = malloc(rowSize);

    // 4. ABRIR Y ESCRIBIR IMAGEN DE SALIDA
    fout = fopen(mask, "wb");
    if (!fout) {
        perror("Error creando archivo de salida para Inversión Horizontal");
        free(row);
        free(rowInv);
        free(fullHeader);
        return;
    }
    fwrite(fullHeader, 1, dataOffset, fout);

    fseek(fin, dataOffset, SEEK_SET);

    // 5. INVERTIR FILA POR FILA Y ESCRIBIR
    for (int r = 0; r < absHeight; r++)
    {
        fread(row, 1, rowSize, fin);

        // Invertir píxeles horizontalmente (B G R)
        for (int c = 0; c < width; c++) {
            int dst = width - 1 - c;
            // Copiar los 3 bytes (B, G, R)
            memcpy(&rowInv[dst*3], &row[c*3], 3);
        }

        // Copiar el padding para BMP
        memcpy(rowInv + width*3, row + width*3, rowSize - width*3);

        fwrite(rowInv, 1, rowSize, fout);
    }

    // 6. LIMPIEZA
    free(row);
    free(rowInv);
    free(fullHeader);
    fclose(fin);
    fclose(fout);

    printf("Imagen guardada: %s (HORIZONTAL)\n", mask);
}