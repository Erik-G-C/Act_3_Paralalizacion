#ifndef IMAGENES_H
#define IMAGENES_H

void imagenes_ByN(const char *mask, const char *path);
void inv_img_color_vertical(const char *mask, const char *path);
void inv_img_color_horizontal(const char *mask, const char *path);
void blurring_57x57(int mm, const char *basename, const char *path, const char *outfolder);
void procesar_imagen(const char *filename);

#endif