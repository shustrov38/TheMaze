#include "game.h"
#include "utilities.h"
#include "maze_generator.h"

static int game_status;
static Uint32 moveEventTime,
            lvlEventTime;

static void Process_events(void) {
    SDL_Event event;
    SDL_Keycode keycode;
    Uint32 curTime = SDL_GetTicks();
    const Uint8* curKey = SDL_GetKeyboardState( NULL );

    while( SDL_PollEvent( &event ) != 0 ) {
        keycode = event.key.keysym.sym;

        if (event.type == SDL_QUIT) {
            game_status = GAME_OVER;
            return;
        }
    }

    //move of player
//    if ((GAME_RUNNING == game_status) && (curTime - moveEventTime >= 20)){
//        moveEventTime = curTime;
//
//        if (curKey[SDL_SCANCODE_UP]		|| curKey[SDL_SCANCODE_W])	moveUp();
//        if (curKey[SDL_SCANCODE_LEFT]	|| curKey[SDL_SCANCODE_A])	moveLeft();
//        if (curKey[SDL_SCANCODE_RIGHT]	|| curKey[SDL_SCANCODE_D])	moveRight();
//        if (curKey[SDL_SCANCODE_DOWN]   || curKey[SDL_SCANCODE_S])  moveDown();
//    }

}



int WinMain(int argc, char *argv[]) {
    srand(time(NULL));
    int TILE_SIZE = 15;
    int SCREEN_SIZE = 750;

    //SDL_Surface *background = Load_img("D:\\AllCodes\\TheMaze\\src\\local-game\\Textures\\wall2.jpg");
    //SDL_Surface *scaled_background = ScaleSurface(background,750,750);

    SDL_Surface *wall = Load_img("D:\\AllCodes\\TheMaze\\src\\local-game\\Textures\\wall.jpg");
    SDL_Surface *scaled_wall = ScaleSurface(wall, TILE_SIZE, TILE_SIZE);

    SDL_Surface *exitPoint = Load_img(
            "D:\\AllCodes\\TheMaze\\src\\local-game\\Textures\\Regeneration_Rune_minimap_icon.png");
    SDL_Surface *scaled_exitPoint = ScaleSurface(exitPoint, TILE_SIZE + 5, TILE_SIZE + 5);

    //SDL_Surface *playerIcon = Load_img("D:\\AllCodes\\TheMaze\\src\\local-game\\Textures\\Phantom_Assassin_minimap_icon.png");
    //SDL_Surface *scaled_playerIcon = ScaleSurface(playerIcon, TILE_SIZE+5, TILE_SIZE+5);


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


    while (GAME_OVER != game_status) {
        Process_events();

        if (GAME_RUNNING == game_status) {

            //SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
            //Draw_image(screen, scaled_background, 0, 0);

            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    if (maze[i][j] == 1) {
                        Draw_image(screen, scaled_wall, TILE_SIZE * i, TILE_SIZE * j);
                    } else if (maze[i][j] == 5) {
                        Draw_image(screen, scaled_exitPoint, TILE_SIZE * i, TILE_SIZE * j);
                    }
                }
            }

            Update_window_rect(0, 0, SCREEN_SIZE, SCREEN_SIZE);
        }
    }
}