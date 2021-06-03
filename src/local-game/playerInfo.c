#include "playerInfo.h"

playerPos botMoves(int **maze, playerPos player){
    srand(time(NULL));

    int step = rand() % 4;

    if (step == 0){
        if (maze[player.X][player.Y - 1] != 1) {
            player.Prev_X = player.X;
            player.Prev_Y = player.Y;
            player.Y -= 1;
        }
    } else if (step == 1){
        if (maze[player.X - 1][player.Y] != 1) {
            player.Prev_X = player.X;
            player.Prev_Y = player.Y;
            player.X -= 1;
        }
    } else if (step == 2){
        if (maze[player.X + 1][player.Y] != 1) {
            player.Prev_X = player.X;
            player.Prev_Y = player.Y;
            player.X += 1;

        }
    } else if (step == 3){
        if (maze[player.X][player.Y + 1] != 1) {
            player.Prev_X = player.X;
            player.Prev_Y = player.Y;
            player.Y += 1;

        }
    }

    return player;
}

playerPos playerMoves(int **maze, playerPos player) {

    const Uint8 *curKey = SDL_GetKeyboardState(NULL);

    if (curKey[SDL_SCANCODE_UP] || curKey[SDL_SCANCODE_W]) {
        if (maze[player.X][player.Y - 1] != 1) {
            player.Prev_X = player.X;
            player.Prev_Y = player.Y;
            player.Y -= 1;

        }
    } else if (curKey[SDL_SCANCODE_LEFT] || curKey[SDL_SCANCODE_A]) {
        if (maze[player.X - 1][player.Y] != 1) {
            player.Prev_X = player.X;
            player.Prev_Y = player.Y;
            player.X -= 1;

        }
    } else if (curKey[SDL_SCANCODE_RIGHT] || curKey[SDL_SCANCODE_D]) {
        if (maze[player.X + 1][player.Y] != 1) {
            player.Prev_X = player.X;
            player.Prev_Y = player.Y;
            player.X += 1;

        }
    } else if (curKey[SDL_SCANCODE_DOWN] || curKey[SDL_SCANCODE_S]) {
        if (maze[player.X][player.Y + 1] != 1) {
            player.Prev_X = player.X;
            player.Prev_Y = player.Y;
            player.Y += 1;

        }
    }

    return player;
}

