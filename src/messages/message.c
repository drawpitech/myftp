/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** message
*/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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
    if (len >= 2 && strcmp(buffer + len - 2, "\r\n") == 0)
        buffer[len - 2] = '\0';
    return buffer;
}

static int compare(const char *buffer, const struct msg_s *msg)
{
    return (buffer == NULL || msg == NULL || msg->cmd == NULL)
        ? 1
        : strncmp(buffer, msg->cmd, strlen(msg->cmd));
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
