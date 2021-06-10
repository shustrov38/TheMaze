//
// Created by Igor on 09.06.2021.
//

#ifndef THEMAZE_INTERFACE_H
#define THEMAZE_INTERFACE_H
#include <winsock.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#define TRANSACTION_CAPACITY    1024
#define PL_PARAM_SIZE           16
#define PL_CNT                  50


typedef enum {
    //TAG:          // WHAT IT DOES:                     ARGS "<> ...":            DIR:
    CONNECTION,     // connect this client to server    "<CONNECTION> NAME PASS"   TO
    SYS_MSG,        // message STR from/to server       "<SYS_MSG> STR"            TO/FROM
    MOV_RIVAL,      // move enemy NAME to X Y           "<MOV_RIVAL> NAME X Y"     FROM
    MOV_SELF,       // move this player to X Y          "<MOV_SELF> X Y"           TO
    EXIT,
    LEADERBOARD,
    ROOMS,
    CREATE_ROOM,
    ENTER_ROOM,
    LEAVE_ROOM,
    DESTROY_ROOM,
    ASK_STATE
} INNER_INTERFACE;

typedef enum{
    IN_MENU,
    IN_ROOM,
    IN_GAME,
    DEFAULT,
    LOSER,
    WINNER
} PL_STATE;

typedef struct {
    INNER_INTERFACE TAG;
    char    ARGS[16][16];
    int     VALID_ARG_CNT;
} COMMAND_PROTOTYPE;

typedef struct {
    char    NAME[PL_PARAM_SIZE];
    int     score;
    int     is_online;
} PLAYER;

typedef struct {
    char    NAME[PL_PARAM_SIZE];
    int     pcnt;
} ROOM;

typedef struct{
    char NAME[PL_PARAM_SIZE];
    int X;
    int Y;
} RECV_PL_INFO;

extern PLAYER scoreboard[PL_CNT];
extern ROOM lobbies[PL_CNT];
extern RECV_PL_INFO pl_render_info[4];
extern PL_STATE myState;
extern int curSeed;
extern COMMAND_PROTOTYPE C;
extern SOCKET client;

char *make_command(SOCKET client, COMMAND_PROTOTYPE proto);
int try_login(SOCKET client, COMMAND_PROTOTYPE C, char *name, char *password);
void cli_exit(SOCKET client, COMMAND_PROTOTYPE C);
void move_self(SOCKET client, COMMAND_PROTOTYPE C, int x, int y);
void cli_send(SOCKET client, COMMAND_PROTOTYPE C, char *str);
void upd_ld_board(SOCKET client, COMMAND_PROTOTYPE C);
void get_rooms(SOCKET client, COMMAND_PROTOTYPE C);
void create_room(SOCKET client, COMMAND_PROTOTYPE C);
void enter_room(SOCKET client, COMMAND_PROTOTYPE C, char *name);
void leave_room(SOCKET client, COMMAND_PROTOTYPE C);
void upd_st(SOCKET client, COMMAND_PROTOTYPE C);
int getLobbySize();
int getScoreboardSize();

#define LOGIN(NAME, PASS) try_login(client, C, NAME,PASS)
#define GET_LDB() upd_ld_board(client, C)
#define GO(X, Y) move_self(client, C, X, Y)
#define SAY(MSG) cli_send(client, C, MSG)
#define DISCONNECT() cli_exit(client, C)
#define LOBBY() get_rooms(client, C)
#define CREATE_ROOM() create_room(client, C)
#define ENTER(NAME) enter_room(client, C, NAME)
#define LEAVE() leave_room(client, C)

#endif //THEMAZE_INTERFACE_H