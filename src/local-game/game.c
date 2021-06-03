#include "game.h"
#include "utilities.h"
#include "maze_generator.h"
#include "playerInfo.h"


playerPos *initAllPlayers(int playersCnt) {
    playerPos *players = (playerPos*) malloc (playersCnt * sizeof(playerPos));

    for (int i = 0; i < playersCnt; ++i) {
        if (i == 0) {
            players[i].X = 1;
            players[i].Y = 1;
            players[i].Prev_X = 1;
            players[i].Prev_Y = 1;
        } else if (i == 1){
            players[i].X = MAZE_SIZE - 2;
            players[i].Y = MAZE_SIZE - 2;
            players[i].Prev_X = MAZE_SIZE - 2;
            players[i].Prev_Y = MAZE_SIZE - 2;
        } else if (i == 2){
            players[i].X = MAZE_SIZE - 2;
            players[i].Y = 1;
            players[i].Prev_X = MAZE_SIZE - 2;
            players[i].Prev_Y = 1;
        } else if (i == 3){
            players[i].X = 1;
            players[i].Y = MAZE_SIZE - 2;
            players[i].Prev_X = 1;
            players[i].Prev_Y = MAZE_SIZE - 2;
        }
    }

    return players;
}

int TILE_SIZE = 15;
int SCREEN_SIZE = 750;


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

    playerPos *players = initAllPlayers(playersCnt);

//textures
    //SDL_Surface *background = Load_img("D:\\AllCodes\\TheMaze\\src\\local-game\\Textures\\wall2.jpg");
    //SDL_Surface *scaled_background = ScaleSurface(background,750,750);

    SDL_Surface *wall = Load_img("../../../src/local-game/Textures/wall.jpg");
    SDL_Surface *scaled_wall = ScaleSurface(wall, TILE_SIZE, TILE_SIZE);

    SDL_Surface *black = Load_img("D:\\AllCodes\\TheMaze\\src\\local-game\\Textures\\black.bmp");
    SDL_Surface *scaled_black = ScaleSurface(black, TILE_SIZE, TILE_SIZE);

    SDL_Surface *exitPoint = Load_img(
            "D:\\AllCodes\\TheMaze\\src\\local-game\\Textures\\Regeneration_Rune_minimap_icon.png");
    SDL_Surface *scaled_exitPoint = ScaleSurface(exitPoint, TILE_SIZE + 5, TILE_SIZE + 5);

    SDL_Surface *playerIcon = Load_img(
            "D:\\AllCodes\\TheMaze\\src\\local-game\\Textures\\Phantom_Assassin_minimap_icon.png");
    SDL_Surface *scaled_playerIcon = ScaleSurface(playerIcon, TILE_SIZE, TILE_SIZE);
//textures

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
                } else if (maze[i][j] == 5) {
                    Draw_image(screen, scaled_exitPoint, TILE_SIZE * i, TILE_SIZE * j);
                } else if (maze[i][j] == 0){

                }
            }
        }

        //begin positions
        for (int i = 0; i < playersCnt; ++i) {
            Draw_image(screen, scaled_playerIcon, players[i].X * TILE_SIZE, players[i].Y * TILE_SIZE);
        }
        Update_window_rect(0, 0, SCREEN_SIZE, SCREEN_SIZE);
    }


    while (game_status != GAME_OVER){
        Process_events();
        players[0] = playerMoves(maze, players[0]);
        game_status = checkFinishPoint(players[0], maze);
        Draw_image(screen, scaled_black, TILE_SIZE * players[0].Prev_X, TILE_SIZE * players[0].Prev_Y);
        Draw_image(screen, scaled_playerIcon, TILE_SIZE * players[0].X, TILE_SIZE * players[0].Y);
        Update_window_rect(0, 0, SCREEN_SIZE, SCREEN_SIZE);

        SDL_Delay(125);
    }

}