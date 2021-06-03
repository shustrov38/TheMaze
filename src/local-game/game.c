#include "game.h"
#include "utilities.h"
#include "maze_generator.h"
#include "playerInfo.h"


int TILE_SIZE = 15;
int SCREEN_SIZE = 750;

playerPos player;

playerPos initPlayer() {
    player.X = 1;
    player.Y = 1;
    return player;
}

static int game_status;
static Uint32 moveEventTime,
        lvlEventTime;


static void Process_events(int **maze) {
    SDL_Event event;
    SDL_Keycode keycode;
    Uint32 curTime = SDL_GetTicks();
    const Uint8 *curKey = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&event) != 0) {
        keycode = event.key.keysym.sym;

        if (event.type == SDL_QUIT) {
            game_status = GAME_OVER;
            return;
        }
    }

    //move of player
    if ((GAME_RUNNING == game_status) && (curTime - moveEventTime >= 20)) {
        moveEventTime = curTime;

        if (curKey[SDL_SCANCODE_UP] || curKey[SDL_SCANCODE_W]) {
            if (maze[player.X][player.Y - 1] != 1) {
                player.Prev_Y = player.Y;
                player.Y -= 1;
            }
        }
        if (curKey[SDL_SCANCODE_LEFT] || curKey[SDL_SCANCODE_A]) {
            if (maze[player.X - 1][player.Y] != 1) {
                player.Prev_X = player.X;
                player.X -= 1;
            }
        }
        if (curKey[SDL_SCANCODE_RIGHT] || curKey[SDL_SCANCODE_D]) {
            if (maze[player.X + 1][player.Y] != 1) {
                player.Prev_X = player.X;
                player.X += 1;
            }
        }
        if (curKey[SDL_SCANCODE_DOWN] || curKey[SDL_SCANCODE_S]) {
            if (maze[player.X][player.Y + 1] != 1) {
                player.Prev_Y = player.Y;
                player.Y += 1;
            }
        }
    }
}
int checkFinishPoint(int **maze){
   if (maze[player.X][player.Y] == 5) {
       return GAME_OVER;
   } else {
       return GAME_RUNNING;
   }
}

int WinMain(int argc, char *argv[]) {
    srand(time(NULL));


    //SDL_Surface *background = Load_img("D:\\AllCodes\\TheMaze\\src\\local-game\\Textures\\wall2.jpg");
    //SDL_Surface *scaled_background = ScaleSurface(background,750,750);

    SDL_Surface *wall = Load_img("D:\\AllCodes\\TheMaze\\src\\local-game\\Textures\\wall.jpg");
    SDL_Surface *scaled_wall = ScaleSurface(wall, TILE_SIZE, TILE_SIZE);

    SDL_Surface *black = Load_img("D:\\AllCodes\\TheMaze\\src\\local-game\\Textures\\black.bmp");
    SDL_Surface *scaled_black = ScaleSurface(black, TILE_SIZE, TILE_SIZE);

    SDL_Surface *exitPoint = Load_img(
            "D:\\AllCodes\\TheMaze\\src\\local-game\\Textures\\Regeneration_Rune_minimap_icon.png");
    SDL_Surface *scaled_exitPoint = ScaleSurface(exitPoint, TILE_SIZE + 5, TILE_SIZE + 5);

    SDL_Surface *playerIcon = Load_img(
            "D:\\AllCodes\\TheMaze\\src\\local-game\\Textures\\Phantom_Assassin_minimap_icon.png");
    SDL_Surface *scaled_playerIcon = ScaleSurface(playerIcon, TILE_SIZE, TILE_SIZE);


    initPlayer();

    moveEventTime = SDL_GetTicks();
    lvlEventTime = moveEventTime;

    game_status = GAME_RUNNING;

    int height = MAZE_SIZE;
    int width = MAZE_SIZE;
    int **maze;

    maze = initMaze(maze, width, height);
    maze = generator(maze, 0, width - 1, 0, height - 1);

    maze = createFinishPoint(maze, width, height);

    Init_window("Maze", SCREEN_SIZE, SCREEN_SIZE);

    Draw_image(screen, scaled_playerIcon, player.X * TILE_SIZE, player.Y * TILE_SIZE);
    Update_window_rect(0, 0, SCREEN_SIZE, SCREEN_SIZE);

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

        Update_window_rect(0, 0, SCREEN_SIZE, SCREEN_SIZE);
    }

    while (game_status != GAME_OVER){
        Process_events(maze);
        game_status = checkFinishPoint(maze);
        Draw_image(screen, scaled_playerIcon, TILE_SIZE * player.X, TILE_SIZE * player.Y);
        Update_window_rect(0, 0, SCREEN_SIZE, SCREEN_SIZE);
        SDL_Delay(150);
    }

}