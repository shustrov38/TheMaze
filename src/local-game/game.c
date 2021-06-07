#include "game.h"
#include "utilities.h"
#include "maze_generator.h"
#include "playerInfo.h"

const int TILE_SIZE = 15;
const int SCREEN_HEIGHT = 750;
const int SCREEN_WIDTH = 900;

static int game_status;

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

    int iconNum;
    int iconUsing[iconsCnt];
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

    for (int i = 0; i < playersCnt; ++i){
        players[i].login = (char*) malloc (20 * sizeof(char));
    }

    return players;
}


static void Process_login() {
    TTF_Init();
    SDL_Event event;
    game_status = GAME_LOGIN;

    SDL_Surface *login_background = Load_img("../../../src/local-game/Textures/login_var1.jpg");
    //SDL_Surface *scaled_main_background = ScaleSurface(main_background, SCREEN_WIDTH, SCREEN_HEIGHT);
    Draw_image(screen, login_background, 0, 0);

    SDL_Surface *field1 = Load_img("../../../src/local-game/Textures/login_field1.png");
    SDL_Surface *field2 = Load_img("../../../src/local-game/Textures/login_field2.png");
    //SDL_Surface *login_password = Load_img("../../../src/local-game/Textures/login&password.png");
    //SDL_Surface *scaled_login_password = ScaleSurface(login_password, SCREEN_WIDTH, SCREEN_HEIGHT);
    //Draw_image(screen, login_password, 0, 0);

    int done = 0;
    char temp[128];
    int temp_size = 0;

    char login[128];
    char password[128];

    int enterCnt = 0;
    memset(temp, 0, 128);
    memset(login, 0, 128);
    memset(password, 0, 128);


    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    KEY_CASE(SDLK_a, 'A', 'a');
                    KEY_CASE(SDLK_b, 'B', 'b');
                    KEY_CASE(SDLK_c, 'C', 'c');
                    KEY_CASE(SDLK_d, 'D', 'd');
                    KEY_CASE(SDLK_e, 'E', 'e');
                    KEY_CASE(SDLK_f, 'F', 'f');
                    KEY_CASE(SDLK_g, 'G', 'g');
                    KEY_CASE(SDLK_h, 'H', 'h');
                    KEY_CASE(SDLK_i, 'I', 'i');
                    KEY_CASE(SDLK_j, 'J', 'j');
                    KEY_CASE(SDLK_k, 'K', 'k');
                    KEY_CASE(SDLK_l, 'L', 'l');
                    KEY_CASE(SDLK_m, 'M', 'm');
                    KEY_CASE(SDLK_n, 'N', 'n');
                    KEY_CASE(SDLK_o, 'O', 'o');
                    KEY_CASE(SDLK_p, 'P', 'p');
                    KEY_CASE(SDLK_q, 'Q', 'q');
                    KEY_CASE(SDLK_r, 'R', 'r');
                    KEY_CASE(SDLK_s, 'S', 's');
                    KEY_CASE(SDLK_t, 'T', 't');
                    KEY_CASE(SDLK_u, 'U', 'u');
                    KEY_CASE(SDLK_v, 'V', 'v');
                    KEY_CASE(SDLK_w, 'W', 'w');
                    KEY_CASE(SDLK_x, 'X', 'x');
                    KEY_CASE(SDLK_y, 'Y', 'y');
                    KEY_CASE(SDLK_z, 'Z', 'z');

                    KEY_CASE(SDLK_0, '0', '0');
                    KEY_CASE(SDLK_1, '1', '1');
                    KEY_CASE(SDLK_2, '2', '2');
                    KEY_CASE(SDLK_3, '3', '3');
                    KEY_CASE(SDLK_4, '4', '4');
                    KEY_CASE(SDLK_5, '5', '5');
                    KEY_CASE(SDLK_6, '6', '6');
                    KEY_CASE(SDLK_7, '7', '7');
                    KEY_CASE(SDLK_8, '8', '8');
                    KEY_CASE(SDLK_9, '9', '9');

                    case SDLK_BACKSPACE:
                        if (temp_size != 0) {
                            temp[--temp_size] = 0;
                        }
                        break;
                    case SDLK_SPACE:
                        if (temp_size == 0) {
                            break;
                        }
                        enterCnt++;
                        if (enterCnt == 1) {
                            strcpy(login, temp);
                            memset(temp, 0, 128);
                            temp_size = 0;
                        } else if (enterCnt == 2) {
                            strcpy(password, temp);
                            done = 1;
                            game_status = GAME_MENU;
                        }
                        break;
                }
                if (done) {
                    break;
                }
            }

            Draw_image(screen, login_background, 0, 0);

            if (enterCnt == 0) {
                Draw_image(screen, field1, 496, 172);
                WriteText(512, 172, temp, 28, 0, 0, 0);
            } else if (enterCnt == 1) {
                Draw_image(screen, field2, 496, 221);
                WriteText(512, 172, login, 28, 0, 0, 0);
                WriteText(512, 221, temp, 28, 0, 0, 0);
            }
            Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        }


        if (event.type == SDL_QUIT) {
            SDL_Quit();
            exit(0);
        }

    }

    printf("\nUser login: %s\npassword: %s", login, password);

}

static void Process_menu(){
    game_status = GAME_MENU;
    SDL_Surface *menu_backgroud = Load_img("../../../src/local-game/Textures/var1.jpg");











}

static void Process_exit_game() {
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

//login
    Init_window("Maze", SCREEN_WIDTH, SCREEN_HEIGHT);
    Process_login();
//login

//menu
    Process_menu();


//menu


//textures
    //SDL_Surface *background = Load_img("D:\\AllCodes\\TheMaze\\src\\local-game\\Textures\\wall2.jpg");
    //SDL_Surface *scaled_background = ScaleSurface(background,750,750);

    SDL_Surface *wall = Load_img("../../../src/local-game/Textures/maze_wall3.bmp");
    SDL_Surface *scaled_wall = ScaleSurface(wall, TILE_SIZE, TILE_SIZE);

    SDL_Surface *floor = Load_img("../../../src/local-game/Textures/maze_floor.bmp");
    SDL_Surface *scaled_floor = ScaleSurface(floor, TILE_SIZE + 6, TILE_SIZE + 6);

    SDL_Surface *exitPoint = Load_img(
            "../../../src/local-game/Textures/maze_final-point.png");
    SDL_Surface *scaled_exitPoint = ScaleSurface(exitPoint, TILE_SIZE + 5, TILE_SIZE + 5);
//textures

//characters
    int iconsCnt = 15;
    SDL_Surface **ICONS = initICONS();
//characters

    playerPos *players = initAllPlayers(playersCnt, ICONS, iconsCnt);


    int height = MAZE_SIZE;
    int width = MAZE_SIZE;
    int **maze;
    int x_finishPoint; int y_finishPoint;

//maze generation
    maze = initMaze(maze, width, height);
    maze = generator(maze, 0, width - 1, 0, height - 1);
    maze = createFinishPoint(maze, width, height);


    if (GAME_RUNNING == game_status) {

        Draw_rectangle(screen, BLACK_BLOCK, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                if (maze[i][j] == 1) {
                    Draw_image(screen, scaled_wall, TILE_SIZE * i, TILE_SIZE * j);
                }
            }
        }

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
                    x_finishPoint = i;
                    y_finishPoint = j;
                }
            }
        }

        //begin positions
        for (int i = 0; i < playersCnt; ++i) {
            Draw_image(screen, players[i].icon, (players[i].X * TILE_SIZE) - 3, (players[i].Y * TILE_SIZE) - 3);
        }
        Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
//maze generation


//game (movements of players)
    while (game_status != GAME_OVER) {
        Process_exit_game();
        if (game_status == GAME_OVER) break;
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
        showPlayersInfo(players, playersCnt);
        Draw_image(screen, scaled_exitPoint, TILE_SIZE * x_finishPoint, TILE_SIZE * y_finishPoint);
        Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        SDL_Delay(125);
    }
//game (movements of players)

}