#include <winsock.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int hash (char *s1, char *s2){
    int base = 23;
    int res = 0;
    for(int i = 0; i < min(strlen(s1),strlen(s2)); i++){
        res+= (s1[i] + s2[i]) * base;
        base *= base;
    }
    return res % (int)10e6;
}

void SendData2Server(char *name) {
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

    char *pass = malloc(sizeof(char) * 32);
    memset(pass, 0, 32);
    strcat(pass, "qwerty");
    strcat(pass, name);

    sprintf(new_msg, "<new> %s %s", name, pass);

    int ret = send(client, new_msg, 128, 0);
    if (ret == SOCKET_ERROR) {
        printf("Can't send message\n");
        closesocket(client);
        return;
    }

    for (int i = 0; i < 10000; ++i) {
        char message[1024];
        sprintf(message, "%s %d", "test", i);
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
            system("cls");
            printf("Receive: \n%s\n bytes: %d\n", message, ret);
        }
    }

    sprintf(new_msg, "<end>");
    ret = send(client, new_msg, 5, 0);
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
    int number = abs(rand()) % 1000;
    char *name = malloc(sizeof(char) * 32);
    sprintf(name, "%d",number+1488);
    SendData2Server(name);
    printf("Session is closed\n");
    Sleep(1000);
    return 0;
}
