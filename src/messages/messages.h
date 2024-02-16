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

static const struct msg_s {
    void (*func)(client_t *client, const char *buffer);
    char *cmd;
    state_t state;
} INCOMMING_MSG[] = {
    {msg_user, "USER", ST_WAITING_USERNAME},
    {msg_pass, "PASS", ST_WAITING_PASSWORD},
    {msg_syst, "SYST", ST_NIL},
    {NULL, NULL, ST_NIL},
};
