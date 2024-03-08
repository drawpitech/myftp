/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** message
*/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../messages.h"
#include "client/client.h"
#include "messages/codes.h"

static void data_fork_child(
    client_t *client, const char *buffer,
    void (*func)(int, client_t *, const char *))
{
    int fd = client_get_data_sock(client);

    if (fd == -1) {
        client_write(client, MSG_425);
        return;
    }
    func(fd, client, buffer);
    close(fd);
    client_close_data_sock(client);
    exit(0);
}

void fork_data_sock(
    client_t *client, const char *buffer,
    void (*func)(int, client_t *, const char *))
{
    if (client == NULL || buffer == NULL || func == NULL)
        return;
    if (client->state == NO_DATA_SOCK) {
        client_write(client, MSG_425);
        return;
    }
    switch (fork()) {
        case -1:
            client_write(client, MSG_425);
            break;
        case 0:
            data_fork_child(client, buffer, func);
            break;
        default:
            break;
    }
}
