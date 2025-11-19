// blurring_57x57.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>

void blurring_57x57(int mm, const char *basename, const char *path, const char *outfolder)
{
    FILE *fin = fopen(path, "rb");
    if (!fin) { perror("fopen input"); return; }

    unsigned char header[54];
    if (fread(header, 1, 54, fin) != 54) { fprintf(stderr,"No header\n"); fclose(fin); return; }

    if (header[0] != 'B' || header[1] != 'M') { fprintf(stderr,"No BMP\n"); fclose(fin); return; }

    uint32_t dataOffset = *(uint32_t*)&header[10];
    int32_t width       = *(int32_t*)&header[18];
    int32_t height      = *(int32_t*)&header[22];
    uint16_t bpp        = *(uint16_t*)&header[28];

    if (bpp != 24) { fprintf(stderr,"Solo BMP 24bpp\n"); fclose(fin); return; }

    int absH = (height > 0 ? height : -height);
    int rowSize = ((width * 3 + 3) / 4) * 4;

    size_t pixelArraySize = (size_t)rowSize * absH;

    unsigned char *fullHeader = malloc(dataOffset);
    fseek(fin, 0, SEEK_SET);
    fread(fullHeader, 1, dataOffset, fin);

    unsigned char *inData = malloc(pixelArraySize);
    fseek(fin, dataOffset, SEEK_SET);
    fread(inData, 1, pixelArraySize, fin);
    fclose(fin);

    unsigned char *outData = malloc(pixelArraySize);
    memset(outData, 0, pixelArraySize);

    int kernelRadius = (mm - 1) / 2;

    #pragma omp parallel for schedule(dynamic)
    for (int y = 0; y < absH; ++y) {

        for (int x = 0; x < width; ++x) {

            unsigned long rSum = 0, gSum = 0, bSum = 0;
            int count = 0;

            for (int ky = -kernelRadius; ky <= kernelRadius; ++ky) {
                int yy = y + ky;
                if (yy < 0 || yy >= absH) continue;

                unsigned char *rowPtr = inData + (size_t)yy * rowSize;

                for (int kx = -kernelRadius; kx <= kernelRadius; ++kx) {
                    int xx = x + kx;
                    if (xx < 0 || xx >= width) continue;

                    size_t idx = xx * 3;
                    bSum += rowPtr[idx + 0];
                    gSum += rowPtr[idx + 1];
                    rSum += rowPtr[idx + 2];
                    count++;
                }
            }

            if (count == 0) count = 1;
            unsigned char bAvg = (unsigned char)(bSum / count);
            unsigned char gAvg = (unsigned char)(gSum / count);
            unsigned char rAvg = (unsigned char)(rSum / count);

            unsigned char *outRow = outData + (size_t)y * rowSize;
            outRow[x*3+0] = bAvg;
            outRow[x*3+1] = gAvg;
            outRow[x*3+2] = rAvg;
        }
    }

    // CREA SALIDA CORRECTA
    char outpath[256];
    snprintf(outpath, sizeof(outpath), "%s/%s_blur.bmp", outfolder, basename);

    FILE *fout = fopen(outpath, "wb");
    fwrite(fullHeader, 1, dataOffset, fout);
    fwrite(outData, 1, pixelArraySize, fout);
    fclose(fout);

    free(inData);
    free(outData);
    free(fullHeader);

    printf("Blur guardado en: %s\n", outpath);
}
