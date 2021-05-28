#include <pthread.h>
#include <winsock.h>
#include <stdio.h>
#include <windows.h>

#include "../sqlite3.h"

#define CLIENT_THREAD_OK 0
#define CLIENT_THREAD_RECEIVE_ERROR 1
#define CLIENT_THREAD_SEND_ERROR 2

#define CLIENT_THREAD_SQL_ERROR 10

typedef struct server_data_t {
    pthread_mutex_t *p_mutex;
    pthread_mutex_t *p_mutex_file;

    sqlite3_mutex *db_mutex;
    sqlite3 *db;

    SOCKET client_socket;
} server_data;

void *client_callback(void *param) {
    server_data *input_data = (server_data *) param;
    SOCKET client = input_data->client_socket;
    char *err;

    while (1) {
        char receive[1024], transmit[1024];
        int ret; // count of received bytes

        ret = recv(client, receive, 1024, 0);
        if (!ret || ret == SOCKET_ERROR) {
            pthread_mutex_lock(input_data->p_mutex);
            pthread_mutex_lock(input_data->p_mutex_file);
            fprintf(stderr, "[SERVER ERROR] Can't receive data.\n");
            pthread_mutex_unlock(input_data->p_mutex_file);
            pthread_mutex_unlock(input_data->p_mutex);
            return (void *) CLIENT_THREAD_RECEIVE_ERROR;
        }

        receive[ret] = '\0';

        char tag[64];
        sscanf(receive, "%s", tag);

        if (!strcmp(tag, "<end>")) {
            break;
        }

        if (!strcmp(tag, "<new>")) {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            char id[32], name[64];
            sscanf(receive, "%s %s %s", tag, id, name);

            while (sqlite3_mutex_try(input_data->db_mutex) == SQLITE_BUSY);

            char sql_add_client[128];
            sprintf(sql_add_client, "INSERT INTO Data VALUES(%s, \'%s\');", id, name);

            int rc = sqlite3_exec(input_data->db, sql_add_client, 0, 0, &err);

            if (rc != SQLITE_OK) {
                fprintf(stderr, "[SQL ERROR] %s\n", err);
                sqlite3_free(err);

                return (void *) CLIENT_THREAD_SQL_ERROR;
            }

            sqlite3_mutex_leave(input_data->db_mutex);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }

        pthread_mutex_lock(input_data->p_mutex);
        pthread_mutex_lock(input_data->p_mutex_file);
        printf("%s\n", receive);
        pthread_mutex_unlock(input_data->p_mutex_file);
        pthread_mutex_unlock(input_data->p_mutex);

        sprintf(transmit, "%s %s %s\n", "Your data", receive, "received");

//    Sleep(2000);

        ret = send(client, transmit, sizeof(transmit), 0);
        if (ret == SOCKET_ERROR) {
            pthread_mutex_lock(input_data->p_mutex);
            pthread_mutex_lock(input_data->p_mutex_file);
            fprintf(stderr, "[SERVER ERROR] Can't send data.\n");
            pthread_mutex_unlock(input_data->p_mutex_file);
            pthread_mutex_unlock(input_data->p_mutex);
            return (void *) CLIENT_THREAD_SEND_ERROR;
        }
    }

    return (void *) CLIENT_THREAD_OK;
}

int stopIssued = 1;
pthread_mutex_t stopMutex;

int getStopIssued(void) {
    int ret = 0;
    pthread_mutex_lock(&stopMutex);
    ret = stopIssued;
    pthread_mutex_unlock(&stopMutex);
    return ret;
}

void setStopIssued(int val) {
    pthread_mutex_lock(&stopMutex);
    stopIssued = val;
    pthread_mutex_unlock(&stopMutex);
}

_Noreturn void *command_callback(void *param) {
    server_data *input_data = (server_data *) param;

    char command[128];

    while (1) {
        scanf("%s", command);
        if (!strcmp(command, "/off")) {
            setStopIssued(0);

            pthread_mutex_destroy(input_data->p_mutex_file);
            pthread_mutex_destroy(input_data->p_mutex);

            printf("[SERVER] Server is stopped.\n");

            closesocket(input_data->client_socket);

            sqlite3_close(input_data->db);

            exit(0);
        }
    }
}

int create_server() {
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

    const char *sql_create_table = "DROP TABLE IF EXISTS Data; CREATE TABLE Data(Id INT, Name TEXT);";

    rc = sqlite3_exec(db, sql_create_table, 0, 0, &err);

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
    localaddr.sin_port = htons(5510); // port number is for example, must be more than 1024
    if (bind(server, (struct sockaddr *) &localaddr, sizeof(localaddr)) == SOCKET_ERROR) {
        fprintf(stderr, "[SERVER ERROR] Can't start server session.\n");
        return EXIT_FAILURE;
    } else {
        printf("[SERVER] Server session is started.\n");
    }

    listen(server, 50); // max clients in the queue

    pthread_mutex_t mutex;
    pthread_mutex_t mutex_file;

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_file, NULL);

    pthread_mutex_init(&stopMutex, NULL);

    sqlite3_mutex *db_mutex = sqlite3_db_mutex(db);

    int status;

    {
        pthread_t command_thread;
        server_data command_data = {
                .p_mutex = &mutex,
                .p_mutex_file = &mutex_file,

                .db_mutex = db_mutex,
                .db = db,

                .client_socket = server
        };
        status = pthread_create(&command_thread, NULL, command_callback, (void *) &command_data);
        if (status != 0) {
            fprintf(stderr, "[THREAD ERROR] create_server(): can't create \"command_thread\", status = %d\n", status);
            exit(EXIT_FAILURE);
        }
        status = pthread_detach(command_thread);
        if (status != 0) {
            fprintf(stderr, "[THREAD ERROR] create_server(): can't detach \"command_thread\", status = %d\n", status);
            exit(EXIT_FAILURE);
        }
    }

    while (getStopIssued()) {
        size = sizeof(clientaddr);
        client = accept(server, (struct sockaddr *) &clientaddr, &size);

        if (client == INVALID_SOCKET) {
            printf("[SERVER] Can't accept client.\n");
            continue;
        } else {
            printf("[SERVER] Client is accepted.\n");
        }

        pthread_t tid;
        server_data data = {
                .p_mutex = &mutex,
                .p_mutex_file = &mutex_file,

                .db_mutex = db_mutex,
                .db = db,

                .client_socket = client
        };

        status = pthread_create(&tid, NULL, client_callback, (void *) &data);
        if (status != 0) {
            fprintf(stderr, "[THREAD ERROR] create_server(): can't create \"tid\", status = %d\n", status);
            exit(EXIT_FAILURE);
        }
        status = pthread_detach(tid);
        if (status != 0) {
            fprintf(stderr, "[THREAD ERROR] create_server(): can't detach \"tid\", status = %d\n", status);
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_destroy(&mutex_file);
    pthread_mutex_destroy(&mutex);

    printf("[SERVER] Server is stopped.\n");

    closesocket(server);

    sqlite3_close(db);

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

    return create_server();
}

