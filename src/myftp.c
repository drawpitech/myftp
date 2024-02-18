/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** myftp
*/

#include "myftp.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "client/client.h"
#include "debug.h"

static void handle_clients(server_t *serv)
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

static int open_serv(server_t *serv)
{
    if (serv == NULL)
        return -1;
    serv->socket.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serv->socket.fd == -1)
        return ret_error("socket", RET_ERROR);
    serv->socket.sock_in.sin_family = AF_INET;
    serv->socket.sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(
            serv->socket.fd, (struct sockaddr *)&serv->socket.sock_in,
            sizeof(serv->socket.sock_in)) == -1) {
        close(serv->socket.fd);
        return ret_error("bind", RET_ERROR);
    }
    if (listen(serv->socket.fd, LISTEN_BACKLOG) != 0) {
        close(serv->socket.fd);
        return ret_error("listen", RET_ERROR);
    }
    return RET_VALID;
}

static server_t *add_client(server_t *serv, client_t *client)
{
    struct clients_s *arr = NULL;

    if (serv == NULL)
        return NULL;
    arr = &serv->clients;
    for (size_t i = 0; i < arr->size; i++) {
        if (arr->arr[i].socket.fd == -1) {
            arr->arr[i] = *client;
            return serv;
        }
    }
    if (arr->size + 1 > arr->alloc) {
        arr->alloc = (arr->alloc) ? arr->alloc * arr->alloc : 2;
        arr->arr = reallocarray(arr->arr, arr->alloc, sizeof(*arr->arr));
    }
    arr->arr[arr->size] = *client;
    arr->size += 1;
    return serv;
}

static void close_server(server_t *serv)
{
    if (serv == NULL)
        return;
    for (size_t i = 0; i < serv->clients.size; i++)
        if (serv->socket.fd != -1)
            client_disconnect(&serv->clients.arr[i]);
    close(serv->socket.fd);
    free(serv->clients.arr);
}

static client_t *new_client(server_t *serv, client_t *client)
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
    return client_init(client, serv);
}

int myftp(int argc, char **argv)
{
    server_t serv = {0};

    args_get(argv, argc, &serv);
    serv.socket.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (open_serv(&serv) == RET_ERROR)
        return RET_ERROR;
    DEBUG("server online.., fd: %d", serv.socket.fd);
    for (client_t client; true;) {
        handle_clients(&serv);
        if (new_client(&serv, &client) == NULL)
            continue;
        add_client(&serv, &client);
        DEBUG("new: %s:%u", GETIP(client), client.socket.sock_in.sin_port);
    }
    close_server(&serv);
    return RET_VALID;
}
