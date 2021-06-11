#include "playerInfo.h"
#include "utilities.h"


const int TILE_SIZE_1 = 15;
const int SCREEN_HEIGHT_1 = 750;
const int SCREEN_WIDTH_1 = 900;


void showPlayersInfo(RECV_PL_INFO *player, int playersCnt){
    int pos_X = 810; int pos_Y = 10;

    Draw_rectangle(screen, BLACK_BLOCK, 750, 0, 150, 750);
    WriteText(770, 0, "Players", 28, 255, 255, 255);

    for (int i = 0; i < pl_render_infoCnt; ++i) {
        char text1[50];
        SDL_Surface *icon = ScaleSurface(player[i].icon, TILE_SIZE_1 + 10, TILE_SIZE_1 + 10);
        WriteText(pos_X - 35, pos_Y + 40, player[i].NAME, 24, 255, 255, 255);
        Draw_image(screen, icon, pos_X - 60, pos_Y + 44);

        double dest = sqrt((24.5 - player[i].X)*(24.5 - player[i].X) + (24.5 - player[i].Y)*(24.5 - player[i].Y));

        sprintf(text1, "POS: (%d : %d)", player[i].X, player[i].Y);
        WriteText(pos_X - 55, pos_Y + 70, text1, 24, 255, 255, 255);
        memset(text1, 0, 50);

        sprintf(text1, "AIM: %.2f", dest);
        WriteText(pos_X - 55, pos_Y + 100, text1, 24, 255, 255, 255);

        pos_Y += 120;
    }
    Update_window_rect(0, 0, SCREEN_WIDTH_1, SCREEN_HEIGHT_1);
}


RECV_PL_INFO playerMoves(int **maze, RECV_PL_INFO player, int curPos) {
    int changesFlag = 0;
    const Uint8 *curKey = SDL_GetKeyboardState(NULL);

    if (curKey[SDL_SCANCODE_UP] || curKey[SDL_SCANCODE_W]) {
        if (maze[player.X][player.Y - 1] != 1) {
            player.X_prev = player.X;
            player.Y_prev = player.Y;
            player.Y -= 1;
        }
    } else if (curKey[SDL_SCANCODE_LEFT] || curKey[SDL_SCANCODE_A]) {
        if (maze[player.X - 1][player.Y] != 1) {
            player.X_prev = player.X;
            player.Y_prev = player.Y;
            player.X -= 1;
        }
    } else if (curKey[SDL_SCANCODE_RIGHT] || curKey[SDL_SCANCODE_D]) {
        if (maze[player.X + 1][player.Y] != 1) {
            player.X_prev = player.X;
            player.Y_prev = player.Y;
            player.X += 1;
        }
    } else if (curKey[SDL_SCANCODE_DOWN] || curKey[SDL_SCANCODE_S]) {
        if (maze[player.X][player.Y + 1] != 1) {
            player.X_prev = player.X;
            player.Y_prev = player.Y;
            player.Y += 1;
        }
    }
        GO(player.X, player.Y);
    return player;
}

