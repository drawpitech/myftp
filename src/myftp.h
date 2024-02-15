/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** myftp
*/

#pragma once

#include <netinet/in.h>

#define PORT 6969
#define LISTEN_BACKLOG 5

#define GETIP(client) inet_ntoa((client).socket.sock_in.sin_addr)

enum {
    RET_VALID = 0,
    RET_ERROR = 84,
};

static const struct timeval TV_CONNECT = {.tv_sec = 0, .tv_usec = 100};
static const struct timeval TV_READ = {.tv_sec = 0, .tv_usec = 100};

typedef struct {
    int fd;
    struct sockaddr_in sock_in;
} socket_t;

typedef struct {
    socket_t socket;
} client_t;

typedef struct {
    struct clients_s {
        client_t *arr;
        size_t size;
        size_t alloc;
    } clients;
    socket_t socket;
} server_t;

int myftp(int argc, char **argv);
