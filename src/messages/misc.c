/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** message
*/

#include "messages.h"

void msg_syst(client_t *client, UNUSED const char *buffer)
{
    if (client == NULL)
        return;
    client_write(client, "215 UNIX Type: L8\r\n");
}
