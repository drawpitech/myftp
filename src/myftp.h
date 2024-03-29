/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** myftp
*/

#pragma once

#include <arpa/inet.h>
#include <linux/limits.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>

#define LISTEN_BACKLOG 5

#define GETIP(client) inet_ntoa((client).socket.sock_in.sin_addr)

#define ATTR(x) __attribute__((x))
#define UNUSED ATTR(unused)

#define LEN_OF(x) (sizeof(x) / sizeof*(x))

extern const char *const __progname;

enum {
    RET_VALID = 0,
    RET_ERROR = 84,
};

static const struct timeval TV_CONNECT = {.tv_sec = 0, .tv_usec = 1000};
static const struct timeval TV_READ = {.tv_sec = 0, .tv_usec = 1000};

typedef struct {
    int fd;
    struct sockaddr_in sock_in;
} socket_t;

typedef struct {
    socket_t socket;
    char username[BUFSIZ];
    char path[PATH_MAX];
    bool logged;
    enum {
        NO_DATA_SOCK,
        PASSIVE_MODE,
        ACTIVE_MODE,
    } state;
    socket_t data_socket;
    char buffer[BUFSIZ];
} client_t;


typedef struct {
    struct clients_s {
        client_t *arr;
        size_t size;
        size_t alloc;
    } clients;
    socket_t socket;
    char path[PATH_MAX];
} server_t;

int myftp(int argc, char **argv);

int ret_error(const char *name, int value);
void args_get(char **argv, int argc, server_t *serv);
char *get_path(const char *parent, const char *child, char result[PATH_MAX]);
