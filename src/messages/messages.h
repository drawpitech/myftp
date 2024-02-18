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
bool client_logged(client_t *client);

void msg_user(client_t *client, const char *buffer);
void msg_pass(client_t *client, const char *buffer);
void msg_syst(client_t *client, const char *buffer);
void msg_quit(client_t *client, const char *buffer);
void msg_noop(client_t *client, const char *buffer);
void msg_cwd(client_t *client, const char *buffer);
void msg_cdup(client_t *client, const char *buffer);
void msg_pwd(client_t *client, const char *buffer);
void msg_xpwd(client_t *client, const char *buffer);
void msg_help(client_t *client, const char *buffer);

static const struct msg_s {
    char *cmd;
    void (*func)(client_t *client, const char *buffer);
    char *help;
} INCOMMING_MSG[] = {
    {"ACCT", NULL, NULL},
    {"USER", msg_user, "Specify user for authentication"},
    {"PASS", msg_pass, "Specify password for authentication"},
    {"CWD", msg_cwd, "Change working directory"},
    {"CDUP", msg_cdup, "Change working directory to parent directory"},
    {"SMNT", NULL, NULL},
    {"QUIT", msg_quit, "Disconnection"},
    {"REIN", NULL, NULL},
    {"PORT", NULL, "Enable \"active\" mode for data transfer"},
    {"PASV", NULL, "Enable \"passive\" mode for data transfer"},
    {"TYPE", NULL, NULL},
    {"STRU", NULL, NULL},
    {"MODE", NULL, NULL},
    {"RETR", NULL, "Download file from server to client"},
    {"STOR", NULL, "Upload file from client to server"},
    {"STOU", NULL, NULL},
    {"APPE", NULL, NULL},
    {"ALLO", NULL, NULL},
    {"REST", NULL, NULL},
    {"RNFR", NULL, NULL},
    {"RNTO", NULL, NULL},
    {"ABOR", NULL, NULL},
    {"DELE", NULL, "Delete file on the server"},
    {"RMD", NULL, NULL},
    {"MKD", NULL, NULL},
    {"PWD", msg_pwd, "Print working directory"},
    {"LIST", NULL, "List files in the current working directory"},
    {"NLST", NULL, NULL},
    {"SITE", NULL, NULL},
    {"SYST", msg_syst, "Print system information"},
    {"STAT", NULL, NULL},
    {"HELP", msg_help, "List available commands"},
    {"NOOP", msg_noop, "Do nothing"},
    {"LPRT", NULL, NULL},
    {"XMKD", NULL, NULL},
    {"XRMD", NULL, NULL},
    {"XPWD", msg_xpwd, "Print working directory"},
    {"XCUP", NULL, NULL},
};
