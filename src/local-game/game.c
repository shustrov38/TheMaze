#include "game.h"
#include "utilities.h"
#include "maze_generator.h"
#include "playerInfo.h"

#define sq(X) (X)*(X)

const int TILE_SIZE = 15;
const int SCREEN_HEIGHT = 750;
const int SCREEN_WIDTH = 900;

static int client_game_status;

char login[128];
char password[128];
char curPlayerRoom[50];

void set_pixel(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_Rect rect;
    rect.x = x - 4;
    rect.y = y - 4;
    rect.w = 8;
    rect.h = 8;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, r, g, b));
}

void draw_circle(SDL_Surface *surface, int n_cx, int n_cy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    // if the first pixel in the screen is represented by (0,0) (which is in sdl)
    // remember that the beginning of the circle is not in the middle of the pixel
    // but to the left-top from it:

    double error = (double) -radius;
    double x = (double) radius - 0.5;
    double y = (double) 0.5;
    double cx = n_cx - 0.5;
    double cy = n_cy - 0.5;

    while (x >= y) {
        set_pixel(surface, (int) (cx + x), (int) (cy + y), r, g, b, a);
        set_pixel(surface, (int) (cx + y), (int) (cy + x), r, g, b, a);

        if (x != 0) {
            set_pixel(surface, (int) (cx - x), (int) (cy + y), r, g, b, a);
            set_pixel(surface, (int) (cx + y), (int) (cy - x), r, g, b, a);
        }

        if (y != 0) {
            set_pixel(surface, (int) (cx + x), (int) (cy - y), r, g, b, a);
            set_pixel(surface, (int) (cx - y), (int) (cy + x), r, g, b, a);
        }

        if (x != 0 && y != 0) {
            set_pixel(surface, (int) (cx - x), (int) (cy - y), r, g, b, a);
            set_pixel(surface, (int) (cx - y), (int) (cy - x), r, g, b, a);
        }

        error += y;
        ++y;
        error += y;

        if (error >= 0) {
            --x;
            error -= x;
            error -= x;
        }
    }
}

static void Process_exit_game() {
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {

        if (event.type == SDL_QUIT) {
            client_game_status = GAME_OVER;
            SDL_Quit();
            LEAVE();
            DISCONNECT();
            exit(0);
        }
    }
}

static void Process_login() {
    TTF_Init();
    SDL_Event event;
    client_game_status = GAME_LOGIN;

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
                        if ((login_size != 0 && password_size != 0) || (enterCnt == 1 && temp_size != 0)) {
                            strcpy(password, temp);
                            if (LOGIN(login, password) == 1) {
                                done = 1;
                                client_game_status = GAME_MENU;
                            } else {
                                enterCnt = 1;
                                temp_size = 0;
                                memset(password, 0, 128);
                                memset(temp, 0, 128);
                            }
                        }
                        break;

                    case SDLK_DOWN:
                        if (enterCnt == 0) {
                            login_size = temp_size;
                            strcpy(login, temp);
                            memset(temp, 0, 128);
                            strcpy(temp, password);
                            temp_size = password_size;
                        }
                        enterCnt = 1;
                        break;
                    case SDLK_UP:
                        if (enterCnt == 1) {
                            password_size = temp_size;
                            strcpy(password, temp);
                            memset(temp, 0, 128);
                            strcpy(temp, login);
                            temp_size = login_size;
                        }
                        enterCnt = 0;
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

static void Process_menu() {
    SDL_Event event;
    TTF_Init();


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
                            client_game_status = GAME_ROOMS;
                            return;
                        }
                        if (selectionPos == 1) {
                            client_game_status = GAME_LEADERBOARD;
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

static void Process_leaderboard(/*PLAYERS_STRUCT *users*/) {
    SDL_Event event;
    TTF_Init();

    GET_LDB();

    SDL_Surface *menu_background = Load_img("../../../src/local-game/Textures/menu/menu_back.jpg");
    Draw_image(screen, menu_background, 0, 0);
    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Surface *pointer = Load_img("../../../src/local-game/Textures/menu/pointer.png");
    SDL_Surface *menu_pointer = ScaleSurface(pointer, 30, 30);

    int done = 0;
    int selectionPos = 0;
    int drawFlag = 1;

    int count = getScoreboardSize();
    int begin = 0;
    int end = 7;
    int curPage = 1;
    int pageCnt = (getScoreboardSize() / 8) + 1;

    int loginPos_Y = 365;
    char textMMR[5], textInGameStatus[1];

    WriteText(315, 310, "PLAYERS", 28, 255, 0, 0);
    WriteText(475, 310, "MMR", 28, 255, 0, 0);
    WriteText(555, 310, "Status", 28, 255, 0, 0);
    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Draw_image(screen, menu_pointer, 275, 365);
    for (int i = begin; i <= end; ++i) {
        WriteText(315, loginPos_Y, scoreboard[i].NAME, 24, 255, 255, 255);

        if (strlen(scoreboard[i].NAME) != 0) {
            itoa(scoreboard[i].score, textMMR, 10);
            WriteText(475, loginPos_Y, textMMR, 24, 255, 0, 0);
        }

        if (strlen(scoreboard[i].NAME) != 0 && scoreboard[i].is_online == 1) {
            draw_circle(screen, 595, loginPos_Y + 17, 4, 0, 255, 0, 0);
        } else if (strlen(scoreboard[i].NAME) != 0 && scoreboard[i].is_online == 0){
            draw_circle(screen, 595, loginPos_Y + 17, 4, 255, 0, 0, 0);
        }

        loginPos_Y += 30;
    }
    loginPos_Y = 365;
    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    while (!done) {

        GET_LDB();
        Draw_image(screen, menu_background, 0, 0);
        WriteText(315, 310, "PLAYERS", 28, 255, 0, 0);
        WriteText(475, 310, "MMR", 28, 255, 0, 0);
        WriteText(555, 310, "Status", 28, 255, 0, 0);
        Draw_image(screen, menu_pointer, 275, 365 + (30 * (selectionPos % 8)));
        Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        for (int i = begin; i <= end; ++i) {
            WriteText(315, loginPos_Y, scoreboard[i].NAME, 24, 255, 255, 255);

            if (strlen(scoreboard[i].NAME) != 0) {
                itoa(scoreboard[i].score, textMMR, 10);
                WriteText(475, loginPos_Y, textMMR, 24, 255, 0, 0);
            }

            if (strlen(scoreboard[i].NAME) != 0 && scoreboard[i].is_online == 1) {
                draw_circle(screen, 595, loginPos_Y + 17, 4, 0, 255, 0, 0);
            } else if (strlen(scoreboard[i].NAME) != 0 && scoreboard[i].is_online == 0){
                draw_circle(screen, 595, loginPos_Y + 17, 4, 255, 0, 0, 0);
            }


            loginPos_Y += 30;
        }
        loginPos_Y = 365;

        Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        pageCnt = (getScoreboardSize() / 8) + 1;

        while (SDL_PollEvent(&event)) {

            GET_LDB();
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        if (selectionPos != 0) {
                            selectionPos--;
                            if (selectionPos == (end - 8) && (curPage - 1) != 0) {
                                curPage--;
                                begin -= 8;
                                end -= 8;
                            }
                        } else {
                            drawFlag = 0;
                        }

                        if (drawFlag) {
                            Draw_image(screen, menu_background, 0, 0);
                            WriteText(315, 310, "PLAYERS", 28, 255, 0, 0);
                            WriteText(475, 310, "MMR", 28, 255, 0, 0);
                            WriteText(555, 310, "Status", 28, 255, 0, 0);
                            Draw_image(screen, menu_pointer, 275, 365 + (30 * (selectionPos % 8)));
                            Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

                            for (int i = begin; i <= end; ++i) {
                                WriteText(315, loginPos_Y, scoreboard[i].NAME, 24, 255, 255, 255);

                                if (strlen(scoreboard[i].NAME) != 0) {
                                    itoa(scoreboard[i].score, textMMR, 10);
                                    WriteText(475, loginPos_Y, textMMR, 24, 255, 0, 0);
                                }

                                if (strlen(scoreboard[i].NAME) != 0 && scoreboard[i].is_online == 1) {
                                    draw_circle(screen, 595, loginPos_Y + 17, 4, 0, 255, 0, 0);
                                } else if (strlen(scoreboard[i].NAME) != 0 && scoreboard[i].is_online == 0){
                                    draw_circle(screen, 595, loginPos_Y + 17, 4, 255, 0, 0, 0);
                                }


                                loginPos_Y += 30;
                            }
                            loginPos_Y = 365;

                            Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                        }
                        drawFlag = 1;
                        break;

                    case SDLK_s:
                        if (selectionPos != count - 1) {
                            selectionPos++;
                            if (selectionPos == begin + 8 && curPage != pageCnt) {
                                curPage++;
                                begin += 8;
                                end += 8;
                            }
                        } else {
                            drawFlag = 0;
                        }

                        if (drawFlag) {
                            Draw_image(screen, menu_background, 0, 0);
                            WriteText(315, 310, "PLAYERS", 28, 255, 0, 0);
                            WriteText(475, 310, "MMR", 28, 255, 0, 0);
                            WriteText(555, 310, "Status", 28, 255, 0, 0);
                            Draw_image(screen, menu_pointer, 275, 365 + (30 * (selectionPos % 8)));
                            Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

                            for (int i = begin; i <= end; ++i) {
                                WriteText(315, loginPos_Y, scoreboard[i].NAME, 24, 255, 255, 255);

                                if (strlen(scoreboard[i].NAME) != 0) {
                                    itoa(scoreboard[i].score, textMMR, 10);
                                    WriteText(475, loginPos_Y, textMMR, 24, 255, 0, 0);
                                }

                                if (strlen(scoreboard[i].NAME) != 0 && scoreboard[i].is_online == 1) {
                                    draw_circle(screen, 595, loginPos_Y + 17, 4, 0, 255, 0, 0);
                                } else if (strlen(scoreboard[i].NAME) != 0 && scoreboard[i].is_online == 0){
                                    draw_circle(screen, 595, loginPos_Y + 17, 4, 255, 0, 0, 0);
                                }

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
                    client_game_status = GAME_MENU;
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

static void Process_rooms(/*ROOMS_STRUCT *rooms, int roomsCnt*/) {
    SDL_Event event;
    TTF_Init();

    LOBBY();

    SDL_Surface *menu_background = Load_img("../../../src/local-game/Textures/menu/menu_back.jpg");

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
    memset(text, 0, 15);
    int roomCreatingFlag = -1;

    if (getLobbySize()) {
        Draw_image(screen, room_join, 285, 235);
        WriteText(342, 356, strcat(lobbies[currentRoom].NAME, "'s Room"), 30, 255, 255, 255);

        itoa(lobbies[currentRoom].pcnt, text, 10);
        WriteText(527, 430, text, 30, 255, 255, 255);
        memset(text, 0, 15);
        Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        roomCreatingFlag = 1;
    } else {

        Draw_image(screen, room_create_button, 293, 327);
        Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        roomCreatingFlag = 0;
    }

    while (!done) {

        LOBBY();
        if (roomCreatingFlag == 0 && getLobbySize()) {
            Draw_image(screen, menu_background, 0, 0);
            Draw_image(screen, room_join, 285, 235);
            WriteText(342, 356, strcat(lobbies[currentRoom].NAME, "'s Room"), 30, 255, 255, 255);

            itoa(lobbies[currentRoom].pcnt, text, 10);
            WriteText(527, 430, text, 30, 255, 255, 255);
            memset(text, 0, 15);
            Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            roomCreatingFlag = 1;
        } else if (getLobbySize() == 0) {
            Draw_image(screen, menu_background, 0, 0);
            Draw_image(screen, room_create_button, 293, 327);
            Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            roomCreatingFlag = 0;
        }

        while (SDL_PollEvent(&event)) {
            LOBBY();

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {

                    case SDLK_d:
                        if (roomCreatingFlag != 0) {
                            if (lobbies[currentRoom + 1].pcnt != 0) {
                                currentRoom++;
                                if (currentPos == 0) {
                                    Draw_image(screen, menu_background, 0, 0);
                                    Draw_image(screen, room_join, 285, 235);
                                    WriteText(342, 356, strcat(lobbies[currentRoom].NAME, "'s Room"), 30, 255, 255,
                                              255);

                                    itoa(lobbies[currentRoom].pcnt, text, 10);
                                    WriteText(527, 430, text, 30, 255, 255, 255);
                                    memset(text, 0, 15);
                                    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                                } else if (currentPos == 1) {
                                    Draw_image(screen, menu_background, 0, 0);
                                    Draw_image(screen, room_create, 285, 235);
                                    WriteText(342, 356, strcat(lobbies[currentRoom].NAME, "'s Room"), 30, 255, 255,
                                              255);

                                    itoa(lobbies[currentRoom].pcnt, text, 10);
                                    WriteText(527, 430, text, 30, 255, 255, 255);
                                    memset(text, 0, 15);
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
                                    WriteText(342, 356, strcat(lobbies[currentRoom].NAME, "'s Room"), 30, 255, 255,
                                              255);

                                    itoa(lobbies[currentRoom].pcnt, text, 10);
                                    WriteText(527, 430, text, 30, 255, 255, 255);
                                    memset(text, 0, 15);
                                    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                                } else if (currentPos == 1) {
                                    Draw_image(screen, menu_background, 0, 0);
                                    Draw_image(screen, room_create, 285, 235);
                                    WriteText(342, 356, strcat(lobbies[currentRoom].NAME, "'s Room"), 30, 255, 255,
                                              255);

                                    itoa(lobbies[currentRoom].pcnt, text, 10);
                                    WriteText(527, 430, text, 30, 255, 255, 255);
                                    memset(text, 0, 15);
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
                                WriteText(342, 356, strcat(lobbies[currentRoom].NAME, "'s Room"), 30, 255, 255,
                                          255);

                                itoa(lobbies[currentRoom].pcnt, text, 10);
                                WriteText(527, 430, text, 30, 255, 255, 255);
                                memset(text, 0, 15);
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
                                WriteText(342, 356, strcat(lobbies[currentRoom].NAME, "'s Room"), 30, 255, 255,
                                          255);

                                itoa(lobbies[currentRoom].pcnt, text, 10);
                                WriteText(527, 430, text, 30, 255, 255, 255);
                                memset(text, 0, 15);
                                Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                            }
                        }
                        break;

                    case SDLK_SPACE:
                        if (roomCreatingFlag == 0) {
                            CREATE_ROOM();
                            memset(curPlayerRoom, 0, 50);
                            strcpy(curPlayerRoom, login);
                            roomCreatingFlag = 1;
                            client_game_status = GAME_WAITING;
                            return;
                        }
                        if (roomCreatingFlag == 1 && currentPos == 0) {
                            ENTER(lobbies[currentRoom].NAME);
                            memset(curPlayerRoom, 0, 50);
                            strcpy(curPlayerRoom, lobbies[currentRoom].NAME);
                            client_game_status = GAME_WAITING;
                            printf("Enter %s\n", strcat(lobbies[currentRoom].NAME, "'s Room"));
                            return;
                        }
                        if (roomCreatingFlag == 1 && currentPos == 1) {
                            CREATE_ROOM();
                            memset(curPlayerRoom, 0, 50);
                            strcpy(curPlayerRoom, login);
                            client_game_status = GAME_WAITING;
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
                    client_game_status = GAME_MENU;
                    return;
                }

                if (done == 2) {
                    client_game_status = GAME_RUNNING;
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

    SDL_Surface *menu_background = Load_img("../../../src/local-game/Textures/menu/menu_back.jpg");
    Draw_image(screen, menu_background, 0, 0);

    SDL_Surface *user_waiting = Load_img("../../../src/local-game/Textures/menu/room_waiting.bmp");
    SDL_Surface *admin_waiting_leave = Load_img("../../../src/local-game/Textures/menu/room_waiting_leave.bmp");
    SDL_Surface *admin_waiting_start = Load_img("../../../src/local-game/Textures/menu/room_waiting_start.bmp");

    if (strcmp(curPlayerRoom, login) == 0) {
        Draw_image(screen, admin_waiting_leave, 248, 227);
    } else if (strcmp(curPlayerRoom, login) != 0) {
        Draw_image(screen, user_waiting, 248, 227);
    }

    char roomName[50];
    strcpy(roomName, curPlayerRoom);
    strcat(roomName, "'s Room");
    WriteText(326, 334, roomName, 30, 255, 255, 255);
    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


    int done = 0;
    int currentChoose = 1;
    int pos_y = 402;
    while (!done) {

        GET_STATUS();
        if (myState == IN_GAME) {
            client_game_status = GAME_RUNNING;
            return;
        } else if (myState != IN_ROOM) {
            LEAVE();
            client_game_status = GAME_ROOMS;
            memset(roomName, 0, 50);
            return;
            break;
        }

        GET_NEIGHBOURS();
        if (strcmp(curPlayerRoom, login) != 0) {
            Draw_image(screen, menu_background, 0, 0);
            Draw_image(screen, user_waiting, 248, 227);
            WriteText(326, 334, roomName, 30, 255, 255, 255);
            for (int i = 0; i < pl_render_infoCnt; ++i) {
                WriteText(345, pos_y, pl_render_info[i].NAME, 30, 255, 255, 255);

                pos_y += 40;
            }
            pos_y = 402;
        }
        if (strcmp(curPlayerRoom, login) == 0) {
            if (currentChoose == 1) {
                Draw_image(screen, menu_background, 0, 0);
                Draw_image(screen, admin_waiting_leave, 248, 227);
                WriteText(326, 334, roomName, 30, 255, 255, 255);
                for (int i = 0; i < pl_render_infoCnt; ++i) {
                    WriteText(345, pos_y, pl_render_info[i].NAME, 30, 255, 255, 255);

                    pos_y += 40;
                }
                pos_y = 402;
            } else if (currentChoose == 2) {
                Draw_image(screen, menu_background, 0, 0);
                Draw_image(screen, admin_waiting_start, 248, 227);
                WriteText(326, 334, roomName, 30, 255, 255, 255);
                for (int i = 0; i < pl_render_infoCnt; ++i) {
                    WriteText(345, pos_y, pl_render_info[i].NAME, 30, 255, 255, 255);

                    pos_y += 40;
                }
                pos_y = 402;
            }
        }
        Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_a:
                        if (strcmp(curPlayerRoom, login) == 0) {
                            if (currentChoose != 1) {
                                currentChoose = 1;
                                Draw_image(screen, menu_background, 0, 0);
                                Draw_image(screen, admin_waiting_leave, 248, 227);
                                WriteText(326, 334, roomName, 30, 255, 255, 255);

                                for (int i = 0; i < pl_render_infoCnt; ++i) {
                                    WriteText(345, pos_y, pl_render_info[i].NAME, 30, 255, 255, 255);
                                    pos_y += 40;
                                }
                                pos_y = 402;
                                Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                            }
                        }

                        break;

                    case SDLK_d:
                        if (strcmp(curPlayerRoom, login) == 0) {
                            if (currentChoose != 2) {
                                currentChoose = 2;
                                Draw_image(screen, menu_background, 0, 0);
                                Draw_image(screen, admin_waiting_start, 248, 227);
                                WriteText(326, 334, roomName, 30, 255, 255, 255);

                                for (int i = 0; i < pl_render_infoCnt; ++i) {
                                    WriteText(345, pos_y, pl_render_info[i].NAME, 30, 255, 255, 255);
                                    pos_y += 40;
                                }
                                pos_y = 402;
                                Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                            }
                        }
                        break;

                    case SDLK_SPACE:
                        if (strcmp(curPlayerRoom, login) == 0) {
                            if (currentChoose == 2) {
                                if (strcmp(START(), "START_ROOM_SUCCESS") == 0) {
                                    client_game_status = GAME_RUNNING;
                                    return;
                                }
                            } else if (currentChoose == 1) {
                                LEAVE();
                                client_game_status = GAME_ROOMS;
                                memset(roomName, 0, 50);
                                return;
                            }
                        } else if (strcmp(curPlayerRoom, login) != 0) {
                            LEAVE();
                            client_game_status = GAME_ROOMS;
                            memset(roomName, 0, 50);
                            return;

                        }
                        break;
                }
            }

            if (event.type == SDL_QUIT) {
                SDL_Quit();
                LEAVE();
                DISCONNECT();
                exit(0);
            }
        }
    }

}

static void Process_game() {
    //textures
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

//    playerPos *players = initAllPlayers(playersCnt, ICONS, iconsCnt);

    GET_SEED();
    srand(curSeed);

    int height = MAZE_SIZE;
    int width = MAZE_SIZE;
    int **maze;
    int x_finishPoint;
    int y_finishPoint;

//maze generation
    maze = initMaze(maze, width, height);
    maze = generator(maze, 0, width - 1, 0, height - 1);
    maze = createFinishPoint(maze, width, height);


    if (GAME_RUNNING == client_game_status) {

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
        UPD_RENDER_INFO();

        for (int i = 0; i < pl_render_infoCnt; ++i) {
            int iconNum;
            int iconUsing[iconsCnt];
            for (int i = 0; i < iconsCnt; ++i) iconUsing[i] = 0;

            for (int i = 0; i < pl_render_infoCnt; ++i) {
                iconNum = rand() % iconsCnt;
                if (iconUsing[iconNum] == 0) {
                    pl_render_info[i].icon = ICONS[iconNum];
                    iconUsing[iconNum]++;
                } else {
                    i--;
                }
            }
        }

        for (int i = 0; i < pl_render_infoCnt; ++i) {
            Draw_image(screen, pl_render_info[i].icon, (pl_render_info[i].X * TILE_SIZE) - 3,
                       (pl_render_info[i].Y * TILE_SIZE) - 3);
        }
        Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
//maze generation


//game (movements of players)
    Uint32 startTime = SDL_GetTicks();
    int fps = 7;

    while (client_game_status != GAME_OVER) {
        Process_exit_game();

        int myCurPosition = 0;
        for (int i = 0; i < pl_render_infoCnt; ++i) {
            if (strcmp(login, pl_render_info[i].NAME) == 0) {
                myCurPosition = i;
            }
        }

        if (SDL_GetTicks() - startTime >= 1000 / fps) {
            startTime = SDL_GetTicks();
            UPD_RENDER_INFO();

            for (int i = 0; i < pl_render_infoCnt; ++i) {
                if (strcmp(login, pl_render_info[i].NAME) == 0) {
                    myCurPosition = i;
                }
            }

            pl_render_info[myCurPosition] = playerMoves(maze, pl_render_info[myCurPosition], myCurPosition);

            GET_STATUS();
            if (myState == WINNER || myState == LOSER || myState == IN_MENU) {
                client_game_status = GAME_RESULTS;
                return;
            }
        }

        // redraw maze
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

        // draw players
        for (int i = 0; i < pl_render_infoCnt; ++i) {
            Draw_image(screen, scaled_floor, (TILE_SIZE * pl_render_info[i].X_prev) - 3,
                       (TILE_SIZE * pl_render_info[i].Y_prev) - 3);
            Draw_image(screen, pl_render_info[i].icon, (TILE_SIZE * pl_render_info[i].X) - 3,
                       (TILE_SIZE * pl_render_info[i].Y) - 3);
        }

        // rune
        Draw_image(screen, scaled_exitPoint, TILE_SIZE * x_finishPoint, TILE_SIZE * y_finishPoint);

        // fog
        myCurPosition = 0;
        for (int i = 0; i < pl_render_infoCnt; ++i) {
            if (strcmp(login, pl_render_info[i].NAME) == 0) {
                myCurPosition = i;
            }
        }
        int x = pl_render_info[myCurPosition].X, y = pl_render_info[myCurPosition].Y;
        int maze_width = width * TILE_SIZE, maze_height = height * TILE_SIZE;
        int dist = 0;
        dist = max(dist, sq(0 - x) + sq(0 - y));
        dist = max(dist, sq(0 - x) + sq(maze_height - y));
        dist = max(dist, sq(maze_width - x) + sq(0 - y));
        dist = max(dist, sq(maze_width - x) + sq(maze_height - y));
        dist = (int) (sqrt(dist) + 1);
        for (int r = 100; r <= dist + 1; r += 8) {
            draw_circle(
                    screen,
                    x * TILE_SIZE,
                    y * TILE_SIZE,
                    r, 0, 0, 0, 0
            );
        }

        showPlayersInfo(pl_render_info, pl_render_infoCnt);

        Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
}

static void Process_results() {
    SDL_Event event;
    TTF_Init();

    SDL_Surface *win = Load_img("../../../src/local-game/Textures/results/win.bmp");
    SDL_Surface *lose = Load_img("../../../src/local-game/Textures/results/lose.png");

    if (myState == WINNER) {
        Draw_image(screen, win, 0, 0);
    } else if (myState == LOSER) {
        Draw_image(screen, lose, 0, 0);
    } else if (myState == IN_MENU) {
        client_game_status = GAME_MENU;
        return;
    }
    Update_window_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    int done = 0;

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        client_game_status = GAME_MENU;
                        LEAVE();
                        return;
                }
            }

            if (event.type == SDL_QUIT) {
                SDL_Quit();
                LEAVE();
                DISCONNECT();
                exit(0);
            }
        }
    }

}

int main(int argc, char *argv[]) {


//init socket
    WSADATA wsd;
    if (WSAStartup(MAKEWORD(1, 1), &wsd) != 0) {
        printf("Can't connect to socket lib");
        return 1;
    }

    startSession();
//init socket


//login
    Init_window("Maze", SCREEN_WIDTH, SCREEN_HEIGHT);
    Process_login();
//login
    SDL_SetWindowTitle(window, login);
//menu
    while (client_game_status != GAME_OVER) {
        if (client_game_status == GAME_MENU) {
            Process_menu();
        }
        if (client_game_status == GAME_LEADERBOARD) {
            Process_leaderboard(/*users*/);
        }
        if (client_game_status == GAME_ROOMS) {
            Process_rooms(/*rooms, roomsCnt*/);
        }
        if (client_game_status == GAME_WAITING) {
            Process_waiting();
        }
        if (client_game_status == GAME_RUNNING) {
            Process_game();
        }
        if (client_game_status == GAME_RESULTS) {
            Process_results();
        }
    }
//menu

}