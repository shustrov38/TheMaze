#ifndef THEMAZE_GAME_H
#define THEMAZE_GAME_H

#include "utilities.h"
#include "maze_generator.h"
#include "player.h"

typedef struct {
    int id;
    char *name;
    COORD coord;
    int color;
} playerInfo;

#endif //THEMAZE_GAME_H
