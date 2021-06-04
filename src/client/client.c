#include <winsock.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#define TRANSACTION_CAPACITY 1024

typedef enum {
    //TAG:          // WHAT IT DOES:                     ARGS "<> ...":            DIR:
    CONNECTION,     // connect this client to server    "<CONNECTION> NAME PASS"   TO
    SYS_MSG,        // message STR from/to server       "<SYS_MSG> STR"            TO/FROM
    INVITE,         // invite NAME/invite from NAME     "<INVITE> NAME"            TO/FROM
    MOV_RIVAL,      // move enemy NAME to X Y           "<MOV_RIVAL> NAME X Y"     FROM
    MOV_SELF,        // move this player to X Y          "<MOV_SELF> X Y"           TO
    EXIT,
    MENU
} INNER_INTERFACE;

typedef struct {
    INNER_INTERFACE TAG;
    char ARGS[16][16];
    int VALID_ARG_CNT;
} COMMAND_PROTOTYPE;

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
    if(!f) printf(">>%s\n", command);
    send(client, command, TRANSACTION_CAPACITY, 0);
    recv(client, command, TRANSACTION_CAPACITY, 0);

    if(f) {
        int z = 0;
        char tmp[32];
        memset(tmp, 0, 32);
        for (int i = 0; i < strlen(command); i++) {
            if (command[i] == '#') {
                printf("%s\n", tmp);
                z = 0;
                memset(tmp, 0, 32);
                continue;
            }
            tmp[z++] = command[i];
        }
    }
    if(!f) printf("<<%s\n", command);

    return command;
}

COMMAND_PROTOTYPE C;

#define LOGIN(NAME, PASS)       C.TAG = CONNECTION; \
                                C.VALID_ARG_CNT = 2;\
                                strcpy(C.ARGS[0],NAME);\
                                strcpy(C.ARGS[1],PASS);\
                                while(strcmp(make_command(client,C),"LOGIN_SUCCESS")!=0){ \
                                    printf("Wrong password! Try again:\n");\
                                    printf("Nickname:");\
                                    scanf("%s", NAME);\
                                    printf("Password:");\
                                    scanf("%s", PASS);\
                                    strcpy(C.ARGS[0],NAME);\
                                    strcpy(C.ARGS[1],PASS);\
                                }

#define LEAVE()                 C.TAG = EXIT; \
                                C.VALID_ARG_CNT = 0;\
                                make_command(client,C); \
                                closesocket(client)

#define SAY(MSG)                C.TAG = SYS_MSG ;\
                                C.VALID_ARG_CNT = 1;\
                                strcpy(C.ARGS[0],MSG);\
                                make_command(client,C)

#define GO(X, Y)                C.TAG = MOV_SELF; \
                                C.VALID_ARG_CNT = 2;\
                                strcpy(C.ARGS[0],itoa(X,C.ARGS[0],10));\
                                strcpy(C.ARGS[1],itoa(Y,C.ARGS[1],10));\
                                make_command(client,C)

#define GET_MENU()              C.TAG = MENU; \
                                C.VALID_ARG_CNT = 0; \
                                make_command(client,C)

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


    char name[32];
    char pass[32];

    printf("Welcome to the Maze!\n");
    printf("Nickname:");
    scanf("%s", name);
    printf("Password:");
    scanf("%s", pass);

    LOGIN(name, pass);
    while(1){
        system("cls");
        GET_MENU();
        sleep(1);
    }
    GO(10,10);

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
