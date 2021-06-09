#include "game.h"
#include "utilities.h"
#include "maze_generator.h"
#include "playerInfo.h"

const int TILE_SIZE = 15;
const int SCREEN_HEIGHT = 750;
const int SCREEN_WIDTH = 900;

static int game_status;

char login[128];
char password[128];
char curPlayerRoom[50];

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

    for (int i = 0; i < playersCnt; ++i) {
        players[i].login = (char *) malloc(20 * sizeof(char));
    }

    return players;
}


static void Process_login() {
    TTF_Init();
    SDL_Event event;
    game_status = GAME_LOGIN;

    SDL_Surface *login_background = Load_img("../../../src/local-game/Textures/login/login_var1.jpg");
    //SDL_Surface *scaled_main_background = ScaleSurface(main_background, SCREEN_WIDTH, SCREEN_HEIGHT);
    Draw_image(screen, login_background, 0, 0);

    SDL_Surface *field1 = Load_img("../../../src/local-game/Textures/login/login_field1.png");
    SDL_Surface *field2 = Load_img("../../../src/local-game/Textures/login/login_field2.png");
    //SDL_Surface *login_password = Load_img("../../../src/local-game/Textures/login&password.png");
    //SDL_Surface *scaled_login_password = ScaleSurface(login_password, SCREEN_WIDTH, SCREEN_HEIGHT);
    //Draw_image(screen, login_password, 0, 0);

    int done = 0;
    char temp[128];
    int temp_size = 0;
    int login_size = 0;
    int password_size = 0;



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
                        if((login_size != 0 && password_size != 0) || (enterCnt==1 && temp_size!=0)) {
                            strcpy(password, temp);
                            if (LOGIN(login, password) == 1) {
                                done = 1;
                                game_status = GAME_MENU;
                            } else {
                                enterCnt = 1;
                                temp_size=0;
                                memset(password, 0, 128);
                                memset(temp, 0, 128);
                            }
                        }
                        break;

                    case SDLK_DOWN:
                        if(enterCnt==0) {
                            login_size = temp_size;
                            strcpy(login, temp);
                            memset(temp, 0, 128);
                            strcpy(temp, password);
                            temp_size = password_size;
                        }
                        enterCnt=1;
                        break;
                    case SDLK_UP:
                        if(enterCnt==1) {
                            password_size = temp_size;
                            strcpy(password, temp);
                            memset(temp, 0, 128);
                            strcpy(temp, login);
                            temp_size = login_size;
                        }
                        enterCnt=0;
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
                WriteText(512, 221, password, 28, 0, 0, 0);
            } else {
                Draw_image(screen, field2, 496, 221);
                WriteText(512, 172, login, 28, 0, 0, 0);
                WriteText(512, 221, temp, 28, 0, 0, 0);
            }
            Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        }


        if (event.type == SDL_QUIT) {
            SDL_Quit();
            DISCONNECT();
            exit(0);
        }

    }

}

static void Process_leaderboard(/*PLAYERS_STRUCT *users*/) {
    SDL_Event event;
    TTF_Init();

    GET_LDB();

    SDL_Surface *menu_background = Load_img("../../../src/local-game/Textures/menu/menu_back.bmp");
    Draw_image(screen, menu_background, 0, 0);
    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Surface *pointer = Load_img("../../../src/local-game/Textures/menu/pointer.png");
    SDL_Surface *menu_pointer = ScaleSurface(pointer, 30, 30);

    int done = 0;
    int selectionPos = 0;
    int drawFlag = 1;

    int begin = 0, end = getScoreboardSize();
    int loginPos_Y = 365;
    char textMMR[5], textInGameStatus[1];

    WriteText(315, 310, "PLAYERS", 28, 255, 0, 0);
    WriteText(475, 310, "MMR", 28, 255, 0, 0);
    WriteText(555, 310, "Status", 28, 255, 0, 0);
    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Draw_image(screen, menu_pointer, 275, 365);
    for (int i = begin; i <= end; ++i) {
        WriteText(315, loginPos_Y, scoreboard[i].NAME, 24, 255, 255, 255);

        itoa(scoreboard[i].score, textMMR, 10);
        WriteText(475, loginPos_Y, textMMR, 24, 255, 0, 0);

        loginPos_Y += 30;
    }
    loginPos_Y = 365;
    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    while (!done) {

        GET_LDB();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        if (selectionPos != 0) {
                            selectionPos--;
                        } else {
                            drawFlag = 0;
                        }

                        if (drawFlag) {
                            Draw_image(screen, menu_background, 0, 0);
                            WriteText(315, 310, "PLAYERS", 28, 255, 0, 0);
                            WriteText(475, 310, "MMR", 28, 255, 0, 0);
                            WriteText(555, 310, "Status", 28, 255, 0, 0);
                            Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                            Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


//                            if (selectionPos <= 4) {
//                                begin = 0;
//                                end = 7;
//                                Draw_image(screen, menu_pointer, 275, 365 + 30 * (selectionPos));
//                            } else if (selectionPos >= 25) {
//                                begin = 21;
//                                end = 29;
//                                Draw_image(screen, menu_pointer, 275, 365 + 30 * (selectionPos - begin));
//                            } else {
//                                begin = selectionPos - 4;
//                                end = selectionPos + 4;
//                                Draw_image(screen, menu_pointer, 275, 365 + 30 * (end - selectionPos));
//                            }

                            for (int i = begin; i <= end; ++i) {
                                WriteText(315, loginPos_Y, scoreboard[i].NAME, 24, 255, 255, 255);

                                itoa(scoreboard[i].score, textMMR, 10);
                                WriteText(475, loginPos_Y, textMMR, 24, 255, 0, 0);

                                loginPos_Y += 30;
                            }

                            loginPos_Y = 365;
                            Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                        }
                        drawFlag = 1;
                        break;

                    case SDLK_s:
                        if (selectionPos != 29) {
                            selectionPos++;
                        } else {
                            drawFlag = 0;
                        }

                        if (drawFlag) {
                            Draw_image(screen, menu_background, 0, 0);
                            WriteText(315, 310, "PLAYERS", 28, 255, 0, 0);
                            WriteText(475, 310, "MMR", 28, 255, 0, 0);
                            WriteText(555, 310, "Status", 28, 255, 0, 0);
                            Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                            Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

//                            if (selectionPos <= 4) {
//                                begin = 0;
//                                end = 7;
//                                Draw_image(screen, menu_pointer, 275, 365 + 30 * (selectionPos));
//                            } else if (selectionPos >= 25) {
//                                begin = 21;
//                                end = 29;
//                                Draw_image(screen, menu_pointer, 275, 365 + 30 * (selectionPos - begin));
//                            } else {
//                                begin = selectionPos - 4;
//                                end = selectionPos + 4;
//                                Draw_image(screen, menu_pointer, 275, 365 + 30 * (end - selectionPos));
//                            }

                            for (int i = begin; i <= end; ++i) {
                                WriteText(315, loginPos_Y, scoreboard[i].NAME, 24, 255, 255, 255);

                                itoa(scoreboard[i].score, textMMR, 10);
                                WriteText(475, loginPos_Y, textMMR, 24, 255, 0, 0);

                                loginPos_Y += 30;
                            }

                            loginPos_Y = 365;
                            Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                        }
                        drawFlag = 1;
                        break;


                    case SDLK_ESCAPE:
                        done = 1;
                        break;

                }

                if (done) {
                    Draw_image(screen, menu_background, 0, 0);
                    WriteText(369, 323, "PLAYROOMS", 30, 255, 255, 255);
                    WriteText(359, 400, "LEADERBOARD", 30, 255, 255, 255);
                    WriteText(404, 594, "EXIT", 25, 255, 255, 255);
                    Draw_image(screen, menu_pointer, 320, 400);
                    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                    game_status = GAME_MENU;
                    return;
                }
            }

            if (event.type == SDL_QUIT) {
                SDL_Quit();
                DISCONNECT();
                exit(0);
            }
        }
    }
}

static void Process_waiting() {
    SDL_Event event;
    TTF_Init();

    SDL_Surface *menu_background = Load_img("../../../src/local-game/Textures/menu/menu_back.bmp");
    Draw_image(screen, menu_background, 0, 0);


    SDL_Surface *waiting = Load_img("../../../src/local-game/Textures/menu/room_waiting.bmp");
    Draw_image(screen, waiting, 248, 227);
    WriteText(326, 334, curPlayerRoom, 30, 255, 255, 255);

    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


    int done = 0;

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {

                    case SDLK_SPACE:
                        LEAVE();
                        game_status = GAME_ROOMS;
                        memset(curPlayerRoom, 0, 50);
                        return;
                        break;

                }
            }

            if (event.type == SDL_QUIT) {
                SDL_Quit();
                DISCONNECT();
                exit(0);
            }
        }
    }
}

static void Process_rooms(/*ROOMS_STRUCT *rooms, int roomsCnt*/) {
    SDL_Event event;
    TTF_Init();

    LOBBY();

    SDL_Surface *menu_background = Load_img("../../../src/local-game/Textures/menu/menu_back.bmp");

    SDL_Surface *room_join = Load_img("../../../src/local-game/Textures/menu/room_1.bmp");
    SDL_Surface *room_create = Load_img("../../../src/local-game/Textures/menu/room_2.bmp");
    SDL_Surface *room_create_button = Load_img("../../../src/local-game/Textures/menu/create_room.bmp");

    SDL_Surface *pointer = Load_img("../../../src/local-game/Textures/menu/pointer.png");
    SDL_Surface *menu_pointer = ScaleSurface(pointer, 30, 30);

    Draw_image(screen, menu_background, 0, 0);
    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    int done = 0;
    int currentRoom = 0;
    int currentPos = 0;
    char text[15];
    int roomCreatingFlag = -1;

    if (getLobbySize()) {

        Draw_image(screen, room_join, 285, 235);
        WriteText(342, 356, lobbies[currentRoom].NAME, 30, 255, 255, 255);

        itoa(lobbies[currentRoom].pcnt, text, 10);
        WriteText(527, 430, text, 30, 255, 255, 255);
        Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        roomCreatingFlag = 1;
    } else {

        Draw_image(screen, room_create_button, 293, 327);
        Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        roomCreatingFlag = 0;
    }

    while (!done) {

        LOBBY();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {

                    case SDLK_d:
                        if (roomCreatingFlag != 0) {
                            if (lobbies[currentRoom + 1].pcnt != 0) {
                                currentRoom++;
                                if (currentPos == 0) {
                                    Draw_image(screen, menu_background, 0, 0);
                                    Draw_image(screen, room_join, 285, 235);
                                    WriteText(342, 356, lobbies[currentRoom].NAME, 30, 255, 255, 255);

                                    char textRoom[15];
                                    itoa(lobbies[currentRoom].pcnt, text, 10);
                                    WriteText(527, 430, text, 30, 255, 255, 255);
                                    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                                } else if (currentPos == 1) {
                                    Draw_image(screen, menu_background, 0, 0);
                                    Draw_image(screen, room_create, 285, 235);
                                    WriteText(342, 356, lobbies[currentRoom].NAME, 30, 255, 255, 255);

                                    char textRoom[15];
                                    itoa(lobbies[currentRoom].pcnt, text, 10);
                                    WriteText(527, 430, text, 30, 255, 255, 255);
                                    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                                }
                            }
                        }
                        break;

                    case SDLK_a:
                        if (roomCreatingFlag != 0) {
                            if (currentRoom != 0) {
                                currentRoom--;
                                if (currentPos == 0) {
                                    Draw_image(screen, menu_background, 0, 0);
                                    Draw_image(screen, room_join, 285, 235);
                                    WriteText(342, 356, lobbies[currentRoom].NAME, 30, 255, 255, 255);

                                    char textRoom[15];
                                    itoa(lobbies[currentRoom].pcnt, text, 10);
                                    WriteText(527, 430, text, 30, 255, 255, 255);
                                    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                                } else if (currentPos == 1) {
                                    Draw_image(screen, menu_background, 0, 0);
                                    Draw_image(screen, room_create, 285, 235);
                                    WriteText(342, 356, lobbies[currentRoom].NAME, 30, 255, 255, 255);

                                    char textRoom[15];
                                    itoa(lobbies[currentRoom].pcnt, text, 10);
                                    WriteText(527, 430, text, 30, 255, 255, 255);
                                    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                                }
                            }
                        }
                        break;

                    case SDLK_s:
                        if (roomCreatingFlag != 0) {
                            if (currentPos != 1) {
                                currentPos++;
                                Draw_image(screen, menu_background, 0, 0);
                                Draw_image(screen, room_create, 285, 235);
                                WriteText(342, 356, lobbies[currentRoom].NAME, 30, 255, 255, 255);

                                char textRoom[15];
                                itoa(lobbies[currentRoom].pcnt, text, 10);
                                WriteText(527, 430, text, 30, 255, 255, 255);
                                Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                            }
                        }
                        break;

                    case SDLK_w:
                        if (roomCreatingFlag != 0) {
                            if (currentPos != 0) {
                                currentPos--;
                                Draw_image(screen, menu_background, 0, 0);
                                Draw_image(screen, room_join, 285, 235);
                                WriteText(342, 356, lobbies[currentRoom].NAME, 30, 255, 255, 255);

                                char textRoom[15];
                                itoa(lobbies[currentRoom].pcnt, text, 10);
                                WriteText(527, 430, text, 30, 255, 255, 255);
                                Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                            }
                        }
                        break;

                    case SDLK_SPACE:
                        if (roomCreatingFlag == 0) {
                            CREATE_ROOM();
                            strcpy(curPlayerRoom, login);
                            roomCreatingFlag = 1;
                            game_status = GAME_WAITING;
                            return;
                        }
                        if (roomCreatingFlag == 1 && currentPos == 0) {
                            ENTER(lobbies[currentPos].NAME);
                            strcpy(curPlayerRoom, lobbies[currentRoom].NAME);
                            game_status = GAME_WAITING;
                            return;
                        }
                        if (roomCreatingFlag == 1 && currentPos == 1) {
                            CREATE_ROOM();
                            strcpy(curPlayerRoom, login);
                            game_status = GAME_WAITING;
                            return;
                        }
                        break;

                    case SDLK_f:
                        done = 2;
                        break;

                    case SDLK_ESCAPE:
                        done = 1;
                        break;
                }

                if (done == 1) {
                    Draw_image(screen, menu_background, 0, 0);
                    WriteText(369, 323, "PLAYROOMS", 30, 255, 255, 255);
                    WriteText(359, 400, "LEADERBOARD", 30, 255, 255, 255);
                    WriteText(404, 594, "EXIT", 25, 255, 255, 255);
                    Draw_image(screen, menu_pointer, 320, 323);
                    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                    game_status = GAME_MENU;
                    return;
                }

                if (done == 2) {
                    game_status = GAME_RUNNING;
                    return;
                }

            }

            if (event.type == SDL_QUIT) {
                SDL_Quit();
                DISCONNECT();
                exit(0);
            }
        }
    }


}

static void Process_menu() {
    SDL_Event event;
    TTF_Init();

    game_status = GAME_MENU;
    SDL_Surface *menu_background = Load_img("../../../src/local-game/Textures/menu/menu_back.jpg");

    SDL_Surface *pointer = Load_img("../../../src/local-game/Textures/menu/pointer.png");
    SDL_Surface *menu_pointer = ScaleSurface(pointer, 30, 30);

    Draw_image(screen, menu_background, 0, 0);
    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

//menu1
    WriteText(369, 323, "PLAYROOMS", 30, 255, 255, 255);
    WriteText(359, 400, "LEADERBOARD", 30, 255, 255, 255);
    WriteText(404, 594, "EXIT", 25, 255, 255, 255);
    Draw_image(screen, menu_pointer, 329, 323);
    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    int done = 0;
    int selectionPos = 0;
    int pointerPos = 0;


    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {

                    case SDLK_w:
                        if (selectionPos != 0) {
                            selectionPos--;
                            Draw_image(screen, menu_background, 0, 0);
                            WriteText(369, 323, "PLAYROOMS", 30, 255, 255, 255);
                            WriteText(359, 400, "LEADERBOARD", 30, 255, 255, 255);
                            WriteText(404, 594, "EXIT", 25, 255, 255, 255);
                            if (selectionPos == 0) Draw_image(screen, menu_pointer, 329, 323);
                            if (selectionPos == 1) Draw_image(screen, menu_pointer, 320, 400);
                            Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                        }
                        break;

                    case SDLK_s:
                        if (selectionPos != 2) {
                            selectionPos++;
                            Draw_image(screen, menu_background, 0, 0);
                            WriteText(369, 323, "PLAYROOMS", 30, 255, 255, 255);
                            WriteText(359, 400, "LEADERBOARD", 30, 255, 255, 255);
                            WriteText(404, 594, "EXIT", 25, 255, 255, 255);
                            if (selectionPos == 1) Draw_image(screen, menu_pointer, 320, 400);
                            if (selectionPos == 2) Draw_image(screen, menu_pointer, 365, 594);
                            Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                        }
                        break;

                    case SDLK_SPACE:
                        if (selectionPos == 0) {
                            game_status = GAME_ROOMS;
                            return;
                        }
                        if (selectionPos == 1) {
                            game_status = GAME_LEADERBOARD;
                            return;
                        }
                        if (selectionPos == 2) {
                            SDL_Quit();
                            DISCONNECT();
                            exit(0);
                        }
                        break;
                }
            }

            if (event.type == SDL_QUIT) {
                SDL_Quit();
                DISCONNECT();
                exit(0);
            }

        }
    }
//menu1

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


int main(int argc, char *argv[]) {
    srand(time(NULL));
//init socket
    WSADATA wsd;
    if (WSAStartup(MAKEWORD(1, 1), &wsd) != 0) {
        printf("Can't connect to socket lib");
        return 1;
    }

    startSession();
//init socket

//init of rooms and players
    int playersCnt;
//    printf("Enter players count: ");
//    scanf("%d", &playersCnt);
//
//    int roomsCnt;
//    printf("Enter rooms count: ");
//    scanf("%d", &roomsCnt);
//
//
//    PLAYERS_STRUCT *users = (PLAYERS_STRUCT *) malloc(30 * sizeof(PLAYERS_STRUCT));
//    for (int i = 0; i < 30; ++i) {
//        users[i].login = (char *) malloc(16 * sizeof(char));
//        scanf("%s %d", users[i].login, &users[i].MMR);
//    }
//
//    ROOMS_STRUCT *rooms = (ROOMS_STRUCT *) malloc(5 * sizeof(ROOMS_STRUCT));
//    for (int i = 0; i < roomsCnt; ++i) {
//        rooms[i].roomName = (char *) malloc(30 * sizeof(char));
//        scanf("%s %d", rooms[i].roomName, &rooms[i].playersCnt);
//    }
//
//    for (int i = 0; i < 30; ++i){
//        printf("\n%s - %d - %d", users[i].login, users[i].MMR, users[i].inGame);
//    }
//init of rooms and players

//login
    Init_window("Maze", SCREEN_WIDTH, SCREEN_HEIGHT);
    Process_login();
//login

//menu
    while (game_status != GAME_RUNNING) {
        if (game_status == GAME_MENU) {
            Process_menu();
        }
        if (game_status == GAME_LEADERBOARD) {
            Process_leaderboard(/*users*/);
        }
        if (game_status == GAME_ROOMS) {
            Process_rooms(/*rooms, roomsCnt*/);
        }
        if (game_status == GAME_WAITING){
            Process_waiting();
        }
    }
//menu


//textures
    //SDL_Surface *background = Load_img("D:\\AllCodes\\TheMaze\\src\\local-game\\Textures\\wall2.jpg");
    //SDL_Surface *scaled_background = ScaleSurface(background,750,750);

    SDL_Surface *wall = Load_img("../../../src/local-game/Textures/maze/maze_wall3.bmp");
    SDL_Surface *scaled_wall = ScaleSurface(wall, TILE_SIZE, TILE_SIZE);

    SDL_Surface *floor = Load_img("../../../src/local-game/Textures/maze/maze_floor.bmp");
    SDL_Surface *scaled_floor = ScaleSurface(floor, TILE_SIZE + 6, TILE_SIZE + 6);

    SDL_Surface *exitPoint = Load_img(
            "../../../src/local-game/Textures/maze/maze_final-point.png");
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
    int x_finishPoint;
    int y_finishPoint;

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