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

void msg_cdup(client_t *client, const char *buffer);
void msg_cwd(client_t *client, const char *buffer);
void msg_dele(client_t *client, const char *buffer);
void msg_help(client_t *client, const char *buffer);
void msg_list(client_t *client, const char *buffer);
void msg_mkd(client_t *client, const char *buffer);
void msg_noop(client_t *client, const char *buffer);
void msg_pass(client_t *client, const char *buffer);
void msg_pasv(client_t *client, const char *buffer);
void msg_port(client_t *client, const char *buffer);
void msg_pwd(client_t *client, const char *buffer);
void msg_quit(client_t *client, const char *buffer);
void msg_retr(client_t *client, const char *buffer);
void msg_rmd(client_t *client, const char *buffer);
void msg_syst(client_t *client, const char *buffer);
void msg_type(client_t *client, const char *buffer);
void msg_user(client_t *client, const char *buffer);
void msg_xpwd(client_t *client, const char *buffer);

static const struct msg_s {
    char *cmd;
    void (*func)(client_t *client, const char *buffer);
    char *help;
} INCOMMING_MSG[] = {
    {"ABOR", NULL, NULL},
    {"ACCT", NULL, NULL},
    {"ALLO", NULL, NULL},
    {"APPE", NULL, NULL},
    {"CDUP", msg_cdup, "Change working directory to parent directory"},
    {"CWD", msg_cwd, "Change working directory"},
    {"DELE", msg_dele, "Delete file on the server"},
    {"HELP", msg_help, "List available commands"},
    {"LIST", msg_list, "List files in the current working directory"},
    {"LPRT", NULL, NULL},
    {"LPSV", NULL, NULL},
    {"MKD", msg_mkd, "Make directory"},
    {"MODE", NULL, NULL},
    {"NLST", NULL, NULL},
    {"NOOP", msg_noop, "Do nothing"},
    {"PASS", msg_pass, "Specify password for authentication"},
    {"PASV", msg_pasv, "Enable \"passive\" mode for data transfer"},
    {"PORT", msg_port, "Enable \"active\" mode for data transfer"},
    {"PWD", msg_pwd, "Print working directory"},
    {"QUIT", msg_quit, "Disconnection"},
    {"REIN", NULL, NULL},
    {"REST", NULL, NULL},
    {"RETR", msg_retr, "Download file from server to client"},
    {"RMD", msg_rmd, "Remove directory"},
    {"RNFR", NULL, NULL},
    {"RNTO", NULL, NULL},
    {"SITE", NULL, NULL},
    {"SMNT", NULL, NULL},
    {"STAT", NULL, NULL},
    {"STOR", NULL, "Upload file from client to server"},
    {"STOU", NULL, NULL},
    {"STRU", NULL, NULL},
    {"SYST", msg_syst, "Print system information"},
    {"TYPE", msg_type, "Set transfer type"},
    {"USER", msg_user, "Specify user for authentication"},
    {"XCUP", NULL, NULL},
    {"XMKD", NULL, NULL},
    {"XPWD", msg_xpwd, "Print working directory"},
    {"XRMD", NULL, NULL},
};
