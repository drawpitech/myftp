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
void msg_noop(client_t *client, const char *buffer);

static const struct msg_s {
    char *cmd;
    void (*func)(client_t *client, const char *buffer);
} INCOMMING_MSG[] = {
    {"USER", msg_user},
    {"PASS", msg_pass},
    {"ACCT", NULL},
    {"CWD", NULL},
    {"USER", NULL},
    {"PASS", NULL},
    {"ACCT", NULL},
    {"CWD ", NULL},
    {"CDUP", NULL},
    {"SMNT", NULL},
    {"QUIT", msg_quit},
    {"REIN", NULL},
    {"PORT", NULL},
    {"PASV", NULL},
    {"TYPE", NULL},
    {"STRU", NULL},
    {"MODE", NULL},
    {"RETR", NULL},
    {"STOR", NULL},
    {"STOU", NULL},
    {"APPE", NULL},
    {"ALLO", NULL},
    {"REST", NULL},
    {"RNFR", NULL},
    {"RNTO", NULL},
    {"ABOR", NULL},
    {"DELE", NULL},
    {"RMD ", NULL},
    {"MKD ", NULL},
    {"PWD ", NULL},
    {"LIST", NULL},
    {"NLST", NULL},
    {"SITE", NULL},
    {"SYST", msg_syst},
    {"STAT", NULL},
    {"HELP", NULL},
    {"NOOP", msg_noop},
    {"LPRT", NULL},
    {NULL, NULL},
};
