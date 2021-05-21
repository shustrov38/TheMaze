#include <pthread.h>
#include <winsock.h>
#include <stdio.h>
#include <windows.h>

pthread_mutex_t mutex;
pthread_mutex_t mutex_file;

void* ClientStart(void* param)
{
    SOCKET client = (SOCKET)param;
    char recieve[1024], transmit[1024];
    int ret;

    ret = recv(client, recieve, 1024, 0);
    if (!ret || ret == SOCKET_ERROR)
    {
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex_file);
        printf("Error getting data\n");
        fprintf(stdout, "test");
        pthread_mutex_unlock(&mutex_file);
        pthread_mutex_unlock(&mutex);
        return (void*)1;
    }
    recieve[ret] = '\0';
    //pthread_mutex_lock(&mutex);
    //pthread_mutex_lock(&mutex_file);
    fprintf(stdout, "test");
    printf("%s\n", recieve);
    //pthread_mutex_unlock(&mutex_file);
    //pthread_mutex_unlock(&mutex);
    sprintf(transmit, "%s %s %s\n", "Your data", recieve, "recieved");
    //Sleep(2000);
    ret = send(client, transmit, sizeof(transmit), 0);
    if (ret == SOCKET_ERROR)
    {
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex_file);
        printf("Error sending data\n");
        pthread_mutex_unlock(&mutex_file);
        pthread_mutex_unlock(&mutex);
        return (void*)2;
    }

    return (void*)0;
}

int CreateServer()
{
    SOCKET server, client;
    struct sockaddr_in localaddr, clientaddr;
    int size;
    server = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (server == INVALID_SOCKET)
    {
        printf("Error create server\n");
        return 1;
    }
    localaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(5510);//port number is for example, must be more than 1024
    if (bind(server, (struct sockaddr*)&localaddr, sizeof(localaddr)) == SOCKET_ERROR)
    {
        printf("Can't start server\n");
        return 2;
    }
    else
    {
        printf("Server is started\n");
    }
    listen(server, 50);//50 клиентов в очереди могут стоять
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_file, NULL);
    while (1)
    {
        size = sizeof(clientaddr);
        client = accept(server, (struct sockaddr*)&clientaddr, &size);

        if (client == INVALID_SOCKET)
        {
            printf("Error accept client\n");
            continue;
        }
        else
        {
            printf("Client is accepted\n");
        }
        pthread_t mythread;
        int status = pthread_create(&mythread, NULL, ClientStart, (void*)client);
        pthread_detach(mythread);
    }
    pthread_mutex_destroy(&mutex_file);
    pthread_mutex_destroy(&mutex);
    printf("Server is stopped\n");
    closesocket(server);
    return 0;
}

int main()
{
    WSADATA wsd;
    if (WSAStartup(MAKEWORD(1, 1), &wsd) == 0)
    {
        printf("Connected to socket lib\n");
    }
    else
    {
        return 1;
    }
    return CreateServer();
}

