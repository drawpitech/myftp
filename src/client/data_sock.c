/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** data_sock
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "client/client.h"
#include "debug.h"
#include "myftp.h"

static int get_passive_mode(client_t *client)
{
    struct timeval tv = TV_CONNECT;
    fd_set fdread;
    fd_set fdwrite;
    socket_t *sock = &client->data_socket;
    socklen_t len = sizeof(sock->sock_in);

    FD_ZERO(&fdread);
    FD_ZERO(&fdwrite);
    FD_SET(sock->fd, &fdread);
    FD_SET(sock->fd, &fdwrite);
    if (select(sock->fd + 1, &fdread, &fdwrite, NULL, &tv) <= 0)
        return -1;
    return accept(sock->fd, (struct sockaddr *)&sock->sock_in, &len);
}

static int get_active_mode(client_t *client)
{
    socket_t *sock = &client->data_socket;

    sock->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock->fd == -1)
        return -1;
    DEBUG("data socket: %d", sock->fd);
    return connect(
        sock->fd, (struct sockaddr *)&sock->sock_in, sizeof(sock->sock_in));
}

int client_get_data_sock(client_t *client)
{
    switch (client->state) {
        case NO_DATA_SOCK:
            return -1;
        case PASSIVE_MODE:
            return get_passive_mode(client);
        case ACTIVE_MODE:
            return get_active_mode(client);
    }
    return -1;
}

void client_close_data_sock(client_t *client)
{
    switch (client->state) {
        case NO_DATA_SOCK:
            break;
        case PASSIVE_MODE:
        case ACTIVE_MODE:
            close(client->data_socket.fd);
            break;
    }
    client->state = NO_DATA_SOCK;
    memset(&client->data_socket, 0, sizeof(client->data_socket));
}
