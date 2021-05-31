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

    SOCKET client_socket, server_socket;
} server_data;

int hash(char *s1, char *s2) {
    int base = 23;
    int res = 0;
    for (int i = 0; i < min(strlen(s1), strlen(s2)); i++) {
        res += (s1[i] + s2[i]) * base % (int) 1e6;
        base = base * base % (int) 1e6;
    }
    return res % (int) 1e6;
}

void *client_callback(void *param) {
    server_data *input_data = (server_data *) param;
    SOCKET client = input_data->client_socket;
    char *err;

    char string_id[10] = "";

    while (1) {
        char receive[1024], transmit[1024];
        int ret; // count of received bytes

        ret = recv(client, receive, 1024, 0);
        if (!ret || ret == SOCKET_ERROR) {
            char sql_update_online[128];
            sprintf(sql_update_online, "UPDATE Data SET Online = 0 WHERE Id = %s", string_id);

            while (sqlite3_mutex_try(input_data->db_mutex) == SQLITE_BUSY);
            int rc = sqlite3_exec(input_data->db, sql_update_online, 0, 0, &err);
            sqlite3_mutex_leave(input_data->db_mutex);

            if (rc != SQLITE_OK) {
                fprintf(stderr, "[SQL ERROR] %s\n", err);
                sqlite3_free(err);

                return (void *) CLIENT_THREAD_SQL_ERROR;
            }

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
            char sql_update_online[128];
            sprintf(sql_update_online, "UPDATE Data SET Online = 0 WHERE Id = %s", string_id);

            while (sqlite3_mutex_try(input_data->db_mutex) == SQLITE_BUSY);
            int rc = sqlite3_exec(input_data->db, sql_update_online, 0, 0, &err);
            sqlite3_mutex_leave(input_data->db_mutex);

            if (rc != SQLITE_OK) {
                fprintf(stderr, "[SQL ERROR] %s\n", err);
                sqlite3_free(err);

                return (void *) CLIENT_THREAD_SQL_ERROR;
            }
            break;
        }

        if (!strcmp(tag, "<new>")) {
            pthread_mutex_lock(input_data->p_mutex);
            pthread_mutex_lock(input_data->p_mutex_file);
            printf("%s\n", receive);
            pthread_mutex_unlock(input_data->p_mutex_file);
            pthread_mutex_unlock(input_data->p_mutex);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            int rc;
            char login[64], password[64];
            sscanf(receive, "%s %s %s", tag, login, password);

            itoa(hash(login, password), string_id, 10);

            char sql_if_exists[128];
            sprintf(sql_if_exists, "SELECT EXISTS(SELECT Id  FROM Data WHERE Id = %s LIMIT 1) AS exist;", string_id);
            int exists = 0;

            while (sqlite3_mutex_try(input_data->db_mutex) == SQLITE_BUSY);
            sqlite3_stmt *stmt = NULL;
            int retval = sqlite3_prepare_v2(input_data->db, sql_if_exists, -1, &stmt, 0);
            if (retval) {
                printf("Selecting data from DB Failed (err_code=%d)\n", retval);
                return (void *) 0;
            }
            int idx = 0;
            while (1) {
                retval = sqlite3_step(stmt);

                if (retval == SQLITE_ROW) {
                    int val = (int) sqlite3_column_int(stmt, 0);
                    if (val == 1) {
                        printf("Gay detected\n");
                        exists = 1;
                    }
                } else if (retval == SQLITE_DONE) {
                    break;
                } else {
                    sqlite3_finalize(stmt);
                    printf("Some error encountered\n");
                    break;
                }
            }
            sqlite3_finalize(stmt);
            sqlite3_mutex_leave(input_data->db_mutex);

            if (!exists) {
                char sql_add_client[128];
                sprintf(sql_add_client, "INSERT INTO Data VALUES(%s, \'%s\', \'%s\', 1);", string_id, login, password);

                while (sqlite3_mutex_try(input_data->db_mutex) == SQLITE_BUSY);
                rc = sqlite3_exec(input_data->db, sql_add_client, 0, 0, &err);
                sqlite3_mutex_leave(input_data->db_mutex);

                if (rc != SQLITE_OK) {
                    fprintf(stderr, "[SQL ERROR] %s\n", err);
                    sqlite3_free(err);

                    return (void *) CLIENT_THREAD_SQL_ERROR;
                }
            } else {
                char sql_update_online[128];
                sprintf(sql_update_online, "UPDATE Data SET Online = 1 WHERE Id = %s", string_id);

                while (sqlite3_mutex_try(input_data->db_mutex) == SQLITE_BUSY);
                rc = sqlite3_exec(input_data->db, sql_update_online, 0, 0, &err);
                sqlite3_mutex_leave(input_data->db_mutex);

                if (rc != SQLITE_OK) {
                    fprintf(stderr, "[SQL ERROR] %s\n", err);
                    sqlite3_free(err);

                    return (void *) CLIENT_THREAD_SQL_ERROR;
                }
            }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }

//        sprintf(transmit, "%s %s %s\n", "Your data", receive, "received");

        memset(transmit, 0, 1024);
        while (sqlite3_mutex_try(input_data->db_mutex) == SQLITE_BUSY);
        char sql_get_online[128] = "SELECT Login FROM Data WHERE Online = 1;";
        sqlite3_stmt *stmt = NULL;
        int retval = sqlite3_prepare_v2(input_data->db, sql_get_online, -1, &stmt, 0);
        if (retval) {
            printf("Selecting data from DB Failed (err_code=%d)\n", retval);
            return (void *) 0;
        }
        int idx = 0;
        while (1) {
            retval = sqlite3_step(stmt);

            if (retval == SQLITE_ROW) {
                char *val = (char *) sqlite3_column_text(stmt, 0);
                strcat(transmit, val);
                strcat(transmit, "\n");
            } else if (retval == SQLITE_DONE) {
                break;
            } else {
                sqlite3_finalize(stmt);
                printf("Some error encountered\n");
                break;
            }
        }
        sqlite3_finalize(stmt);
        sqlite3_mutex_leave(input_data->db_mutex);

        Sleep(2000);

        ret = send(client, transmit, sizeof(transmit), 0);
        if (ret == SOCKET_ERROR) {
            char sql_update_online[128];
            sprintf(sql_update_online, "UPDATE Data SET Online = 0 WHERE Id = %s", string_id);

            while (sqlite3_mutex_try(input_data->db_mutex) == SQLITE_BUSY);
            int rc = sqlite3_exec(input_data->db, sql_update_online, 0, 0, &err);
            sqlite3_mutex_leave(input_data->db_mutex);

            if (rc != SQLITE_OK) {
                fprintf(stderr, "[SQL ERROR] %s\n", err);
                sqlite3_free(err);

                return (void *) CLIENT_THREAD_SQL_ERROR;
            }
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

int getStopIssued() {
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

void *command_callback(void *param) {
    server_data *input_data = (server_data *) param;

    char command[128];

    while (1) {
        scanf("%s", command);
        if (!strcmp(command, "/off")) {
            closesocket(input_data->server_socket);
            setStopIssued(0);
            break;
        }
    }

    return (void *) 0;
}

void *server_listener(void *param) {
    server_data *input_data = (server_data *) param;

    SOCKET client;
    int status;

    while (getStopIssued()) {
        client = accept(input_data->server_socket, NULL, NULL);

        if (getStopIssued() == 0) {
            break;
        }

        if (client == INVALID_SOCKET) {
            printf("[SERVER] Can't accept client.\n");
            continue;
        } else {
            printf("[SERVER] Client is accepted.\n");
        }

        pthread_t tid;
        server_data data = *input_data;
        data.client_socket = client;

        status = pthread_create(&tid, NULL, client_callback, (void *) &data);
        if (status != 0) {
            fprintf(stderr, "[THREAD ERROR] create_server(): can't create \"tid\", status = %d\n", status);
            return (void *) 1;
        }
        status = pthread_detach(tid);
        if (status != 0) {
            fprintf(stderr, "[THREAD ERROR] create_server(): can't detach \"tid\", status = %d\n", status);
            return (void *) 1;
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

    const char *sql_create_table = "CREATE TABLE IF NOT EXISTS Data("
                                   "Id INT, "
                                   "Login TEXT, "
                                   "Password TEXT, "
                                   "Online INT);";

    rc = sqlite3_exec(db, sql_create_table, 0, 0, &err);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "[SQL ERROR] %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);

        return EXIT_FAILURE;
    }

    rc = sqlite3_exec(db, "UPDATE Data SET Online = 0", 0, 0, &err);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "[SQL ERROR] %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);

        return EXIT_FAILURE;
    }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    SOCKET server;
    struct sockaddr_in localaddr;
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    server_data server_data = {
            .p_mutex = &mutex,
            .p_mutex_file = &mutex_file,

            .db_mutex = db_mutex,
            .db = db,

            .client_socket = 0,
            .server_socket = server
    };

    pthread_t command_thread;
    status = pthread_create(&command_thread, NULL, command_callback, (void *) &server_data);
    if (status != 0) {
        fprintf(stderr, "[THREAD ERROR] create_server(): can't create \"command_thread\", status = %d\n", status);
        exit(EXIT_FAILURE);
    }
    status = pthread_detach(command_thread);
    if (status != 0) {
        fprintf(stderr, "[THREAD ERROR] create_server(): can't detach \"command_thread\", status = %d\n", status);
        exit(EXIT_FAILURE);
    }

    pthread_t server_thread;
    status = pthread_create(&server_thread, NULL, server_listener, (void *) &server_data);
    if (status != 0) {
        fprintf(stderr, "[THREAD ERROR] create_server(): can't create \"server_thread\", status = %d\n", status);
        exit(EXIT_FAILURE);
    }
    status = pthread_detach(server_thread);
    if (status != 0) {
        fprintf(stderr, "[THREAD ERROR] create_server(): can't detach \"server_thread\", status = %d\n", status);
        exit(EXIT_FAILURE);
    }

    while (getStopIssued());
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    pthread_mutex_destroy(&mutex_file);
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&stopMutex);

    printf("[SERVER] Server is stopped.\n");

    closesocket(server);

    sqlite3_mutex_enter(db_mutex);
    sqlite3_close(db);

    return 0;
}

typedef struct {

} ServerThreadData;

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

