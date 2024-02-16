/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** myftp
*/

#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 6969
#define LISTEN_BACKLOG 5

#define GETIP(client) inet_ntoa((client).socket.sock_in.sin_addr)

enum {
    RET_VALID = 0,
    RET_ERROR = 84,
};

typedef enum {
    UNDEFINED,
    ST_JUST_CONNECTED,
} state_t;

static const struct timeval TV_CONNECT = {.tv_sec = 0, .tv_usec = 1000};
static const struct timeval TV_READ = {.tv_sec = 0, .tv_usec = 1000};

typedef struct {
    int fd;
    struct sockaddr_in sock_in;
} socket_t;

typedef struct {
    socket_t socket;
    state_t state;
} client_t;

typedef struct {
    struct clients_s {
        client_t *arr;
        size_t size;
        size_t alloc;
    } clients;
    socket_t socket;
    const char *path;
} server_t;

int myftp(int argc, char **argv);
client_t *new_client(server_t *serv, client_t *client);
void handle_clients(server_t *serv);
void client_handle(client_t *client);
int client_write(client_t *client, const char *fmt, ...);
void args_get(char **argv, int argc, server_t *serv);
