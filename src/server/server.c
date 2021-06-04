#include <pthread.h>
#include <winsock.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>

#include "../sqlite3.h"

#define SUCCESS 0
#define RECEIVE_ERROR 1
#define SEND_ERROR 2

#define SQL_ERROR 10

#define _SERVER_DEBUG

char *get_current_time() {
    time_t r_time;
    time(&r_time);
    char *buffer = asctime(localtime(&r_time));
    buffer[strlen(buffer) - 1] = 0;
    return buffer;
}

int hash(char *s, int base) {
    int pow = 1;
    int length = (int) strlen(s);
    int value = 0;
    for (int i = 0; i < length; ++i) {
        value += (s[i] - 'a' + 1) * pow;
        pow *= base;
    }
    return value;
}

int hash_combine(char *s1, char *s2) {
    int h1 = hash(s1, 31);
    int h2 = hash(s2, 31);
    return (h1 << 1) ^ h2;
}

sqlite3 *db;
sqlite3_mutex *db_mutex;

pthread_mutex_t mutex_file;

void pthread_fprintf(char *format, ...) {
    va_list arg;
    va_start(arg, format);
    pthread_mutex_lock(&mutex_file);
    vprintf(format, arg);
    fflush(stdout);
    pthread_mutex_unlock(&mutex_file);
    va_end(arg);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DB things
int db_execute(char *sql, int (*callback)(void *, int, char **, char **), void *param, char *err) {
    sqlite3_mutex_enter(db_mutex); // may cause errors
    int rc = sqlite3_exec(db, sql, callback, param, &err);
    sqlite3_mutex_leave(db_mutex);

    if (rc != SQLITE_OK) {
        pthread_fprintf("[SQL ERROR] %s\n", err);
        sqlite3_free(err);

        return SQL_ERROR;
    }

    return SUCCESS;
}

// sql thread safe execution
#define SQL_THREAD_EXEC(sql, callback, arg, err) if (db_execute(sql, callback, arg, err) != SUCCESS) return (void *) SQL_ERROR

// callbacks

int callback_check_if_exists(void *param, int argc, char **argv, char **col_name) {
    int *exists = (int *) param;
    *exists = atoi(argv[0]);
    return 0;
}

int callback_select_all_online_users(void *param, int argc, char **argv, char **col_name) {
    char *online_users = (char *) param;
    for (int i = 0; i < argc; ++i) {
        strcat(online_users, argv[i]);
        strcat(online_users, "#");
    }
    return 0;
}

int callback_get_one_string(void *param, int argc, char **argv, char **col_name) {
    char *result = (char *) param;
    sprintf(result, "%s", argv[0]);
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void printf_server_error_prefix() {
    pthread_fprintf("[SERVER ERROR][%s] ", get_current_time());
}

void printf_server_prefix() {
    pthread_fprintf("[SERVER][%s] ", get_current_time());
}

typedef struct client_data_t {
    char login[64], password[64];
    char string_id[10];
} ClientData;

void printf_client_data(ClientData *client, int show_password) {
    pthread_fprintf("Client {login: \"%s\"", client->login);
    if (show_password) {
        pthread_fprintf(", password: \"%s\"} ", client->password);
    } else {
        pthread_fprintf("} ");
    }
}

#define _MESSAGE_LENGTH 1024

#define PRINTF_WITH_SERVER_AND_CLIENT_PREFIX(show_pass, ...) \
printf_server_prefix(); \
printf_client_data(&data, show_pass); \
pthread_fprintf(__VA_ARGS__);

#define _RECV() \
ret = recv(client, receive, _MESSAGE_LENGTH, 0); \
if (!ret || ret == SOCKET_ERROR) { \
    printf_server_error_prefix(); \
    pthread_fprintf("Can't receive data from the client {login: \"%s\"}.\n", data.login); \
    SQL_THREAD_EXEC(sql_update_online_0, 0, 0, err); \
    PRINTF_WITH_SERVER_AND_CLIENT_PREFIX(0, "disconnected.\n"); \
    return (void *) RECEIVE_ERROR; \
}

#define _SEND() \
ret = send(client, transmit, _MESSAGE_LENGTH, 0); \
if (ret == SOCKET_ERROR) { \
    printf_server_error_prefix(); \
    pthread_fprintf("Can't send data to the client {login: \"%s\"}.\n", data.login); \
    SQL_THREAD_EXEC(sql_update_online_0, 0, 0, err); \
    PRINTF_WITH_SERVER_AND_CLIENT_PREFIX(0, "disconnected.\n"); \
    return (void *) SEND_ERROR; \
}

void *client_callback(void *param) {
    SOCKET client = (SOCKET) param;
    char *err;

    int first_time = 1;

    char tag[64];
    ClientData data;

    char sql_update_online_0[128];
    char sql_update_online_1[128];
    char sql_if_exists_name[128];
    char sql_select_all_online_users[128];
    char sql_get_password_by_name[128];
    char sql_add_client[128];

    while (1) {
        char receive[_MESSAGE_LENGTH], transmit[_MESSAGE_LENGTH];
        int ret; // count of received bytes

        _RECV()

        receive[ret] = '\0';
        sscanf(receive, "%s", tag);

//        Sleep(500);

        if (!strcmp(tag, "<CONNECTION>") && first_time) {
            memset(tag, 0, 64);
            memset(data.login, 0, 64);
            memset(data.password, 0, 64);
            memset(data.string_id, 0, 10);

            sscanf(receive, "%s %s %s", tag, data.login, data.password);

            PRINTF_WITH_SERVER_AND_CLIENT_PREFIX(1, "connected.\n");

            itoa(hash_combine(data.login, data.password), data.string_id, 10);

            sprintf(sql_update_online_0, "UPDATE Data SET Online = 0 WHERE Id = %s", data.string_id);
            sprintf(sql_update_online_1, "UPDATE Data SET Online = 1 WHERE Id = %s", data.string_id);
            sprintf(sql_if_exists_name, "SELECT EXISTS(SELECT Id FROM Data WHERE Login = \'%s\' LIMIT 1) AS exist;",
                    data.login);
            sprintf(sql_select_all_online_users, "SELECT Login FROM Data WHERE Online = 1 ORDER BY Login;");
            sprintf(sql_get_password_by_name, "SELECT Password FROM Data WHERE Login = \'%s\';", data.login);
            sprintf(sql_add_client, "INSERT INTO Data VALUES(%s, \'%s\', \'%s\', 1);", data.string_id, data.login,
                    data.password);

            int exists = 0;
            SQL_THREAD_EXEC(sql_if_exists_name, callback_check_if_exists, (void *) &exists, err);

            if (!exists) {
                SQL_THREAD_EXEC(sql_add_client, 0, 0, err);
            } else {
                char stored_password[64];
                SQL_THREAD_EXEC(sql_get_password_by_name, callback_get_one_string, (void *) stored_password, err);
                if (strcmp(stored_password, data.password) != 0) {
                    PRINTF_WITH_SERVER_AND_CLIENT_PREFIX(0, "<- in danger.\n");

                    memset(transmit, 0, 1024);
                    sprintf(transmit, "LOGIN_FAILURE");

                    _SEND()

                    continue;
                }
                SQL_THREAD_EXEC(sql_update_online_1, 0, 0, err);
            }

            memset(transmit, 0, 1024);
            sprintf(transmit, "LOGIN_SUCCESS");

            _SEND()

            first_time = 0;

            continue;
        }

        if (!strcmp(tag, "<MENU>") && !first_time) {
            memset(transmit, 0, 1024);
            SQL_THREAD_EXEC(sql_select_all_online_users, callback_select_all_online_users, (void *) &transmit, err);

            _SEND()

            continue;
        }

        if (!strcmp(tag, "<EXIT>") && !first_time) {
            SQL_THREAD_EXEC(sql_update_online_0, 0, 0, err);
            PRINTF_WITH_SERVER_AND_CLIENT_PREFIX(0, "disconnected.\n");

            memset(transmit, 0, 1024);
            sprintf(transmit, "disconnected!");

            _SEND()

            break;
        }

        if (first_time == 1) {
            memset(transmit, 0, 1024);
            sprintf(transmit, "gay website 8===o"); // todo change reply.

            _SEND()

            continue;
        }

#ifdef _SERVER_DEBUG
        PRINTF_WITH_SERVER_AND_CLIENT_PREFIX(0, ": %s.\n", (strlen(receive) ? receive : "EMPTY MESSAGE"));
#endif

        memset(transmit, 0, 1024);
        sprintf(transmit, "I have nothing to say to you.");

        _SEND()
    }

    return (void *) SUCCESS;
}

void *command_callback(void *param) {
    SOCKET server = (SOCKET) param;

    char command[128];

    while (1) {
        scanf("%s", command);
        if (!strcmp(command, "/off")) {
            closesocket(server);
            setStopIssued(0);
            break;
        }
    }

    return (void *) 0;
}

void *server_listener(void *param) {
    SOCKET server = (SOCKET) param;

    SOCKET client;
    int status;

    while (getStopIssued()) {
        client = accept(server, NULL, NULL);

        if (getStopIssued() == 0) {
            break;
        }

        if (client == INVALID_SOCKET) {
            pthread_fprintf("[SERVER] Can't accept client.\n");
            continue;
        } else {
            pthread_fprintf("[SERVER] Client is accepted.\n");
        }

        pthread_t tid;
        status = pthread_create(&tid, NULL, client_callback, (void *) client);
        if (status != 0) {
            pthread_fprintf("[THREAD ERROR] create_server(): can't create \"tid\", status = %d\n", status);
            return (void *) 1;
        }
        status = pthread_detach(tid);
        if (status != 0) {
            pthread_fprintf("[THREAD ERROR] create_server(): can't detach \"tid\", status = %d\n", status);
            return (void *) 1;
        }
    }
}

int create_server() {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // DB things

    char *err = 0;

    int rc = sqlite3_open("test.db", &db);
    if (rc != SQLITE_OK) {
        printf("[SQL ERROR] Cannot open database: %s\n", sqlite3_errmsg(db));
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
        printf("[SQL ERROR] %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);

        return EXIT_FAILURE;
    }

    rc = sqlite3_exec(db, "UPDATE Data SET Online = 0", 0, 0, &err);

    if (rc != SQLITE_OK) {
        printf("[SQL ERROR] %s\n", err);
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
        printf("[SERVER ERROR] Can't start server session.\n");
        return EXIT_FAILURE;
    } else {
        printf("[SERVER] Server session is started.\n");
    }
    listen(server, 50); // max clients in the queue

    pthread_mutex_init(&mutex_file, NULL);
    pthread_mutex_init(&stopMutex, NULL);

    sqlite3_db_mutex(db);

    int status;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    pthread_t command_thread;
    status = pthread_create(&command_thread, NULL, command_callback, (void *) server);
    if (status != 0) {
        pthread_fprintf("[THREAD ERROR] create_server(): can't create \"command_thread\", status = %d\n",
                        status);
        exit(EXIT_FAILURE);
    }
    status = pthread_detach(command_thread);
    if (status != 0) {
        pthread_fprintf("[THREAD ERROR] create_server(): can't detach \"command_thread\", status = %d\n",
                        status);
        exit(EXIT_FAILURE);
    }

    pthread_t server_thread;
    status = pthread_create(&server_thread, NULL, server_listener, (void *) server);
    if (status != 0) {
        pthread_fprintf("[THREAD ERROR] create_server(): can't create \"server_thread\", status = %d\n",
                        status);
        exit(EXIT_FAILURE);
    }
    status = pthread_detach(server_thread);
    if (status != 0) {
        pthread_fprintf("[THREAD ERROR] create_server(): can't detach \"server_thread\", status = %d\n",
                        status);
        exit(EXIT_FAILURE);
    }

    while (getStopIssued());
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    pthread_mutex_destroy(&mutex_file);
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

