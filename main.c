#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>
#include <string.h>
#include <math.h>


#define WIDTH 900
#define HEIGHT 600

#define abs(number) (number<0?number*-1:number)

typedef struct Point{
    int x;
    int y;
} Point;

typedef struct Color{
    int r;
    int g;
    int b;
} Color;

typedef Color Canvas[HEIGHT][WIDTH];

static Canvas canvas;

void saveAsPPM(const char *filepath){
    FILE *file = fopen(filepath, "wb");
    if(!file) exit(1);

    fprintf(file, "P6\n%i %i 255\n", WIDTH, HEIGHT);

    int y; for(y = 0; y < HEIGHT; ++y) {
        int x; for(x = 0; x < WIDTH; ++x) {
            Color pixel = canvas[y][x];
            uint8_t bytes[3] = {
                pixel.r,
                pixel.g,
                pixel.b
            };

            fwrite(bytes, sizeof(bytes), 1, file);
        }
    }

    fclose(file);
}

void draw_line(int x0, int y0, int x1, int y1, Color color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;

    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float Xinc = dx / (float)steps;
    float Yinc = dy / (float)steps;

    float X = x0;
    float Y = y0;
    int i; for(i = 0; i <= steps; i++) {
        canvas[(int)Y][(int)X] = color;
        X += Xinc;
        Y += Yinc;
    }
}

void set_background(Color color)
{
    int y; for(y = 0; y < HEIGHT; ++y) {
        int x; for(x = 0; x < WIDTH; ++x) {
            canvas[y][x] = color;
        }
    }
}

int calcX(int x0, float distance, float rotation)
{
    return x0 + (int)(distance * cos(rotation));
}

int calcY(int y0, float distance, float rotation)
{
    return y0 + (int)(distance * -sin(rotation));
}

float rotation = M_PI/2;

void render_fractal_tree(int x0, int y0, float distance)
{
    int newX = calcX(x0, distance, rotation);
    int newY = calcY(y0, distance, rotation);
    newX = newX>0?newX<WIDTH?newX:WIDTH:0;
    newY = newY>0?newY<HEIGHT?newY:HEIGHT:0;
    draw_line(x0, y0, newX, newY, (Color) {255, 255, 255});
    float old_rotation = rotation;
    rotation += M_PI/8;
    if(distance > 8){
        render_fractal_tree(newX, newY, distance*0.85);
    }
    rotation = old_rotation - M_PI/8;
    if(distance > 8){
        render_fractal_tree(newX, newY, distance*0.85);
    }
}

int main(void)
{
    set_background((Color) {0, 0, 0});
    render_fractal_tree(WIDTH/2, HEIGHT, 100);
    saveAsPPM("canvas.ppm");
    return 0;
}
