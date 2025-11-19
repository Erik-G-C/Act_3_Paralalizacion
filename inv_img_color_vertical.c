// inv_img_color_vertical.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

extern void inv_img_color_vertical(const char *mask, const char *path) {

    FILE *fin = NULL;
    FILE *fout = NULL;
    unsigned char header[54];
    uint32_t dataOffset;
    int32_t width, height;
    uint16_t bpp;
    char outpath[160];

    snprintf(outpath, sizeof(outpath), "./img/%s_flip.bmp", mask);

    fin = fopen(path, "rb");
    if (!fin) { perror("Error"); return; }

    fread(header, 1, 54, fin);

    dataOffset = *(uint32_t*)&header[10];
    width      = *(int32_t*)&header[18];
    height     = *(int32_t*)&header[22];
    bpp        = *(uint16_t*)&header[28];

    if (bpp != 24) {
        printf("Solo BMP 24 bits.\n");
        fclose(fin); return;
    }

    int absHeight = (height > 0 ? height : -height);
    int bytesPP   = 3;
    int rowSize   = ((width * bytesPP + 3) / 4) * 4;

    unsigned char *fullHeader = malloc(dataOffset);
    fseek(fin, 0, SEEK_SET);
    fread(fullHeader, 1, dataOffset, fin);

    // cargar toda la imagen en RAM
    unsigned char *img = malloc(rowSize * absHeight);

    fseek(fin, dataOffset, SEEK_SET);
    fread(img, 1, rowSize * absHeight, fin);
    fclose(fin);

    fout = fopen(outpath, "wb");
    fwrite(fullHeader, 1, dataOffset, fout);

    // escribir filas en orden invertido
    for (int r = 0; r < absHeight; r++) {
        int srcRow = absHeight - 1 - r;
        fwrite(img + srcRow * rowSize, 1, rowSize, fout);
    }

    free(img);
    free(fullHeader);
    fclose(fout);

    printf("Imagen guardada: %s (VERTICAL)\n", outpath);
}
