//
// Created by IGOR on 09.06.2021.
//

#include "interface.h"
PLAYER scoreboard[PL_CNT];
ROOM lobbies[PL_CNT];
RECV_PL_INFO pl_render_info[4];
PL_STATE myState = DEFAULT;
COMMAND_PROTOTYPE C;
SOCKET client;
int curSeed;

char *make_command(SOCKET client, COMMAND_PROTOTYPE proto) {
    int parsable_ret_ld = 0;
    int parsable_ret_menu = 0;
    char *command = malloc(sizeof(char) * TRANSACTION_CAPACITY);
    memset(command, 0, TRANSACTION_CAPACITY);
    switch (proto.TAG) {
        case CONNECTION:
            sprintf(command, "%s %s %s", "<CONNECTION>", proto.ARGS[0], proto.ARGS[1]);
            break;
        case SYS_MSG:
            sprintf(command, "%s", proto.ARGS[0]);
            break;
        case MOV_SELF:
            sprintf(command, "%s %s %s", "<MOV_SELF>", proto.ARGS[0], proto.ARGS[1]);
            break;
        case EXIT:
            sprintf(command, "%s", "<EXIT>");
            break;
        case LEADERBOARD:
            parsable_ret_ld = 1;
            sprintf(command, "%s", "<LEADERBOARD>");
            break;
        case ROOMS:
            parsable_ret_menu = 1;
            sprintf(command, "%s", "<ROOMS>");
            break;
        case CREATE_ROOM:
            sprintf(command,"%s", "<CREATE_ROOM>");
            break;
        case ENTER_ROOM:
            sprintf(command, "%s %s", "<ENTER_ROOM>", proto.ARGS[0]);
            break;
        case LEAVE_ROOM:
            sprintf(command, "%s %s", "<LEAVE_ROOM>", proto.ARGS[0]);
            break;
        case DESTROY_ROOM:
            sprintf(command,"%s", "<DESTROY_ROOM>");
            break;
        case ASK_STATE:
            sprintf(command,"%s", "<ASK_STATE>");
    }
    if (!parsable_ret_ld) printf(">>%s\n", command);
    send(client, command, TRANSACTION_CAPACITY, 0);
    recv(client, command, TRANSACTION_CAPACITY, 0);

    if(strcmp(command,"IN_MENU")==0){
        myState = IN_MENU;
    } else if(strcmp(command,"IN_ROOM")==0){
        myState = IN_ROOM;
    } else if(strcmp(command,"IN_GAME")==0){
        myState = IN_GAME;
    } else if(strcmp(command,"LOSER")==0){
        myState = LOSER;
    }else if(strcmp(command,"WINNER")==0){
        myState = WINNER;
    } else {
        myState = DEFAULT;
    }

    if (parsable_ret_ld) {
        int z = 0;
        int j = 0;
        char tmp[32];
        memset(tmp, 0, 32);
        for (int i = 0; i < strlen(command) && j < PL_CNT; i++) {
            if (command[i] == '#') {
//              printf("%s->", tmp);
                z = 0;
                sscanf(tmp, "%s %d %d", scoreboard[j].NAME, &scoreboard[j].score, &scoreboard[j].is_online);
                if(strlen(lobbies[i].NAME)>0) j++;
                else {
                    memset(scoreboard[j].NAME,0,PL_PARAM_SIZE);
                    scoreboard[j].score = 0;
                    scoreboard[j].is_online = 0;
                }
                memset(tmp, 0, 32);
                continue;
            }
            tmp[z++] = command[i];
        }
        for (int i = 0; i < PL_CNT; i++) {
            if(strlen(scoreboard[i].NAME)>0)printf("%d %s %d %d\n", i + 1, scoreboard[i].NAME, scoreboard[i].score, scoreboard[i].is_online);
        }
    }
    if (parsable_ret_menu) {
        int z = 0;
        int j = 0;
        char tmp[32];
        memset(tmp, 0, 32);
        for (int i = 0; i < strlen(command) && j < PL_CNT; i++) {
            if (command[i] == '#') {
//              printf("%s->", tmp);
                z = 0;
                sscanf(tmp, "%s %d", lobbies[j].NAME, &lobbies[j].pcnt);
                if(strlen(lobbies[i].NAME)>0) j++;
                else {
                    memset(lobbies[j].NAME,0,PL_PARAM_SIZE);
                    lobbies[j].pcnt = 0;
                }
                memset(tmp, 0, 32);
                continue;
            }
            tmp[z++] = command[i];
        }
        for (int i = 0; i < PL_CNT; i++) {
            if(strlen(lobbies[i].NAME)>0)printf("%s's room %d/4\n", lobbies[i].NAME, lobbies[i].pcnt);
        }
    }
    printf("<<%s\n", command);

    return command;
}


void try_login(SOCKET client, COMMAND_PROTOTYPE C, char *name, char *password) {
    C.TAG = CONNECTION;
    C.VALID_ARG_CNT = 2;
    strcpy(C.ARGS[0], name);
    strcpy(C.ARGS[1], password);
    while (strcmp(make_command(client, C), "CONNECTION_SUCCESS") != 0) {
        printf("Wrong password! Try again:\n");
        printf("Nickname:");
        scanf("%s", name);
        printf("Password:");
        scanf("%s", password);
        strcpy(C.ARGS[0], name);
        strcpy(C.ARGS[1], password);
    }
}

void cli_exit(SOCKET client, COMMAND_PROTOTYPE C) {
    C.TAG = EXIT;
    C.VALID_ARG_CNT = 0;
    make_command(client, C);
    closesocket(client);
}

void move_self(SOCKET client, COMMAND_PROTOTYPE C, int x, int y) {
    C.TAG = MOV_SELF;
    C.VALID_ARG_CNT = 2;
    strcpy(C.ARGS[0], itoa(x, C.ARGS[0], 10));
    strcpy(C.ARGS[1], itoa(y, C.ARGS[1], 10));
    make_command(client, C);
}

void cli_send(SOCKET client, COMMAND_PROTOTYPE C, char *str) {
    C.TAG = SYS_MSG;
    C.VALID_ARG_CNT = 1;
    strcpy(C.ARGS[0], str);
    make_command(client, C);
}

void upd_ld_board(SOCKET client, COMMAND_PROTOTYPE C) {
    C.TAG = LEADERBOARD;
    C.VALID_ARG_CNT = 0;
    make_command(client, C);
}

void get_rooms(SOCKET client, COMMAND_PROTOTYPE C) {
    C.TAG = ROOMS;
    C.VALID_ARG_CNT = 0;
    make_command(client, C);
}

void create_room(SOCKET client, COMMAND_PROTOTYPE C) {
    C.TAG = CREATE_ROOM;
    C.VALID_ARG_CNT = 0;
    make_command(client, C);
}

void enter_room(SOCKET client, COMMAND_PROTOTYPE C, char *name) {
    C.TAG = ENTER_ROOM;
    C.VALID_ARG_CNT = 1;
    strcpy(C.ARGS[0], name);
    make_command(client, C);
    //WARN
}

void leave_room(SOCKET client, COMMAND_PROTOTYPE C) {
    C.TAG = LEAVE_ROOM;
    C.VALID_ARG_CNT = 0;
    make_command(client, C);
}

void upd_st(SOCKET client, COMMAND_PROTOTYPE C) {
    C.TAG = ASK_STATE;
    C.VALID_ARG_CNT = 0;
    make_command(client, C);
}