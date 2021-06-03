#ifndef THEMAZE_PLAYERINFO_H
#define THEMAZE_PLAYERINFO_H

#include "utilities.h"

typedef struct {
    int X;
    int Y;
    int Prev_X;
    int Prev_Y;
    SDL_Surface *icon;
} playerPos;



playerPos playerMoves(int **maze, playerPos player);
playerPos botMoves(int **maze, playerPos player);

#endif //THEMAZE_PLAYERINFO_H
