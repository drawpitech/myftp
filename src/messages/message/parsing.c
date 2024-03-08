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
    if (msg == NULL || msg->cmd == NULL || buffer == NULL)
        return buffer;
    buffer += strlen(msg->cmd);
    if (buffer[0] == ' ')
        buffer += 1;
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

static void exec_command(client_t *client)
{
    const struct msg_s *msg = get_message(client->buffer);

    if (msg == NULL) {
        client_write(client, MSG_500);
        return;
    }
    if (msg->func == NULL) {
        client_write(client, MSG_502);
        return;
    }
    msg->func(client, clear_msg(msg, client->buffer));
}

void client_process_message(client_t *client, char *buffer)
{
    char *ptr = NULL;

    if (client == NULL || buffer == NULL || buffer[0] == '\0')
        return;
    DEBUG("client said: %s", buffer);
    ptr = strchr(buffer, '\n');
    if (ptr == NULL) {
        strcat(client->buffer, buffer);
        return;
    }
    *ptr = '\0';
    if (buffer[0] != '\0' && *(ptr - 1) == '\r')
        *(ptr - 1) = '\0';
    strcat(client->buffer, buffer);
    DEBUG("buf: %s\n", client->buffer);
    exec_command(client);
    client->buffer[0] = '\0';
    client_process_message(client, ptr + 1);
}
