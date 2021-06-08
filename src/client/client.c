#include <winsock.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define TRANSACTION_CAPACITY 1024
#define PL_PARAM_SIZE   16
#define PL_CNT 50

typedef enum {
    //TAG:          // WHAT IT DOES:                     ARGS "<> ...":            DIR:
    CONNECTION,     // connect this client to server    "<CONNECTION> NAME PASS"   TO
    SYS_MSG,        // message STR from/to server       "<SYS_MSG> STR"            TO/FROM
    ENTER,         //
    MOV_RIVAL,      // move enemy NAME to X Y           "<MOV_RIVAL> NAME X Y"     FROM
    MOV_SELF,       // move this player to X Y          "<MOV_SELF> X Y"           TO
    EXIT,
    LD_BOARD,
    ROOMS,
    CREATE_ROOM,

} INNER_INTERFACE;

typedef struct {
    INNER_INTERFACE TAG;
    char ARGS[16][16];
    int VALID_ARG_CNT;
} COMMAND_PROTOTYPE;

typedef struct {
    char NAME[PL_PARAM_SIZE];
    int score;
    int is_online;
} PLAYER;

typedef struct {
    int id;
    int pcnt;
} ROOM;

PLAYER scoreboard[PL_CNT];
ROOM LOBBIES[PL_CNT];

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
            sprintf(command, "%s %s", "<SYS_MSG>", proto.ARGS[0]);
            break;
        case MOV_SELF:
            sprintf(command, "%s %s %s", "<MOV_SELF>", proto.ARGS[0], proto.ARGS[1]);
            break;
        case ENTER:
            sprintf(command, "%s %s", "<ENTER>", proto.ARGS[0]);
            break;
        case EXIT:
            sprintf(command, "%s", "<EXIT>");
            break;
        case LD_BOARD:
            parsable_ret_ld = 1;
            sprintf(command, "%s", "<LD_BOARD>");
            break;
        case ROOMS:
            parsable_ret_menu = 1;
            sprintf(command, "%s", "<ROOMS>");
            break;
    }
    if (!parsable_ret_ld) printf(">>%s", command);
    send(client, command, TRANSACTION_CAPACITY, 0);
    recv(client, command, TRANSACTION_CAPACITY, 0);

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
                j++;
                memset(tmp, 0, 32);
                continue;
            }
            tmp[z++] = command[i];
        }
        for (int i = 0; i < PL_CNT; i++) {
            printf("%d %s %d %d\n", i + 1, scoreboard[i].NAME, scoreboard[i].score, scoreboard[i].is_online);
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
                sscanf(tmp, "%s %d %d", scoreboard[j].NAME, &scoreboard[j].score, &scoreboard[j].is_online);
                j++;
                memset(tmp, 0, 32);
                continue;
            }
            tmp[z++] = command[i];
        }
        for (int i = 0; i < PL_CNT; i++) {
            printf("%d %s %d %d\n", i + 1, scoreboard[i].NAME, scoreboard[i].score, scoreboard[i].is_online);
        }
    }
    if (parsable_ret_ld) printf("<<%s\n", command);

    return command;
}


void try_login(SOCKET client, COMMAND_PROTOTYPE C, char *name, char *password) {
    C.TAG = CONNECTION;
    C.VALID_ARG_CNT = 2;
    strcpy(C.ARGS[0], name);
    strcpy(C.ARGS[1], password);
    while (strcmp(make_command(client, C), "LOGIN_SUCCESS") != 0) {
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
    C.TAG = LD_BOARD;
    C.VALID_ARG_CNT = 0;
    make_command(client, C);
}

void get_rooms(SOCKET client, COMMAND_PROTOTYPE C) {
    C.TAG = ROOMS;
    C.VALID_ARG_CNT = 0;
    make_command(client, C);
}


char *check_inv(SOCKET client, COMMAND_PROTOTYPE C) {
    C.TAG = ENTER;
    C.VALID_ARG_CNT = 0;
    make_command(client, C);

}

COMMAND_PROTOTYPE C;

#define LOGIN(NAME, PASS) try_login(client, C, NAME,PASS)
#define GET_LDB() upd_ld_board(client, C)
#define GO(X, Y) move_self(client, C, X, Y)
#define SAY(MSG) cli_send(client, C, MSG)
#define LEAVE() cli_exit(client, C)
#define LOBBY() get_rooms(client, C)

void startSession() {
    SOCKET client;
    client = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (client == INVALID_SOCKET) {
        printf("Error create socket\n");
        return;
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(5510); //the same as in server
//  server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //special look-up address
    server.sin_addr.S_un.S_addr = inet_addr("26.173.251.89");
    if (connect(client, (struct sockaddr *) &server, sizeof(server)) == SOCKET_ERROR) {
        printf("Can't connect to server\n");
        closesocket(client);
        return;
    }


    char name[PL_PARAM_SIZE];
    char pass[PL_PARAM_SIZE];

    printf("Welcome to the Maze!\n");
    printf("Nickname:");
    scanf("%s", name);
    printf("Password:");
    scanf("%s", pass);

    LOGIN(name, pass);
    while (1) {
        system("cls");
        GET_LDB();

        sleep(1);
    }
    GO(10, 10);
    LEAVE();
}

int main() {
    WSADATA wsd;
    if (WSAStartup(MAKEWORD(1, 1), &wsd) != 0) {
        printf("Can't connect to socket lib");
        return 1;
    }
    srand(time(0));
    rand();
    int number = abs(rand()) % 1000;
    char *name = malloc(sizeof(char) * 32);
    sprintf(name, "%d", number + 1488);
    startSession();
    printf("Session is closed\n");
    system("pause");
    return 0;
}
