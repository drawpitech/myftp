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

#include "client/client.h"
#include "debug.h"
#include "messages.h"
#include "messages/codes.h"

static char *clear_msg(const struct msg_s *msg, char *buffer)
{
    size_t len = 0;

    if (msg == NULL || msg->cmd == NULL || buffer == NULL)
        return buffer;
    buffer += strlen(msg->cmd);
    if (buffer[0] == ' ')
        buffer += 1;
    len = strlen(buffer);
    if (len >= 1 && buffer[len - 1] == '\n') {
        len -= 1;
        buffer[len] = '\0';
        if (len >= 1 && buffer[len - 1] == '\r')
            buffer[len - 1] = '\0';
    }
    return buffer;
}

static int compare(const char *buffer, const struct msg_s *msg)
{
    return (buffer == NULL || msg == NULL || msg->cmd == NULL)
        ? 1
        : strncmp(buffer, msg->cmd, strlen(msg->cmd));
}

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

void client_process_message(client_t *client, char *buffer)
{
    const struct msg_s *msg = NULL;

    if (client == NULL || buffer == NULL)
        return;
    DEBUG("client said: %s", buffer);
    msg = bsearch(
        buffer, INCOMMING_MSG, LEN_OF(INCOMMING_MSG), sizeof(*INCOMMING_MSG),
        (int (*)(const void *, const void *))compare);
    if (msg == NULL) {
        client_write(client, MSG_500);
        return;
    }
    if (msg->func == NULL) {
        client_write(client, MSG_502);
        return;
    }
    msg->func(client, clear_msg(msg, buffer));
}
