#include "playerInfo.h"
#include "utilities.h"


const int TILE_SIZE_1 = 15;
const int SCREEN_HEIGHT_1 = 750;
const int SCREEN_WIDTH_1 = 900;


void showPlayersInfo(RECV_PL_INFO *player, int playersCnt){
    int pos_X = 810; int pos_Y = 10;

    Draw_rectangle(screen, BLACK_BLOCK, 750, 0, 150, 750);
    WriteText(770, 0, "Players", 28, 255, 255, 255);

    for (int i = 0; i < playersCnt; ++i) {
        char text1[50];
        SDL_Surface *icon = ScaleSurface(player[i].icon, TILE_SIZE_1 + 10, TILE_SIZE_1 + 10);
        Draw_image(screen, icon, pos_X - 40, pos_Y + 68);

        itoa(player[i].X, text1, 10);
        WriteText(pos_X, pos_Y + 40, "X:", 28, 255, 255, 255);
        WriteText(pos_X + 20, pos_Y + 40, text1, 28, 255, 255, 255);

        itoa(player[i].Y, text1, 10);
        WriteText(pos_X, pos_Y + 80, "Y:", 28, 255, 255, 255);
        WriteText(pos_X + 20, pos_Y + 80, text1, 28, 255, 255, 255);

        pos_Y += 100;
    }
    Update_window_rect(0, 0, SCREEN_WIDTH_1, SCREEN_HEIGHT_1);
}

//playerPos botMoves(int **maze, playerPos player){
//    srand(time(NULL));
//
//    int step = rand() % 4;
//
//    if (step == 0){
//        if (maze[player.X][player.Y - 1] != 1) {
//            player.Prev_X = player.X;
//            player.Prev_Y = player.Y;
//            player.Y -= 1;
//        }
//    } else if (step == 1){
//        if (maze[player.X - 1][player.Y] != 1) {
//            player.Prev_X = player.X;
//            player.Prev_Y = player.Y;
//            player.X -= 1;
//        }
//    } else if (step == 2){
//        if (maze[player.X + 1][player.Y] != 1) {
//            player.Prev_X = player.X;
//            player.Prev_Y = player.Y;
//            player.X += 1;
//
//        }
//    } else if (step == 3){
//        if (maze[player.X][player.Y + 1] != 1) {
//            player.Prev_X = player.X;
//            player.Prev_Y = player.Y;
//            player.Y += 1;
//
//        }
//    }
//
//    return player;
//}

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

