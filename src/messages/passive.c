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

static const int MIN_PORT = 49152;
static const int MAX_PORT = 65535;

static int get_port(socket_t *sock)
{
    socket_t s = {0};

    memset(sock, 0, sizeof(*sock));
    s.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s.fd == -1)
        return -1;
    s.sock_in.sin_family = AF_INET;
    s.sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
    for (int i = MIN_PORT; i <= MAX_PORT; i++) {
        s.sock_in.sin_port = htons(i);
        if (bind(s.fd, (struct sockaddr *)&s.sock_in, sizeof(s.sock_in)) == -1
            || listen(s.fd, LISTEN_BACKLOG) != 0)
            continue;
        *sock = s;
        DEBUG("passive mode: port: %d\n", i);
        return i;
    }
    close(s.fd);
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
    DEBUG("passive mode: port: %d\n", i);
    client->state = PASSIVE_MODE;
    client_write(client, MSG_227, 127, 0, 0, 1, port / bits, port % bits);
}
