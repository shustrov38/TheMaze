#include "game.h"
#include "utilities.h"
#include "maze_generator.h"
#include "playerInfo.h"


playerPos *initAllPlayers(int playersCnt, SDL_Surface **icons, int iconsCnt) {
    playerPos *players = (playerPos *) malloc(playersCnt * sizeof(playerPos));

    for (int i = 0; i < playersCnt; ++i) {
        if (i == 0) {
            players[i].X = 1;
            players[i].Y = 1;
            players[i].Prev_X = 1;
            players[i].Prev_Y = 1;
        } else if (i == 1) {
            players[i].X = MAZE_SIZE - 2;
            players[i].Y = MAZE_SIZE - 2;
            players[i].Prev_X = MAZE_SIZE - 2;
            players[i].Prev_Y = MAZE_SIZE - 2;
        } else if (i == 2) {
            players[i].X = MAZE_SIZE - 2;
            players[i].Y = 1;
            players[i].Prev_X = MAZE_SIZE - 2;
            players[i].Prev_Y = 1;
        } else if (i == 3) {
            players[i].X = 1;
            players[i].Y = MAZE_SIZE - 2;
            players[i].Prev_X = 1;
            players[i].Prev_Y = MAZE_SIZE - 2;
        }
    }

    int iconNum; int iconUsing[iconsCnt];
    for (int i = 0; i < iconsCnt; ++i) iconUsing[i] = 0;

    for (int i = 0; i < playersCnt; ++i) {
        iconNum = rand() % iconsCnt;
        if (iconUsing[iconNum] == 0) {
            players[i].icon = icons[iconNum];
            iconUsing[iconNum]++;
        } else {
            i--;
        }
    }

    return players;
}

const int TILE_SIZE = 15;
const int ICON_SIZE = TILE_SIZE + 6;
const int SCREEN_SIZE = 750;


static int game_status;
static Uint32 moveEventTime,
        lvlEventTime;


static void Process_events() {
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            game_status = GAME_OVER;
            return;
        }
    }
}

int checkFinishPoint(playerPos player, int **maze) {
    if (maze[player.X][player.Y] == 5) {
        return GAME_OVER;
    } else {
        return GAME_RUNNING;
    }
}


int WinMain(int argc, char *argv[]) {
    srand(time(NULL));

    int playersCnt;
    printf("Enter players count: ");
    scanf("%d", &playersCnt);


//textures
    //SDL_Surface *background = Load_img("D:\\AllCodes\\TheMaze\\src\\local-game\\Textures\\wall2.jpg");
    //SDL_Surface *scaled_background = ScaleSurface(background,750,750);

    SDL_Surface *wall = Load_img("../../../src/local-game/Textures/wall3.bmp");
    SDL_Surface *scaled_wall = ScaleSurface(wall, TILE_SIZE, TILE_SIZE);

    SDL_Surface *floor = Load_img("../../../src/local-game/Textures/floor.bmp");
    SDL_Surface *scaled_floor = ScaleSurface(floor, TILE_SIZE + 6, TILE_SIZE + 6);

    SDL_Surface *exitPoint = Load_img(
            "../../../src/local-game/Textures/final-point.png");
    SDL_Surface *scaled_exitPoint = ScaleSurface(exitPoint, TILE_SIZE + 5, TILE_SIZE + 5);
//textures

//characters
    int iconsCnt = 15;
    SDL_Surface **ICONS = (SDL_Surface **) malloc(15 * sizeof(SDL_Surface *));

    ICONS[0] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/anti-mage.png"), ICON_SIZE, ICON_SIZE);

    ICONS[1] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/bristleback.png"), ICON_SIZE, ICON_SIZE);

    ICONS[2] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/invoker.png"), ICON_SIZE, ICON_SIZE);

    ICONS[3] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/juggernaut.png"), ICON_SIZE, ICON_SIZE);

    ICONS[4] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/monkey-king.png"), ICON_SIZE, ICON_SIZE);

    ICONS[5] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/morph.png"), ICON_SIZE, ICON_SIZE);

    ICONS[6] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/phantom-assasin.png"), ICON_SIZE, ICON_SIZE);

    ICONS[7] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/pidor.png"), ICON_SIZE, ICON_SIZE);

    ICONS[8] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/queen-of-pain.png"), ICON_SIZE, ICON_SIZE);

    ICONS[9] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/rudge.png"), ICON_SIZE, ICON_SIZE);

    ICONS[10] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/shadow-fiend.png"), ICON_SIZE, ICON_SIZE);

    ICONS[11] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/templar_assassin.png"), ICON_SIZE, ICON_SIZE);

    ICONS[12] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/void-spirit.png"), ICON_SIZE, ICON_SIZE);

    ICONS[13] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/zeus.png"), ICON_SIZE, ICON_SIZE);

    ICONS[14] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/windranger.png"), ICON_SIZE, ICON_SIZE);
//characters

    playerPos *players = initAllPlayers(playersCnt, ICONS, iconsCnt);


    moveEventTime = SDL_GetTicks();
    lvlEventTime = moveEventTime;

    game_status = GAME_RUNNING;

    int height = MAZE_SIZE;
    int width = MAZE_SIZE;
    int **maze;

    //maze generation
    maze = initMaze(maze, width, height);
    maze = generator(maze, 0, width - 1, 0, height - 1);
    maze = createFinishPoint(maze, width, height);
    Init_window("Maze", SCREEN_SIZE, SCREEN_SIZE);


    if (GAME_RUNNING == game_status) {

        //SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        //Draw_image(screen, scaled_background, 0, 0);

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                if (maze[i][j] == 1) {
                    Draw_image(screen, scaled_wall, TILE_SIZE * i, TILE_SIZE * j);
                }
            }
        }

\
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                if (maze[i][j] == 0 || maze[i][j] == 2) {
                    Draw_image(screen, scaled_floor, TILE_SIZE * i - 3, TILE_SIZE * j - 3);
                }
            }
        }

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                if (maze[i][j] == 5) {
                    Draw_image(screen, scaled_exitPoint, TILE_SIZE * i, TILE_SIZE * j);
                }
            }
        }

        //begin positions
        for (int i = 0; i < playersCnt; ++i) {
            Draw_image(screen, players[i].icon, (players[i].X * TILE_SIZE) - 3, (players[i].Y * TILE_SIZE) - 3);
        }
        Update_window_rect(0, 0, SCREEN_SIZE, SCREEN_SIZE);
    }


    while (game_status != GAME_OVER){
        Process_events();
        players[0] = playerMoves(maze, players[0]);
        for (int i = 1; i < playersCnt; ++i) {
            players[i] = botMoves(maze, players[i]);
        }

        for (int i = 0; i < playersCnt; ++i) {
            game_status = checkFinishPoint(players[i], maze);
            if (game_status == GAME_OVER) break;
            Draw_image(screen, scaled_floor, (TILE_SIZE * players[i].Prev_X) - 3, (TILE_SIZE * players[i].Prev_Y) - 3);
            Draw_image(screen, players[i].icon, (TILE_SIZE * players[i].X) - 3, (TILE_SIZE * players[i].Y) - 3);
        }
        Update_window_rect(0, 0, SCREEN_SIZE, SCREEN_SIZE);

        SDL_Delay(125);
    }

}