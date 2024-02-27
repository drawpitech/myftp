/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** write
*/

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client.h"
#include "debug.h"
#include "myftp.h"

int client_fd_write(int fd, client_t *client, const char *str, size_t size)
{
    if (client == NULL || str == NULL || client->state == NO_DATA_SOCK)
        return 0;
    DEBUG(
        "[fd: %d] writing data in %s mode (type %d): %lu bytes", fd,
        (client->state == PASSIVE_MODE) ? "passive" : "active",
        client->data_type, size);
    switch (client->data_type) {
        case DT_ASCII:
            DEBUG("%.*s", (int)size, str);
            return dprintf(fd, "%.*s", (int)size, str);
        case DT_IMAGE:
        case DT_LOCAL:
        case DT_EBCDIC:
            DEBUG_DO(write(STDOUT_FILENO, str, size));
            return (int)write(fd, str, size);
    }
    return 0;
}

static int copy_vdprintf(int fd, const char *fmt, va_list args)
{
    va_list args_copy;
    int ret = 0;

    va_copy(args_copy, args);
    ret = vdprintf(fd, fmt, args_copy);
    va_end(args_copy);
    return ret;
}

int client_write(client_t *client, const char *fmt, ...)
{
    va_list args;
    int ret = 0;

    va_start(args, fmt);
    DEBUG_DO(copy_vdprintf(STDOUT_FILENO, fmt, args));
    ret = copy_vdprintf(client->socket.fd, fmt, args);
    va_end(args);
    return ret;
}
