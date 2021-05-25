#include <pthread.h>
#include <winsock.h>
#include <stdio.h>
#include <windows.h>

#include "../sqlite3.h"

pthread_mutex_t mutex;
pthread_mutex_t mutex_file;

typedef struct thread_in {
    sqlite3_mutex *cur_mutex;
    sqlite3 *db;
    SOCKET cur_socket;
} th_input;

void *ClientStart(void *param) {
    th_input *input_data = (th_input *) param;
    SOCKET client = input_data->cur_socket;
    char recieve[1024], transmit[1024];
    int ret;

    int state = sqlite3_mutex_try(input_data->cur_mutex);

    if (state == SQLITE_BUSY){
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex_file);
        fprintf(stderr, "[SQL ERROR] Cannot open database: it's already in use\n");
        pthread_mutex_unlock(&mutex_file);
        pthread_mutex_unlock(&mutex);
        sqlite3_mutex_leave(input_data->cur_mutex);
        return (void *) 3;
    }

    ret = recv(client, recieve, 1024, 0);
    if (!ret || ret == SOCKET_ERROR) {
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex_file);
        printf("Error getting data\n");
        fprintf(stdout, "test");
        pthread_mutex_unlock(&mutex_file);
        pthread_mutex_unlock(&mutex);
        sqlite3_mutex_leave(input_data->cur_mutex);
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
        sqlite3_mutex_leave(input_data->cur_mutex);
        return (void *) 2;
    }

    sqlite3_mutex_leave(input_data->cur_mutex);
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
    sqlite3_mutex *db_mutex = sqlite3_db_mutex(db);

    while (1) {
        size = sizeof(clientaddr);
        client = accept(server, (struct sockaddr *) &clientaddr, &size);

        if (client == INVALID_SOCKET) {
            printf("[SERVER] Can't accept client.\n");
            continue;
        } else {
            printf("[SERVER] Client is accepted.\n");
        }

        pthread_t client_thread;
        th_input cur_client = {.cur_mutex = db_mutex, .cur_socket = client, .db = db};
        status = pthread_create(&client_thread, NULL, ClientStart, (void *) &cur_client);
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

