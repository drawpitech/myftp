/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** messages
*/

#pragma once

#include <stdlib.h>

#include "myftp.h"

void client_process_message(client_t *client, char *buffer);

void msg_user(client_t *client, const char *buffer);
void msg_pass(client_t *client, const char *buffer);
void msg_syst(client_t *client, const char *buffer);
void msg_quit(client_t *client, const char *buffer);

static const struct msg_s {
    void (*func)(client_t *client, const char *buffer);
    char *cmd;
} INCOMMING_MSG[] = {
    {msg_user, "USER"},
    {msg_pass, "PASS"},
    {msg_syst, "SYST"},
    {msg_quit, "QUIT"},
    {NULL, NULL},
};
