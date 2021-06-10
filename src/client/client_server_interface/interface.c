#include "interface.h"

PLAYER scoreboard[PL_CNT];
ROOM lobbies[PL_CNT];
RECV_PL_INFO pl_render_info[4];
PL_STATE myState = DEFAULT;
COMMAND_PROTOTYPE C;
SOCKET client;
int curSeed;
int pl_render_infoCnt = 0;

void wipe_lobbies(){
    for(int i = 0; i < PL_CNT; i++){
        lobbies[i].pcnt = 0;
        memset(lobbies[i].NAME,0,PL_PARAM_SIZE);
    }
}

void wipe_scores(){
    for(int i = 0; i < PL_CNT; i++){
        scoreboard[i].score = 0;
        scoreboard[i].is_online = 0;
        memset(scoreboard[i].NAME,0,PL_PARAM_SIZE);
    }
}

void wipe_room(){
    for(int i = 0; i < 4; i++){
        memset(pl_render_info[i].NAME,0,PL_PARAM_SIZE);
        pl_render_info[i].X = 0;
        pl_render_info[i].Y = 0;
    }
}

char *make_command(SOCKET client, COMMAND_PROTOTYPE proto) {
    int parsable_ret_ld = 0;
    int parsable_ret_menu = 0;
    int state_check = 0;
    int check_nei = 0;
    int check_cord = 0;
    int check_seed = 0;
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
            sprintf(command, "%s", "<CREATE_ROOM>");
            break;
        case ENTER_ROOM:
            sprintf(command, "%s %s", "<ENTER_ROOM>", proto.ARGS[0]);
            break;
        case LEAVE_ROOM:
            sprintf(command, "%s %s", "<LEAVE_ROOM>", proto.ARGS[0]);
            break;
        case DESTROY_ROOM:
            sprintf(command, "%s", "<DESTROY_ROOM>");
            break;
        case GET_STATE:
            sprintf(command, "%s", "<GET_STATE>");
            state_check = 1;
            break;
        case ROOM_NEIGHBOURS:
            sprintf(command, "%s", "<ROOM_NEIGHBOURS>");
            check_nei = 1;
            break;
        case GET_RENDER_CORD:
            sprintf(command, "%s", "<GET_RENDER_CORD>");
            check_cord = 1;
            break;
        case START_ROOM:
            sprintf(command, "%s", "<START_ROOM>");
            break;
        case GET_SEED:
            sprintf(command, "%s", "<GET_SEED>");
            check_seed = 1;
            break;
    }
    if (!parsable_ret_ld) printf(">>%s\n", command);
    send(client, command, TRANSACTION_CAPACITY, 0);
    recv(client, command, TRANSACTION_CAPACITY, 0);

    if (check_seed == 1) curSeed = atoi (command);

    printf("<<%s\n", command);

    if (state_check) {
        if (strcmp(command, "IN_MENU") == 0) {
            myState = IN_MENU;
        } else if (strcmp(command, "IN_ROOM") == 0) {
            myState = IN_ROOM;
        } else if (strcmp(command, "IN_GAME") == 0) {
            myState = IN_GAME;
        } else if (strcmp(command, "LOSER") == 0) {
            myState = LOSER;
        } else if (strcmp(command, "WINNER") == 0) {
            myState = WINNER;
        } else {
            myState = DEFAULT;
        }
    }

    if (parsable_ret_ld) {
        wipe_scores();
        int z = 0;
        int j = 0;
        char tmp[32];
        memset(tmp, 0, 32);
        for (int i = 0; i < strlen(command) && j < PL_CNT; i++) {
            if (command[i] == '#') {
//              printf("%s->", tmp);
                z = 0;
                sscanf(tmp, "%s %d %d", scoreboard[j].NAME, &scoreboard[j].score, &scoreboard[j].is_online);
                j++;
                memset(tmp, 0, 32);
                continue;
            }
            tmp[z++] = command[i];
        }
        for (int i = 0; i < PL_CNT; i++) {
            if (strlen(scoreboard[i].NAME) > 0)
                printf("%d %s %d %d\n", i + 1, scoreboard[i].NAME, scoreboard[i].score, scoreboard[i].is_online);
        }
    }
    if (parsable_ret_menu) {
        wipe_lobbies();
        int z = 0;
        int j = 0;
        char tmp[32];
        memset(tmp, 0, 32);
        for (int i = 0; i < strlen(command) && j < PL_CNT; i++) {
            if (command[i] == '#') {
//              printf("%s->", tmp);
                z = 0;
                sscanf(tmp, "%s %d", lobbies[j].NAME, &lobbies[j].pcnt);
                j++;
                memset(tmp, 0, 32);
                continue;
            }
            tmp[z++] = command[i];
        }
        for (int i = 0; i < PL_CNT; i++) {
            if (strlen(lobbies[i].NAME) > 0)printf("%s's room %d/4\n", lobbies[i].NAME, lobbies[i].pcnt);
        }
    }

    if (check_nei) {
        wipe_room();
        int z = 0;
        int j = 0;
        char tmp[32];
        memset(tmp, 0, 32);
        for (int i = 0; i < strlen(command) && j < 4; i++) {
            if (command[i] == '#') {
//              printf("%s->", tmp);
                z = 0;
                sscanf(tmp, "%s", pl_render_info[j].NAME);
                if (strlen(pl_render_info[j].NAME) > 0) j++;
                else {
                    memset(pl_render_info[j].NAME, 0, PL_PARAM_SIZE);
                }
                memset(tmp, 0, 32);
                continue;
            }
            tmp[z++] = command[i];
        }
        pl_render_infoCnt = j;
        for (int i = 0; i < 4; i++) {
            if (strlen(pl_render_info[i].NAME) > 0)printf("%s ", pl_render_info[i].NAME);
        }
    }

    if (check_cord) {
//        wipe_room();
        int z = 0;
        int j = 0;
        char tmp[32];
        memset(tmp, 0, 32);
        for (int i = 0; i < strlen(command) && j < 4; i++) {
            if (command[i] == '#') {
//              printf("%s->", tmp);
                z = 0;
                sscanf(tmp, "%s %d %d", pl_render_info[j].NAME, &pl_render_info[j].X, &pl_render_info[j].Y);
                if (strlen(pl_render_info[j].NAME) > 0) j++;
                else {
                    memset(pl_render_info[j].NAME, 0, PL_PARAM_SIZE);
                }
                memset(tmp, 0, 32);
                continue;
            }
            tmp[z++] = command[i];
        }
        for (int i = 0; i < 4; i++) {
            if (strlen(pl_render_info[i].NAME) > 0)
                printf(tmp, "%s %d %d\n", pl_render_info[j].NAME, pl_render_info[j].X, pl_render_info[j].Y);
        }
    }


    return command;
}

int getLobbySize() {
    int i = 0;
    while (i < PL_CNT && lobbies[i].pcnt != 0) {
        i++;
    }
    return i;
}

int getParticipantsSize() {
    int i = 0;
    while (i < 4 && strlen(pl_render_info[i].NAME) != 0) {
        i++;
    }
    return i;
}

int getScoreboardSize() {
    int i = 0;
    while (i < PL_CNT && strlen(scoreboard[i].NAME) != 0) {
        i++;
    }
    return i;
}

int try_login(SOCKET client, COMMAND_PROTOTYPE C, char *name, char *password) {
    C.TAG = CONNECTION;
    C.VALID_ARG_CNT = 2;
    strcpy(C.ARGS[0], name);
    strcpy(C.ARGS[1], password);
    return strcmp(make_command(client, C), "CONNECTION_SUCCESS") == 0;
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

void get_seed(SOCKET client, COMMAND_PROTOTYPE C) {
    C.TAG = GET_STATE;
    C.VALID_ARG_CNT = 0;
    make_command(client, C);
}

void start_room(SOCKET client, COMMAND_PROTOTYPE C) {
    C.TAG = START_ROOM;
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
    C.TAG = GET_STATE;
    C.VALID_ARG_CNT = 0;
    make_command(client, C);
}

void nei(SOCKET client, COMMAND_PROTOTYPE C) {
    C.TAG = ROOM_NEIGHBOURS;
    C.VALID_ARG_CNT = 0;
    make_command(client, C);
}

void cords(SOCKET client, COMMAND_PROTOTYPE C) {
    C.TAG = GET_RENDER_CORD;
    C.VALID_ARG_CNT = 0;
    make_command(client, C);
}