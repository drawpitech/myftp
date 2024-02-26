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

static bool get_port(client_t *client)
{
    socket_t *sock = &client->data_socket;
    socklen_t len = sizeof(sock->sock_in);

    client->data_socket.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client->data_socket.fd == -1)
        return 0;
    sock->sock_in = client->socket.sock_in;
    sock->sock_in.sin_port = 0;
    if (bind(
            sock->fd, (struct sockaddr *)&sock->sock_in,
            sizeof(sock->sock_in)) == 0 &&
        listen(sock->fd, LISTEN_BACKLOG) == 0 &&
        getsockname(sock->fd, (struct sockaddr *)&sock->sock_in, &len) == 0)
        return true;
    close(client->data_socket.fd);
    return false;
}

void msg_pasv(client_t *client, UNUSED const char *buffer)
{
    unsigned short port = 0;
    unsigned int raw_ip = 0;
    char *ip = (char *)&raw_ip;

    if (client == NULL || !client_logged(client))
        return;
    client->state = NO_DATA_SOCK;
    if (!get_port(client)) {
        client_write(client, MSG_550);
        return;
    }
    port = ntohs(client->data_socket.sock_in.sin_port);
    DEBUG("passive mode: port: %d\n", port);
    client->state = PASSIVE_MODE;
    raw_ip = htonl(client->socket.sock_in.sin_addr.s_addr);
    client_write(
        client, MSG_227, ip[3], ip[2], ip[1], ip[0], port >> 8,
        port - (port >> 8 << 8));
}
