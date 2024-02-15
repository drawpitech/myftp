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

static int ret_error(const char *name, int value)
{
    dprintf(STDERR_FILENO, "myftp: ");
    perror(name);
    return value;
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
    serv->socket.sock_in.sin_port = htons(PORT);
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

static server_t *add_elem_to_array(server_t *serv, client_t *client)
{
    struct clients_s *arr = NULL;

    if (serv == NULL)
        return NULL;
    arr = &serv->clients;
    if (arr->size + 1 > arr->alloc) {
        arr->alloc = (arr->alloc) ? arr->alloc * arr->alloc : 2;
        arr->arr = reallocarray(arr->arr, arr->alloc, sizeof(*arr->arr));
    }
    arr->arr[arr->size] = *client;
    arr->size += 1;
    return serv;
}

static void disconnect(client_t *client)
{
    if (client == NULL)
        return;
    close(client->socket.fd);
    client->socket.fd = -1;
    printf(
        "client disconnected: %s:%u\n", GETIP(*client),
        client->socket.sock_in.sin_port);
}

static bool is_client_trying_to_talk(client_t *client)
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

static void please_answer(client_t *client, char *buffer)
{
    if (client == NULL || buffer == NULL)
        return;
    printf("client said: \"%s\"\n", buffer);
    dprintf(client->socket.fd, "you said %s", buffer);
}

static void client_desk(client_t *client)
{
    static char buffer[BUFSIZ + 1];
    ssize_t size = 0;

    if (client == NULL || !is_client_trying_to_talk(client))
        return;
    size = read(client->socket.fd, buffer, BUFSIZ);
    if (size == 0 || size == -1) {
        disconnect(client);
        return;
    }
    buffer[size] = 0;
    please_answer(client, buffer);
}

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
        client_desk(client);
    }
}

static client_t *is_there_someone_at_the_door(int sock)
{
    struct timeval tv = TV_CONNECT;
    static client_t cli;
    fd_set fdread;
    fd_set fdwrite;
    socklen_t len = sizeof(cli.socket);

    FD_ZERO(&fdread);
    FD_ZERO(&fdwrite);
    FD_SET(sock, &fdread);
    FD_SET(sock, &fdwrite);
    memset(&cli, 0, sizeof(cli));
    if (select(sock + 1, &fdread, &fdwrite, NULL, &tv) <= 0)
        return NULL;
    if (!FD_ISSET(sock, &fdread))
        return NULL;
    cli.socket.fd = accept(sock, (struct sockaddr *)&cli.socket, &len);
    return (cli.socket.fd == -1) ? NULL : &cli;
}

int myftp(int argc, char **argv)
{
    server_t serv = {0};

    serv.socket.fd = socket(AF_INET, SOCK_STREAM, 0);
    serv.socket.fd = open_serv(&serv);
    if (serv.socket.fd == -1)
        return RET_ERROR;
    printf("server online..\n");
    for (client_t *client = NULL; true; client = NULL) {
        client = is_there_someone_at_the_door(serv.socket.fd);
        if (client != NULL) {
            add_elem_to_array(&serv, client);
            printf(
                "client connected: %s:%u\n", GETIP(*client),
                client->socket.sock_in.sin_port);
        }
        handle_clients(&serv);
    }
    close(serv.socket.fd);
    return RET_VALID;
}
