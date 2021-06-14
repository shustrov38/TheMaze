#include <winsock.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include"client_server_interface/interface.h"

void __startSession() {
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
        system("cls");
        char buffer[256];
        memset(buffer,0,256);
        while(1) {
            fgets(buffer, 256, stdin);
            buffer[strlen(buffer)-1] = 0;
            SAY(buffer);
            memset(buffer,0,256);
        }
        sleep(100);
    DISCONNECT();
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