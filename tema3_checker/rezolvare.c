#include "BMP.h"

BMP *edit() {
    char *citire = (char *)malloc(MAX_NR_CH_COMMAND * sizeof(char));
    if (citire == NULL) {
        printf("Allocation didn't take place!\n");
        return NULL;
    }

    scanf("%s", citire);

    FILE *in = fopen(citire, "rb");
    BMP *image = read_bmp(in);

    free(citire);
    fclose(in);
    return image;
}

Pencil *set(Pencil *pencil) {
    char *command2 = (char *)malloc(MAX_NR_CH_COMMAND * sizeof(char));
    if (command2 == NULL) {
        printf("Allocation didn't take place!\n");
        return 0;
    }

    scanf("%s", command2);

    // I read the R, G, B values ​​of the pencil and its size from the keyboard

    if (strcmp(command2, "draw_color") == 0) {
        scanf("%d", &pencil->r);
        scanf("%d", &pencil->g);
        scanf("%d", &pencil->b);
    }

    if (strcmp(command2, "line_width") == 0) {
        scanf("%d", &pencil->dimension);
    }

    return pencil;
}

BMP *color(BMP *image, Pencil *pencil, int x1, int y1, int x2, int y2) {
    int dif_x = x2 - x1;
    int dif_y = y2 - y1;
    int x_interval = 0, y_interval = 0, result = 0;

    x_interval = x1 - x2;
    y_interval = y1 - y2;
    if (x1 < x2) {
        x_interval = x2 - x1;
    }
    if (y1 < y2) {
        y_interval = y2 - y1;
    }
    if (x_interval > y_interval) {
        if (x1 < x2) {
            for (int i = x1; i <= x2; i++) {
                if (i < 0 || i >= image->infoheader->width) {
                    continue;
                }
                result = ((i - x1) * dif_y + y1 * dif_x) / dif_x;
                if (result < 0 || result >= image->infoheader->height) {
                    continue;
                }
                image->bitmap->pixels[result][i].R = (char)pencil->r;
                image->bitmap->pixels[result][i].G = (char)pencil->g;
                image->bitmap->pixels[result][i].B = (char)pencil->b;
            }
        } else {
            for (int i = x1; i >= x2; i--) {
                if (i < 0 || i >= image->infoheader->width) {
                    continue;
                }
                result = ((i - x1) * dif_y + y1 * dif_x) / dif_x;
                if (result < 0 || result >= image->infoheader->height) {
                    continue;
                }
                image->bitmap->pixels[result][i].R = (char)pencil->r;
                image->bitmap->pixels[result][i].G = (char)pencil->g;
                image->bitmap->pixels[result][i].B = (char)pencil->b;
            }
        }
    } else {  // I draw x after Oy
        if (y1 < y2) {
            for (int i = y1; i <= y2; i++) {
                if (i < 0 || i >= image->infoheader->height) {
                    continue;
                }
                result = ((i - y1) * dif_x + x1 * dif_y) / dif_y;
                if (result < 0 || result >= image->infoheader->width) {
                    continue;
                }
                image->bitmap->pixels[i][result].R = (char)pencil->r;
                image->bitmap->pixels[i][result].G = (char)pencil->g;
                image->bitmap->pixels[i][result].B = (char)pencil->b;
            }
        } else {
            for (int i = y1; i >= y2; i--) {
                if (i < 0 || i >= image->infoheader->height) {
                    continue;
                }
                result = ((i - y1) * dif_x + x1 * dif_y) / dif_y;
                if (result < 0 || result >= image->infoheader->width) {
                    continue;
                }
                image->bitmap->pixels[i][result].R = (char)pencil->r;
                image->bitmap->pixels[i][result].G = (char)pencil->g;
                image->bitmap->pixels[i][result].B = (char)pencil->b;
            }
        }
    }
    return image;
}

BMP *fill_matrice(BMP *image, char blue, char green, char red, int i, int j, Pencil *pencil) {
    image->bitmap->pixels[i][j].R = (char)pencil->r;
    image->bitmap->pixels[i][j].G = (char)pencil->g;
    image->bitmap->pixels[i][j].B = (char)pencil->b;
    // depending on the position, I color the "neighbor" pixel
    if (i >= 1) {
        Pixel p1 = image->bitmap->pixels[i-1][j];
        if (p1.B == blue && p1.G == green && p1.R == red) {
            image = fill_matrice(image, blue, green, red, i - 1, j, pencil);
        }
    }
    if (i < image->infoheader->height - 1) {
        Pixel p2 = image->bitmap->pixels[i + 1][j];
        if (p2.B == blue && p2.G == green && p2.R == red) {
            image = fill_matrice(image, blue, green, red, i + 1, j, pencil);
        }
    }
    if (j >= 1) {
        Pixel p3 = image->bitmap->pixels[i][j-1];
        if (p3.B == blue && p3.G == green && p3.R == red) {
            image = fill_matrice(image, blue, green, red, i, j - 1, pencil);
        }
    }
    if (j < image->infoheader->width - 1) {
        Pixel p4 = image->bitmap->pixels[i][j+1];
        if (p4.B == blue && p4.G == green && p4.R == red) {
            image = fill_matrice(image, blue, green, red, i, j + 1, pencil);
        }
    }
    return image;
}

BMP *draw(BMP *image, Pencil *pencil) {
    char *command2 = (char *)malloc(MAX_NR_CH_COMMAND * sizeof(char));
    if (command2 == NULL) {
        printf("Allocation didn't take place!\n");
        return 0;
    }

    scanf("%s", command2);

    if (strcmp(command2, "line") == 0) {
        int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
        scanf("%d", &y1);
        scanf("%d", &x1);
        scanf("%d", &y2);
        scanf("%d", &x2);
        // We start with each pixel from the square around the pixel
        for (int i = -pencil->dimension / 2; i <= pencil->dimension / 2; i++) {
            for (int j = -pencil->dimension/2; j <= pencil->dimension / 2; j++) {
                color(image, pencil, y1 + i, x1 + j, y2 + i, x2 + j);
            }
        }
    }

    if (strcmp(command2, "rectangle") == 0) {
        // I read the dimensions of the rectangle from the stdin
        int y1 = 0, x1 = 0, y2 = 0, x2 = 0, y3 = 0, x3 = 0, y4 = 0, x4 = 0, width = 0, height = 0;
        scanf("%d", &y1);
        scanf("%d", &x1);
        scanf("%d", &width);
        scanf("%d", &height);

        x2 = x1 + height;
        y2 = y1;
        x3 = x2;
        y3 = y1 + width;
        y4 = y3;
        x4 = x1;
        // I join the vertices of the rectangle and color the pixels contained by the line

        for (int i = -pencil->dimension / 2; i <= pencil->dimension / 2; i++) {
            for (int j = -pencil->dimension/2; j <= pencil->dimension/2; j++) {
                color(image, pencil, y1 + i, x1 + j, y2 + i, x2 + j);
            }
        }
        for (int i = -pencil->dimension / 2; i <= pencil->dimension / 2; i++) {
            for (int j = -pencil->dimension / 2; j <= pencil->dimension / 2; j++) {
                color(image, pencil, y2 + i, x2 + j, y3 + i, x3 + j);
            }
        }
        for (int i = -pencil->dimension / 2; i <= pencil->dimension / 2; i++) {
            for (int j = -pencil->dimension / 2; j <= pencil->dimension / 2; j++) {
                color(image, pencil, y3 + i, x3 + j, y4 + i, x4 + j);
            }
        }
        for (int i = -pencil->dimension / 2; i <= pencil->dimension / 2; i++) {
            for (int j = -pencil->dimension / 2; j <= pencil->dimension / 2; j++) {
                color(image, pencil, y4 + i, x4 + j, y1 + i, x1 + j);
            }
        }
        return image;
    }

    if (strcmp(command2, "triangle") == 0) {
        // I read the dimensions of the triangle from the stdin
        int y1 = 0, x1 = 0, y2 = 0, x2 = 0, y3 = 0, x3 = 0;
        scanf("%d", &y1);
        scanf("%d", &x1);
        scanf("%d", &y2);
        scanf("%d", &x2);
        scanf("%d", &y3);
        scanf("%d", &x3);
        // I join the vertices of the triangle and color the pixels contained by the line
        for (int i = -pencil->dimension / 2; i <= pencil->dimension / 2; i++) {
            for (int j = -pencil->dimension / 2; j <= pencil->dimension / 2; j++) {
                color(image, pencil, y1 + i, x1 + j, y2 + i, x2 + j);
            }
        }
        for (int i = -pencil->dimension / 2; i <= pencil->dimension / 2; i++) {
            for (int j = -pencil->dimension / 2; j <= pencil->dimension / 2; j++) {
                color(image, pencil, y2 + i, x2 + j, y3 + i, x3 + j);
            }
        }
        for (int i = -pencil->dimension / 2; i <= pencil->dimension / 2; i++) {
            for (int j = -pencil->dimension / 2; j <= pencil->dimension / 2; j++) {
                color(image, pencil, y1 + i, x1 + j, y3 + i, x3 + j);
            }
        }
    }
    return image;
}

BMP *insert(BMP *image) {
    char *calea = (char *)malloc(MAX_NR_CH_COMMAND * sizeof(char));
    if (calea == NULL) {
        printf("Allocation didn't take place!\n");
        return NULL;
    }
    scanf("%s", calea);
    int x = 0, y = 0, end_y = 0, end_x = 0;
    scanf("%d", &x);
    scanf("%d", &y);

    // I open and read the file that needs to be inserted
    FILE *in_insert = fopen(calea, "rb");
    BMP *image_to_insert = read_bmp(in_insert);
    free(calea);
    fclose(in_insert);

    int width1 = image->infoheader->width;
    int height1 = image->infoheader->height;
    int width2 = image_to_insert->infoheader->width;
    int height2 = image_to_insert->infoheader->height;


    if (y + height2 > height1) {
       end_y = height1;
    } else {
        end_y = y + height2;
    }
    if (x + width2 > width1) {
       end_x = width1;
    } else {
        end_x = x + width2;
    }

    int n = 0, m = 0;
     for (int i = y; i < end_y; i++) {
        for (int j = x; j < end_x; j++) {
            image->bitmap->pixels[i][j] = image_to_insert->bitmap->pixels[n][m];
            m++;
        }
        m = 0;
        n++;
    }
    return image;
}

void save(BMP *image) {
    char *destination = (char *)malloc(MAX_NR_CH_COMMAND * sizeof(char));
    if (destination == NULL) {
        printf("Allocation didn't take place!\n");
        return;
    }

    scanf("%s", destination);
    FILE *out = fopen(destination, "wb");

    fwrite(image->fileheader, sizeof(bmp_fileheader), 1, out);
    fwrite(image->infoheader, sizeof(bmp_infoheader), 1, out);

    unsigned char octet_padding = 0;
    int padding = image->infoheader->width % 4;

    for (int i = 0; i < image->infoheader->height; i++) {
        for (int j = 0; j < image->infoheader->width; j++) {
            fwrite(&(image->bitmap->pixels[i][j]), sizeof(Pixel), 1, out);
        }
        for (int j = 0; j < padding; j++) {
            fwrite(&octet_padding, sizeof(unsigned char), 1, out);
        }
    }

    fclose(out);
    free(destination);
}

void quit(BMP *image) {
    freeBMP(image);
}

int main() {
    char *command1 = (char *)malloc(MAX_NR_CH_COMMAND * sizeof(char));
    if (command1 == NULL) {
        printf("Allocation didn't take place!\n");
        return -1;
    }

    Pencil *pencil = (Pencil *)malloc(sizeof(pencil));
    if (pencil == NULL) {
        printf("Allocation didn't take place!\n");
        return -1;
    }

    BMP *image = NULL;

    // depending on the requested order, I perform a specific function
    while (scanf("%s", command1) != EOF) {
        if (strcmp(command1, "edit") == 0) {
            image = edit();
        }

        if (strcmp(command1, "set") == 0) {
            pencil = set(pencil);
        }

        if (strcmp(command1, "draw") == 0) {
            image = draw(image, pencil);
        }

        if (strcmp(command1, "insert") == 0) {
            image = insert(image);
        }

        if (strcmp(command1, "fill") == 0) {
            int y = 0, x = 0;
            scanf("%d", &y);
            scanf("%d", &x);
            char blue = image->bitmap->pixels[x][y].B;
            char red = image->bitmap->pixels[x][y].R;
            char green = image->bitmap->pixels[x][y].G;
            int i = x;
            int j = y;

            if (blue != pencil->b || green != pencil->g || red != pencil->r) {
                fill_matrice(image, blue, green, red, i, j, pencil);
            }
        }


        if (strcmp(command1, "save") == 0) {
            save(image);
        }

        if (strcmp(command1, "quit") == 0) {
            quit(image);
        }
    }
    free(command1);
    return 0;
}
