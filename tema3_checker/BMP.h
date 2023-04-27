#include "utils.h"
BMP *allocBMP() {
    BMP *bmp = (BMP *)malloc(sizeof(BMP));
    if (bmp == NULL) {
        printf("Allocation didn't take place!\n");
        return NULL;
    }

    bmp->fileheader = (bmp_fileheader *)malloc(sizeof(bmp_fileheader));
    if (bmp->fileheader == NULL) {
        printf("Allocation didn't take place!\n");
        return NULL;
    }

    bmp->infoheader = (bmp_infoheader *)malloc(sizeof(bmp_infoheader));
    if (bmp->infoheader == NULL) {
        printf("Allocation didn't take place!\n");
        return NULL;
    }

    bmp->bitmap = (BitMap *)malloc(sizeof(BitMap));
    if (bmp->bitmap == NULL) {
        printf("Allocation didn't take place!\n");
        return NULL;
    }

    return bmp;
}

BMP *read_bmp(FILE *in) {
    BMP *image = allocBMP();
    if (image == NULL) {
        printf("Allocation didn't take place!\n");
        return NULL;
    }

    fread(image->fileheader, sizeof(bmp_fileheader), 1, in);
    fread(image->infoheader, sizeof(bmp_infoheader), 1, in);
    image->bitmap->pixels = (Pixel **)malloc(image->infoheader->height * sizeof(Pixel *));
    if (image->bitmap->pixels == NULL) {
        return NULL;
    }

    for (int i = 0; i < image->infoheader->height; i++) {
        image->bitmap->pixels[i] = (Pixel *)malloc(image->infoheader->width * sizeof(Pixel));
        if (image->bitmap->pixels[i] == NULL) {
            return NULL;
        }
    }

    int padding = image->infoheader->width % 4;
    // I delete the padding bits
    for (int i = 0; i < image->infoheader->height; i++) {
        for (int j = 0; j < image->infoheader->width; j++) {
            fread(&(image->bitmap->pixels[i][j]), sizeof(Pixel), 1, in);
        }
        fseek(in, padding, SEEK_CUR);
    }

    return image;
}

BMP *read_bmp_insertion(FILE *in) {
    BMP *image_to_insert = allocBMP();
    if (image_to_insert == NULL) {
        printf("Allocation didn't take place!\n");
        return NULL;
    }
}

void freeBMP(BMP *image) {
    // I free the initial imgage space
    for (int i = 0; i < image->infoheader->height; i++) {
        free(image->bitmap->pixels[i]);
    }

    free(image->bitmap->pixels);
    free(image->bitmap);
    free(image->infoheader);
    free(image->fileheader);
    free(image);
}

void freeBMPinsertion(BMP *image_to_insert) {
    // I free the inserted imgage space
    for (int i = 0; i < image_to_insert->infoheader->height; i++) {
        free(image_to_insert->bitmap->pixels[i]);
    }

    free(image_to_insert->bitmap->pixels);
    free(image_to_insert->bitmap);
    free(image_to_insert->infoheader);
    free(image_to_insert->fileheader);
    free(image_to_insert);
}

