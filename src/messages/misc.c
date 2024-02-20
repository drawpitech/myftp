/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** misc
*/

#include <string.h>

#include "client/client.h"
#include "messages.h"
#include "messages/codes.h"

void msg_syst(client_t *client, UNUSED const char *buffer)
{
    if (client == NULL)
        return;
    client_write(client, MSG_215, "UNIX");
}

void msg_quit(client_t *client, UNUSED const char *buffer)
{
    if (client == NULL)
        return;
    client_write(client, MSG_221);
    client_disconnect(client);
}

void msg_noop(client_t *client, UNUSED const char *buffer)
{
    if (client == NULL)
        return;
    client_write(client, MSG_200);
}

void msg_help(client_t *client, UNUSED const char *buffer)
{
    const struct msg_s *msg = NULL;

    if (client == NULL)
        return;
    for (size_t i = 0; i < LEN_OF(INCOMMING_MSG); i++) {
        msg = &INCOMMING_MSG[i];
        if (strcmp(buffer, msg->cmd) != 0)
            continue;
        client_write(
            client, MSG_214, (msg->help != NULL) ? msg->help : "(null)");
        return;
    }
    client_write(client, MSG_500);
}
