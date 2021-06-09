#ifndef THEMAZE_GAME_H
#define THEMAZE_GAME_H

static void Process_events();
static void Process_menu();

typedef struct{
    char *login;
    int MMR;
    int inGameStatus;
} PLAYERS;

typedef struct{
    char *roomName;
    int playersCnt;
} ROOMS;


enum {
    GAME_LOGIN,
    GAME_MENU,
    GAME_RUNNING,
    GAME_OVER
};


#endif //THEMAZE_GAME_H
