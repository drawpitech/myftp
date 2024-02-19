/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** basics
*/

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client.h"
#include "debug.h"
#include "messages/codes.h"
#include "messages/messages.h"
#include "myftp.h"

void client_disconnect(client_t *client)
{
    if (client == NULL)
        return;
    close(client->socket.fd);
    DEBUG(
        "client disconnected: %s:%u\n", GETIP(*client),
        client->socket.sock_in.sin_port);
    memset(client, 0, sizeof(*client));
    client->socket.fd = -1;
}

client_t *client_init(client_t *client, server_t *serv)
{
    socklen_t len = sizeof(client->socket);

    if (client == NULL)
        return NULL;
    memset(client, 0, sizeof(*client));
    client->socket.fd = accept(
        serv->socket.fd, (struct sockaddr *)&client->socket.sock_in, &len);
    if (client->socket.fd == -1)
        return NULL;
    strcpy(client->path, serv->path);
    client_write(client, MSG_220);
    return client;
}

static bool client_wrote(client_t *client)
{
    fd_set fdread;
    fd_set fdwrite;
    struct timeval tv = TV_READ;

    if (client == NULL)
        return false;
    FD_ZERO(&fdread);
    FD_ZERO(&fdwrite);
    FD_SET(client->socket.fd, &fdread);
    FD_SET(client->socket.fd, &fdwrite);
    if (select(client->socket.fd + 1, &fdread, NULL, NULL, &tv) <= 0)
        return false;
    return FD_ISSET(client->socket.fd, &fdread);
}

int client_fd_write(int fd, client_t *client, const char *str, size_t size)
{
    switch (client->state) {
        case NO_DATA_SOCK:
            DEBUG_MSG("writing no data sock");
            return 0;
        case PASSIVE_MODE:
            DEBUG("writing passive mode: %lu bytes", size);
            return (int)write(fd, str, size);
    }
    return 0;
}

static int client_wr(int fd, const char *fmt, va_list args)
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
    DEBUG_DO(client_wr(STDOUT_FILENO, fmt, args));
    ret = client_wr(client->socket.fd, fmt, args);
    va_end(args);
    return ret;
}

void client_handle(client_t *client)
{
    static char buffer[BUFSIZ + 1];
    ssize_t size = 0;

    if (client == NULL || !client_wrote(client))
        return;
    size = read(client->socket.fd, buffer, BUFSIZ);
    if (size == 0 || size == -1) {
        client_disconnect(client);
        return;
    }
    buffer[size] = 0;
    client_process_message(client, buffer);
}
