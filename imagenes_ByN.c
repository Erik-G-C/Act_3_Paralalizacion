// imagenes_ByN.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void imagenes_ByN(const char *mask, const char *path)
{
    FILE *fin = NULL;
    FILE *fout = NULL;
    unsigned char header[54];
    uint32_t dataOffset;
    int32_t width, height;
    uint16_t bpp;

    fin = fopen(path, "rb");
    if (!fin) { 
        perror("Error abriendo imagen de entrada"); 
        return; 
    }

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
    int rowSize   = ((width * 3 + 3) / 4) * 4;

    unsigned char *fullHeader = malloc(dataOffset);
    fseek(fin, 0, SEEK_SET);
    fread(fullHeader, 1, dataOffset, fin);

    unsigned char *img = malloc(rowSize * absHeight);
    fseek(fin, dataOffset, SEEK_SET);
    fread(img, 1, rowSize * absHeight, fin);
    fclose(fin);

    unsigned char *out = malloc(rowSize * absHeight);

    for (int r = 0; r < absHeight; r++) {
        unsigned char *rowIn  = img + r * rowSize;
        unsigned char *rowOut = out + r * rowSize;

        for (int c = 0; c < width; c++) {
            unsigned char b = rowIn[c*3 + 0];
            unsigned char g = rowIn[c*3 + 1];
            unsigned char r_ = rowIn[c*3 + 2];

            unsigned char gray = (unsigned char)(0.21*r_ + 0.72*g + 0.07*b);

            rowOut[c*3 + 0] = gray;
            rowOut[c*3 + 1] = gray;
            rowOut[c*3 + 2] = gray;
        }

        memcpy(rowOut + width*3, rowIn + width*3, rowSize - width*3);
    }

    fout = fopen(mask, "wb");
    if (!fout) {
        perror("No se pudo crear archivo de salida");
        free(img);
        free(out);
        free(fullHeader);
        return;
    }

    fwrite(fullHeader, 1, dataOffset, fout);
    fwrite(out, 1, rowSize * absHeight, fout);
    fclose(fout);

    free(img);
    free(out);
    free(fullHeader);

    printf("Imagen convertida a grises: %s\n", mask);
}
