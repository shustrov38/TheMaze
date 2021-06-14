#ifndef THEMAZE_GAME_H
#define THEMAZE_GAME_H

static void Process_events();
static void Process_menu();


enum {
    GAME_LOGIN,
    GAME_MENU,
    GAME_ROOMS,
    GAME_WAITING,
    GAME_LEADERBOARD,
    GAME_RUNNING,
    GAME_RESULTS,
    GAME_OVER
};


#endif //THEMAZE_GAME_H
