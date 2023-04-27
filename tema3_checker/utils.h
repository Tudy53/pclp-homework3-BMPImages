#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "bmp_header.h"
#define MAX_NR_CH_COMMAND 100
#define MAX_NR_CH 1000

typedef struct {
    char B;
    char G;
    char R;
} Pixel;

typedef struct {
    Pixel **pixels;
} BitMap;


typedef struct {
    bmp_fileheader *fileheader;
    bmp_infoheader *infoheader;
    BitMap *bitmap;
} BMP;

typedef struct {
    int r;  // R
    int g;  // G
    int b;  // B
    int dimension;
} Pencil;

BMP *allocBMP();
BMP *read_bmp(FILE *in);
void freeBMP(BMP *image);
