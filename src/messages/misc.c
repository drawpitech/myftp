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

static const char *desc(const struct msg_s *msg)
{
    static char buf[BUFSIZ];

    if (msg == NULL || msg->help == NULL)
        return "No description available";
    strcpy(buf, msg->cmd);
    strcat(buf, " - ");
    strcat(buf, msg->help);
    return buf;
}

void msg_help(client_t *client, const char *buffer)
{
    const struct msg_s *msg = NULL;

    if (client == NULL)
        return;
    if (buffer[0] != '\0') {
        client_write(client, MSG_214, desc(get_message(buffer)));
        return;
    }
    for (size_t i = 0; i < LEN_OF(INCOMMING_MSG); i++) {
        msg = &INCOMMING_MSG[i];
        if (msg->func != NULL)
            client_write(client, MSG_214, desc(msg));
    }
}
