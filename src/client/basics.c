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

int client_write(client_t *client, const char *fmt, ...)
{
    va_list args1;
    va_list args2;
    int ret = 0;

    if (client == NULL || fmt == NULL)
        return 0;
    va_start(args1, fmt);
    va_copy(args2, args1);
    ret = vdprintf(client->socket.fd, fmt, args1);
    DEBUG_DO(vprintf(fmt, args2));
    va_end(args1);
    va_end(args2);
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
