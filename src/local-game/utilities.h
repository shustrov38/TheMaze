#ifndef THEMAZE_UTILITIES_H
#define THEMAZE_UTILITIES_H


#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <conio.h>

SDL_Window *window;
SDL_Surface *screen;

enum {
    GAME_STOPPING,
    GAME_RUNNING,
    GAME_OVER
};

int Init_window(const char *title, int width, int height);
SDL_Surface *Get_screen(void);
SDL_Window *Get_window(void);
int Update_window(void);
int Update_window_rect(int x, int y, unsigned int w, unsigned int h);
void CleanUp_window(void);
void CleanUp_screen(void);

SDL_Surface *Load_img(const char *filename);
SDL_Surface *ScaleSurface(SDL_Surface *image, size_t width, size_t height);
static SDL_Surface *ScaledCopy(SDL_Surface *src, SDL_Rect *dstSize);
int Draw_image(SDL_Surface *dest_surface, SDL_Surface *src_surface, size_t x, size_t y);



#endif //THEMAZE_UTILITIES_H
