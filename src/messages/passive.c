/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** passive
*/

#include <string.h>
#include <unistd.h>

#include "client/client.h"
#include "codes.h"
#include "debug.h"
#include "messages.h"

static const unsigned short MIN_PORT = 49152;
static const unsigned short MAX_PORT = 65535;

static bool try_port(client_t *client, unsigned short port)
{
    socket_t *sock = &client->data_socket;

    DEBUG("try port: %d", port);
    sock->sock_in = client->socket.sock_in;
    sock->sock_in.sin_port = htons(port);
    if (bind(
            sock->fd, (struct sockaddr *)&sock->sock_in,
            sizeof(sock->sock_in)) == -1 ||
        listen(sock->fd, LISTEN_BACKLOG) != 0)
        return false;
    return true;
}

static unsigned short get_port(client_t *client)
{
    client->data_socket.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client->data_socket.fd == -1)
        return 0;
    for (unsigned short i = MIN_PORT; i != MIN_PORT + 3; i++)
        if (try_port(client, i))
            return i;
    close(client->data_socket.fd);
    return 0;
}

void msg_pasv(client_t *client, UNUSED const char *buffer)
{
    const int bits = 256;
    unsigned short port = 0;

    if (client == NULL || !client_logged(client))
        return;
    client->state = NO_DATA_SOCK;
    port = get_port(client);
    if (port == 0) {
        client_write(client, MSG_550);
        return;
    }
    DEBUG("passive mode: port: %d\n", port);
    client->state = PASSIVE_MODE;
    client_write(client, MSG_227, 127, 0, 0, 1, port / bits, port % bits);
}
