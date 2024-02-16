/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** init_clients
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client/client.h"
#include "messages/codes.h"
#include "myftp.h"

void handle_clients(server_t *serv)
{
    client_t *client = NULL;
    struct clients_s *arr = NULL;

    if (serv == NULL)
        return;
    arr = &serv->clients;
    for (size_t i = 0; i < arr->size; i++) {
        client = &arr->arr[i];
        if (client->socket.fd == -1)
            continue;
        client_handle(client);
    }
}

static client_t *init_client(server_t *serv, client_t *client)
{
    socklen_t len = sizeof(client->socket);

    if (client == NULL)
        return NULL;
    memset(client, 0, sizeof(*client));
    client->socket.fd = accept(
        serv->socket.fd, (struct sockaddr *)&client->socket.sock_in, &len);
    if (client->socket.fd == -1)
        return NULL;
    client_write(client, MSG_220);
    return client;
}

client_t *new_client(server_t *serv, client_t *client)
{
    struct timeval tv = TV_CONNECT;
    fd_set fdread;
    fd_set fdwrite;
    int sock = serv->socket.fd;

    FD_ZERO(&fdread);
    FD_ZERO(&fdwrite);
    FD_SET(sock, &fdread);
    FD_SET(sock, &fdwrite);
    if (select(sock + 1, &fdread, &fdwrite, NULL, &tv) <= 0)
        return NULL;
    if (!FD_ISSET(sock, &fdread))
        return NULL;
    return init_client(serv, client);
}
