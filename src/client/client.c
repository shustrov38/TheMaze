#include <winsock.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

SOCKET client;

typedef enum {
    //TAG:          // WHAT IT DOES:                     ARGS "<> ...":            DIR:
    CONNECTION,     // connect this client to server    "<CONNECTION> NAME PASS"   TO
    SYS_MSG,        // message STR from/to server       "<SYS_MSG> STR"            TO/FROM
    INVITE,         // invite NAME/invite from NAME     "<INVITE> NAME"            TO/FROM
    MOV_RIVAL,      // move enemy NAME to X Y           "<MOV_RIVAL> NAME X Y"     FROM
    MOV_SELF        // move this player to X Y          "<MOV_SELF> X Y"           TO
} INNER_INTERFACE;

typedef struct {
    INNER_INTERFACE TAG;
    char ARG_0[16];
    char ARG_1[16];
    char ARG_2[16];
    char ARG_3[16];
    char ARG_4[16];
} COMMAND_PROTOTYPE;

void make_command(COMMAND_PROTOTYPE proto) {
    char command[1024];
    memset(command, 0, 1024);
    switch (proto.TAG) {
        case CONNECTION:
            sprintf(command, "%s %s %s", "<CONNECTION>", proto.ARG_0, proto.ARG_1);
            break;
        case SYS_MSG:
            sprintf(command, "%s %s", "<SYS_MSG>", proto.ARG_0);
            break;
        case MOV_SELF:
            sprintf(command, "%s %s %s", "<MOV_SELF>", proto.ARG_0, proto.ARG_1);
            break;
    }
    printf("%s\n", command);
    send(client, command, 1024, 0);
}

COMMAND_PROTOTYPE C;

#define LOGIN(NAME, PASS)    C.TAG = CONNECTION;\
                            strcpy(C.ARG_0,NAME);\
                            strcpy(C.ARG_1,PASS);\
                            make_command(C)\

#define SAY(MSG)            C.TAG = SYS_MSG ;\
                            strcpy(C.ARG_0,MSG);\
                            make_command(C)\

#define GO(X, Y)             C.TAG = MOV_SELF;\
                            strcpy(C.ARG_0,itoa(X,C.ARG_0,10));\
                            strcpy(C.ARG_1,itoa(Y,C.ARG_1,10));\
                            make_command(C)\


//int stopIssued = 1;
//pthread_mutex_t stopMutex;
//
//int getStopIssued() {
//    int ret = 0;
//    pthread_mutex_lock(&stopMutex);
//    ret = stopIssued;
//    pthread_mutex_unlock(&stopMutex);
//    return ret;
//}
//
//void setStopIssued(int val) {
//    pthread_mutex_lock(&stopMutex);
//    stopIssued = val;
//    pthread_mutex_unlock(&stopMutex);
//}
//
//void *command_callback(void *param) {
//    SOCKET server = (SOCKET) param;
//    int status;
//
//    while (getStopIssued()) {
//
//
//        if(strlen(command)==0){
//            continue;
//        }
//
//        status = send(server, command, 128, 0);
//        memset(command,0,128);
//
//        if(status==SOCKET_ERROR){
//            printf("[CLIENT ERROR] An error occurred after trying to send command");
//            setStopIssued(0);
//        }
//
//    }
//    return (void *) 0;
//}

void startSession() {
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

//    setStopIssued(1);
//
//    int com_status;
//    pthread_t command_thread;
//    com_status = pthread_create(&command_thread, NULL, command_callback, (void *) client);
//    com_status = pthread_detach(command_thread);

//    int rec_status;
//    pthread_t rec_thread;
//    rec_status = pthread_create(&command_thread, NULL, command_callback, (void *) client);
//    rec_status = pthread_detach(command_thread);

    LOGIN(name, pass);
    //    for (int i = 0; i < 10000; ++i) {
//        char message[1024];
//        sprintf(message, "%s %d", "test", i);
//        int ret = send(client, message, (int) strlen(message), 0);
//        if (ret == SOCKET_ERROR) {
//            printf("Can't send message\n");
//            closesocket(client);
//            return;
//        }
//
//        printf("Sent: %s\nbytes: %d\n\n", message, ret);
//        ret = SOCKET_ERROR;
//        while (ret == SOCKET_ERROR) {
//            //полчение ответа
//            ret = recv(client, message, 1024, 0);
//            //обработка ошибок
//            if (ret == 0 || ret == WSAECONNRESET) {
//                printf("Connection closed\n");
//                break;
//            }
//            if (ret < 0) {
//                printf("Can't receive message\n");
//                closesocket(client);
//                return;
//            }
//            //вывод на экран количества полученных байт и сообщение
//            system("cls");
//            printf("Receive: \n%s\n bytes: %d\n", message, ret);
//        }
//    }

    closesocket(client);
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
    Sleep(1000);
    return 0;
}