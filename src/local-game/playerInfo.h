#ifndef THEMAZE_PLAYERINFO_H
#define THEMAZE_PLAYERINFO_H

#include "utilities.h"

typedef struct {
    int X;
    int Y;
    int Prev_X;
    int Prev_Y;
    SDL_Surface *icon;
    char *login;
} playerPos;


void showPlayersInfo(RECV_PL_INFO *player, int playersCnt);
RECV_PL_INFO playerMoves(int **maze, RECV_PL_INFO player, int curPos);
playerPos botMoves(int **maze, RECV_PL_INFO player);

#endif //THEMAZE_PLAYERINFO_H
