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
    INVITE,         // invite NAME/invite from NAME     "<INVITE> NAME"            TO/FROM
    MOV_RIVAL,      // move enemy NAME to X Y           "<MOV_RIVAL> NAME X Y"     FROM
    MOV_SELF,       // move this player to X Y          "<MOV_SELF> X Y"           TO
    EXIT,
    MENU
} INNER_INTERFACE;

typedef struct {
    INNER_INTERFACE TAG;
    char ARGS[16][16];
    int VALID_ARG_CNT;
} COMMAND_PROTOTYPE;

typedef struct {
    char NAME[PL_PARAM_SIZE];
    int score;
} PLAYER;

PLAYER scoreboard[PL_CNT];

char *make_command(SOCKET client, COMMAND_PROTOTYPE proto) {
    int f = 0;
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
        case INVITE:
            sprintf(command, "%s %s", "<INVITE>", proto.ARGS[0]);
            break;
        case EXIT:
            sprintf(command, "%s", "<EXIT>");
            break;
        case MENU:
            // flag - > parse+sort
            f = 1;
            sprintf(command, "%s", "<MENU>");
            break;
    }
    if (!f) printf(">>%s", command);
    send(client, command, TRANSACTION_CAPACITY, 0);
    recv(client, command, TRANSACTION_CAPACITY, 0);

    if (f) {
        int z = 0;
        int j = 0;
        char tmp[32];
        memset(tmp, 0, 32);
        for (int i = 0; i < strlen(command) && j < PL_CNT; i++) {
            if (command[i] == '#') {
                printf("%s->", tmp);
                z = 0;
                sscanf(tmp,"%s %d",scoreboard[j].NAME,&scoreboard[j].score);
                j++;
                memset(tmp, 0, 32);
                continue;
            }
            tmp[z++] = command[i];
        }
        for(int i = 0; i < PL_CNT; i++){
            printf("%d %s %d\n",i, scoreboard[i].NAME,scoreboard[i].score);
        }
    }
    if (f) printf("<<%s\n", command);

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

void upd_menu(SOCKET client, COMMAND_PROTOTYPE C) {
    C.TAG = MENU;
    C.VALID_ARG_CNT = 0;
    make_command(client, C);
}

COMMAND_PROTOTYPE C;

#define LOGIN(NAME, PASS) try_login(client, C, NAME,PASS)
#define GET_MENU() upd_menu(client, C)
#define GO(X, Y) move_self(client, C, X, Y)
#define SAY(MSG) cli_send(client, C, MSG)
#define LEAVE() cli_exit(client, C)

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
        GET_MENU();
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
