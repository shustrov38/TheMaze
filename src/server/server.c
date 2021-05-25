#include <pthread.h>
#include <winsock.h>
#include <stdio.h>
#include <windows.h>

#include "../sqlite3.h"

pthread_mutex_t mutex;
pthread_mutex_t mutex_file;

typedef struct thread_data {
    int isAlive;
    int showStatus;
} thread_data;

void *parseCommand(void *param) {
    thread_data *tdata=(thread_data *)param;

    char command[100];
    while (1) {
        pthread_mutex_lock(&mutex_file);
        scanf("%s", command);
        pthread_mutex_unlock(&mutex_file);
        if (!strcmp("/off", command)) {
            pthread_mutex_lock(&mutex_file);
            printf("[COMMAND] processing /off command...\n");
            pthread_mutex_unlock(&mutex_file);
            tdata->isAlive = 0;
            break;
        }
    }
    return (void *) 0;
}

void *ClientStart(void *param) {
    SOCKET client = (SOCKET) param;
    char recieve[1024], transmit[1024];
    int ret;

    ret = recv(client, recieve, 1024, 0);
    if (!ret || ret == SOCKET_ERROR) {
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex_file);
        printf("Error getting data\n");
        fprintf(stdout, "test");
        pthread_mutex_unlock(&mutex_file);
        pthread_mutex_unlock(&mutex);
        return (void *) 1;
    }
    recieve[ret] = '\0';
    pthread_mutex_lock(&mutex);
    pthread_mutex_lock(&mutex_file);
    fprintf(stdout, "test");
    printf("%s\n", recieve);
    pthread_mutex_unlock(&mutex_file);
    pthread_mutex_unlock(&mutex);
    sprintf(transmit, "%s %s %s\n", "Your data", recieve, "recieved");
    Sleep(2000);
    ret = send(client, transmit, sizeof(transmit), 0);
    if (ret == SOCKET_ERROR) {
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex_file);
        printf("Error sending data\n");
        pthread_mutex_unlock(&mutex_file);
        pthread_mutex_unlock(&mutex);
        return (void *) 2;
    }

    return (void *) 0;
}

int CreateServer() {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // DB things

    sqlite3 *db;
    char *err = 0;

    int rc = sqlite3_open("test.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[SQL ERROR] Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return EXIT_FAILURE;
    }

    rc = sqlite3_exec(db, "DROP TABLE IF EXISTS Data; CREATE TABLE Data(Id INT, Name TEXT);", 0, 0, &err);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "[SQL ERROR] %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);

        return EXIT_FAILURE;
    }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    SOCKET server, client;
    struct sockaddr_in localaddr, clientaddr;
    int size;
    server = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (server == INVALID_SOCKET) {
        printf("[SERVER ERROR] Can't create server instance.\n");
        return EXIT_FAILURE;
    }
    localaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(5510);//port number is for example, must be more than 1024
    if (bind(server, (struct sockaddr *) &localaddr, sizeof(localaddr)) == SOCKET_ERROR) {
        printf("[SERVER ERROR] Can't start server session.\n");
        return EXIT_FAILURE;
    } else {
        printf("[SERVER] Server session is started.\n");
    }

    listen(server, 50);//50 клиентов в очереди могут стоять
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_file, NULL);

    int status;

    thread_data command_thread_data = {.isAlive = 1};

    pthread_t command_thread;
    status = pthread_create(&command_thread, NULL, parseCommand, (void *)&command_thread_data);
    if (status != 0) {
        printf("[THREAD ERROR] CreateServer(): can't create command_thread, status = %d\n", status);
        exit(EXIT_FAILURE);
    }
    status = pthread_join(command_thread, NULL);
    if (status != 0) {
        printf("[THREAD ERROR] CreateServer(): can't join command_thread, status = %d\n", status);
        exit(EXIT_FAILURE);
    }

    while (command_thread_data.isAlive) {
        size = sizeof(clientaddr);
        client = accept(server, (struct sockaddr *) &clientaddr, &size);

        if (client == INVALID_SOCKET) {
            printf("[SERVER] Can't accept client.\n");
            continue;
        } else {
            printf("[SERVER] Client is accepted.\n");
        }

        pthread_t client_thread;
        status = pthread_create(&client_thread, NULL, ClientStart, (void *) client);
        if (status != 0) {
            printf("[THREAD ERROR] CreateServer(): can't create client_thread, status = %d\n", status);
            exit(EXIT_FAILURE);
        }
        status = pthread_detach(client_thread);
        if (status != 0) {
            printf("[THREAD ERROR] CreateServer(): can't detatch client_thread, status = %d\n", status);
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_destroy(&mutex_file);
    pthread_mutex_destroy(&mutex);
    printf("[SERVER] Server is stopped.\n");

    closesocket(server);

    return 0;
}

int main() {
    WSADATA wsd;

    if (WSAStartup(MAKEWORD(1, 1), &wsd) == 0) {
        printf("[INIT] Socket lib was linked properly.\n");
    } else {
        printf("[INIT] Socket lib was not properly linked.\n");
        return EXIT_FAILURE;
    }

    return CreateServer();
}

