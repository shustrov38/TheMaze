#include <winsock.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void SendData2Server(int number, char *name) {
    SOCKET client;
    client = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (client == INVALID_SOCKET) {
        printf("Error create socket\n");
        return;
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(5510); //the same as in server
    server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //special look-up address
//    server.sin_addr.S_un.S_addr = inet_addr("26.173.251.89");
    if (connect(client, (struct sockaddr *) &server, sizeof(server)) == SOCKET_ERROR) {
        printf("Can't connect to server\n");
        closesocket(client);
        return;
    }

    char new_msg[128];
    sprintf(new_msg, "<new> %d %s", number, name);

    int ret = send(client, new_msg, 128, 0);
    if (ret == SOCKET_ERROR) {
        printf("Can't send message\n");
        closesocket(client);
        return;
    }

    for (int i = 0; i < 1000; ++i) {
        char message[1024];

        sprintf(message, "<%d> %s %d", number, "test", i);
        int ret = send(client, message, (int) strlen(message), 0);
        if (ret == SOCKET_ERROR) {
            printf("Can't send message\n");
            closesocket(client);
            return;
        }

        printf("Sent: %s\nbytes: %d\n\n", message, ret);
        ret = SOCKET_ERROR;
        while (ret == SOCKET_ERROR) {
            //полчение ответа
            ret = recv(client, message, 1024, 0);
            //обработка ошибок
            if (ret == 0 || ret == WSAECONNRESET) {
                printf("Connection closed\n");
                break;
            }
            if (ret < 0) {
                printf("Can't receive message\n");
                closesocket(client);
                return;
            }
            //вывод на экран количества полученных байт и сообщение
            printf("Receive: %s\n bytes: %d\n", message, ret);
        }
    }

    ret = send(client, "<end>", 5, 0);
    if (ret == SOCKET_ERROR) {
        printf("Can't send message\n");
        closesocket(client);
        return;
    }

    closesocket(client);
}

int main() {
    WSADATA wsd;
    if (WSAStartup(MAKEWORD(1, 1), &wsd) != 0) {
        printf("Can't connect to socket lib");
        return 1;
    }
    int i = 0;
    srand(time(0));
    rand();
    int number = rand();
    SendData2Server(number, "pipiska");
    printf("Session is closed\n");
    Sleep(1000);
    return 0;
}

