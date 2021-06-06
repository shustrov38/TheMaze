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

static void Process_exit_game() {
    SDL_Event event;

    while( SDL_PollEvent( &event ) != 0 ) {

        if (event.type == SDL_QUIT) {
            game_status = GAME_OVER;
            return;
        }
    }
}

#define KEY_CASE(letter,upper_sym, lower_sym) \
case letter:                                  \
    if(temp_size==15) break;\
    if(SDL_GetModState() & KMOD_SHIFT ) {\
        temp[temp_size++]=upper_sym;\
    } else {\
        temp[temp_size++]=lower_sym;\
    }\
    break

static void Process_menu() {
    TTF_Init();
    SDL_Event event;
    game_status = GAME_MENU;
    SDL_Surface *main_background = Load_img("../../../src/local-game/Textures/main_background.png");
    SDL_Surface *scaled_main_background = ScaleSurface(main_background, SCREEN_SIZE, SCREEN_SIZE);
    Draw_image(screen,main_background,0,0);
    SDL_Surface *login_password = Load_img("../../../src/local-game/Textures/login&password.png");
    SDL_Surface *scaled_login_password = ScaleSurface(login_password, SCREEN_SIZE, SCREEN_SIZE);
    Draw_image(screen,login_password,300,50);
    int done=0;
    char temp[128];
    char login[128];
    char password[128];
    int entercnt=0;
    memset(temp,0,128);
    memset(login,0,128);
    memset(password,0,128);
    int temp_size = 0;
    while(!done) {
        while (SDL_PollEvent(&event)) {
            if(event.type==SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    KEY_CASE(SDLK_a,'A','a');
                    KEY_CASE(SDLK_b,'B','b');
                    KEY_CASE(SDLK_c,'C','c');
                    KEY_CASE(SDLK_d,'D','d');
                    KEY_CASE(SDLK_e,'E','e');
                    KEY_CASE(SDLK_f,'F','f');
                    KEY_CASE(SDLK_g,'G','g');
                    KEY_CASE(SDLK_h,'H','h');
                    KEY_CASE(SDLK_i,'I','i');
                    KEY_CASE(SDLK_j,'J','j');
                    KEY_CASE(SDLK_k,'K','k');
                    KEY_CASE(SDLK_l,'L','l');
                    KEY_CASE(SDLK_m,'M','m');
                    KEY_CASE(SDLK_n,'N','n');
                    KEY_CASE(SDLK_o,'O','o');
                    KEY_CASE(SDLK_p,'P','p');
                    KEY_CASE(SDLK_q,'Q','q');
                    KEY_CASE(SDLK_r,'R','r');
                    KEY_CASE(SDLK_s,'S','s');
                    KEY_CASE(SDLK_t,'T','t');
                    KEY_CASE(SDLK_u,'U','u');
                    KEY_CASE(SDLK_v,'V','v');
                    KEY_CASE(SDLK_w,'W','w');
                    KEY_CASE(SDLK_x,'X','x');
                    KEY_CASE(SDLK_y,'Y','y');
                    KEY_CASE(SDLK_z,'Z','z');

                    KEY_CASE(SDLK_0,'0','0');
                    KEY_CASE(SDLK_1,'1','1');
                    KEY_CASE(SDLK_2,'2','2');
                    KEY_CASE(SDLK_3,'3','3');
                    KEY_CASE(SDLK_4,'4','4');
                    KEY_CASE(SDLK_5,'5','5');
                    KEY_CASE(SDLK_6,'6','6');
                    KEY_CASE(SDLK_7,'7','7');
                    KEY_CASE(SDLK_8,'8','8');
                    KEY_CASE(SDLK_9,'9','9');

                    case SDLK_BACKSPACE:
                        if(temp_size!=0) {
                            temp[--temp_size]=0;
                        }
                        break;
                    case SDLK_KP_ENTER:
                        if(temp_size==0) {
                            break;
                        }
                        entercnt++;
                        if(entercnt==1) {
                            strcpy(login, temp);
                            memset(temp,0,128);
                            temp_size=0;
                        }
                        else if(entercnt==2) {
                            strcpy(password, temp);
                            done=1;
                            game_status = GAME_RUNNING;
                        }
                        break;
                }
                if(done) {
                    break;
                }
            }

            Draw_image(screen,main_background,0,0);
            Draw_image(screen,login_password,0,0);
            if(entercnt==0) {
                WriteText(314,337,temp,28,0,0,0);
            }
            else if (entercnt==1) {
                WriteText(314,337,login,28,0,0,0);
                WriteText(314,380,temp,28,0,0,0);
            }
            Update_window_rect(0, 0, SCREEN_SIZE, SCREEN_SIZE);
        }



        if (event.type == SDL_QUIT) {
            SDL_Quit();
            exit(0);
        }


    }



//    if(!strcmp(login, "nigger")) {
//        SDL_Surface *dababy = Load_img("../../../src/local-game/Textures/dababy.jpg");
//        SDL_Surface *scaled_dababy = ScaleSurface(dababy, SCREEN_SIZE, SCREEN_SIZE);
//        Draw_image(screen,dababy,-150,-150);
//        WriteText(SCREEN_SIZE/2-150,SCREEN_SIZE/2,"LESS GOOOOOO",50,255,0,0);
//    }
//    else{
//        WriteText(SCREEN_SIZE/2-150,SCREEN_SIZE/2,"press space to play",30,255,0,0);
//    }
    //Update_window_rect(0, 0, SCREEN_SIZE, SCREEN_SIZE);

    printf("login: %s\npassword: %s", login, password);

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

    Init_window("Maze", SCREEN_SIZE, SCREEN_SIZE);
    Process_menu();

    //menu


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
    int height = MAZE_SIZE;
    int width = MAZE_SIZE;
    int **maze;

    //maze generation
    maze = initMaze(maze, width, height);
    maze = generator(maze, 0, width - 1, 0, height - 1);
    maze = createFinishPoint(maze, width, height);


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
        Update_window_rect(0, 0, SCREEN_SIZE, SCREEN_SIZE);

        SDL_Delay(125);
    }

}