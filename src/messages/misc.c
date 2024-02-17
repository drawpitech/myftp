/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** misc
*/

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
