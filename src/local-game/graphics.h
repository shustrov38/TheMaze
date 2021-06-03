//
// Created by Alex on 02.06.2021.
//

#ifndef THEMAZE_GRAPHICS_H
#define THEMAZE_GRAPHICS_H

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <conio.h>

enum {
    GAME_STOPPING,
    GAME_RUNNING,
    GAME_OVER
};

int init();

void quit();

#endif //THEMAZE_GRAPHICS_H


