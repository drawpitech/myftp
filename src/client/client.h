/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** client
*/

#pragma once

#include "myftp.h"

void client_disconnect(client_t *client);
void client_handle(client_t *client);
client_t *client_init(client_t *client, server_t *serv);
ATTR(format(printf, 2, 3))
int client_data_write(client_t *client, const char *fmt, ...);
ATTR(format(printf, 2, 3))
int client_write(client_t *client, const char *fmt, ...);
