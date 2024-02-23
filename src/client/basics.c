/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** basics
*/

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
    client->state = NO_DATA_SOCK;
    client->data_type = DT_ASCII;
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
