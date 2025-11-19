// inv_img_color_horizontal.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void inv_img_color_horizontal(const char *basename, const char *path, const char *outfolder)
{
    FILE *fin = NULL;
    FILE *fout = NULL;
    unsigned char header[54];
    uint32_t dataOffset;
    int32_t width, height;
    uint16_t bpp;
    char outpath[256];

    // Salida correcta
    snprintf(outpath, sizeof(outpath), "%s/%s_invH.bmp", outfolder, basename);

    fin = fopen(path, "rb");
    if (!fin) { perror("Error abriendo entrada"); return; }

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

    // Leer header completo (hasta dataOffset)
    unsigned char *fullHeader = malloc(dataOffset);
    fseek(fin, 0, SEEK_SET);
    fread(fullHeader, 1, dataOffset, fin);

    // Buffers
    unsigned char *row    = malloc(rowSize);
    unsigned char *rowInv = malloc(rowSize);

    // Crear archivo de salida
    fout = fopen(outpath, "wb");
    fwrite(fullHeader, 1, dataOffset, fout);

    fseek(fin, dataOffset, SEEK_SET);

    // Invertir fila por fila
    for (int r = 0; r < absHeight; r++)
    {
        fread(row, 1, rowSize, fin);

        // invertir píxeles horizontalmente
        for (int c = 0; c < width; c++) {
            int dst = width - 1 - c;
            memcpy(&rowInv[dst*3], &row[c*3], 3);
        }

        // copiar padding
        memcpy(rowInv + width*3, row + width*3, rowSize - width*3);

        fwrite(rowInv, 1, rowSize, fout);
    }

    free(row);
    free(rowInv);
    free(fullHeader);
    fclose(fin);
    fclose(fout);

    printf("Imagen guardada: %s (HORIZONTAL)\n", outpath);
}
