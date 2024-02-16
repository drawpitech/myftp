/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** message
*/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "messages.h"

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

void client_process_message(client_t *client, char *buffer)
{
    const struct msg_s *msg = NULL;

    if (client == NULL || buffer == NULL)
        return;
    printf("client said: %s", buffer);
    for (size_t i = 0; INCOMMING_MSG[i].func != NULL; i++) {
        msg = &INCOMMING_MSG[i];
        if (strncmp(buffer, msg->cmd, strlen(msg->cmd)) != 0)
            continue;
        msg->func(client, clear_msg(msg, buffer));
        return;
    }
    client_write(client, "500 Syntax error, command unrecognized.\r\n");
}
