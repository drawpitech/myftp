/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** passive
*/

#include <string.h>
#include <unistd.h>

#include "codes.h"
#include "debug.h"
#include "messages.h"

static const int MIN_PORT = 49152;
static const int MAX_PORT = 65535;

static int get_port(socket_t *sock)
{
    sock->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock->fd == -1)
        return -1;
    sock->sock_in.sin_family = AF_INET;
    sock->sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
    for (int i = MIN_PORT; i <= MAX_PORT; i++) {
        sock->sock_in.sin_port = htons(i);
        if (bind(
                sock->fd, (struct sockaddr *)&sock->sock_in,
                sizeof(sock->sock_in)) == -1 ||
            listen(sock->fd, LISTEN_BACKLOG) != 0)
            continue;
        DEBUG("passive mode: port: %d\n", i);
        return i;
    }
    close(sock->fd);
    memset(sock, 0, sizeof(*sock));
    return -1;
}

void msg_pasv(client_t *client, UNUSED const char *buffer)
{
    const int bits = 256;
    int port = 0;

    if (client == NULL || !client_logged(client))
        return;
    memset(&client->data_socket, 0, sizeof(client->data_socket));
    client->state = NO_DATA_SOCK;
    port = get_port(&client->data_socket);
    if (port == -1) {
        client_write(client, MSG_550);
        return;
    }
    client->state = PASSIVE_MODE;
    client_write(client, MSG_227, 127, 0, 0, 1, port / bits, port % bits);
}
