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
#include "debug.h"
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

struct msg_s *get_message(const char *cmd)
{
    return bsearch(
        cmd, INCOMMING_MSG, LEN_OF(INCOMMING_MSG), sizeof(*INCOMMING_MSG),
        (int (*)(const void *, const void *))compare);
}

void client_process_message(client_t *client, char *buffer)
{
    const struct msg_s *msg = NULL;

    if (client == NULL || buffer == NULL)
        return;
    DEBUG("client said: %s", buffer);
    msg = get_message(buffer);
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
