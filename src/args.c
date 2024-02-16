/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** args
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "myftp.h"

static void show_help(void)
{
    printf(
        "USAGE: ./myftp port path\n"
        "\tport is the port number on which the server socket listens\n"
        "\tpath is the path to the home directory for the Anonymous user\n");
}

void args_get(char **argv, int argc, server_t *serv)
{
    if (serv == NULL || argv == NULL || argc <= 0)
        exit(RET_ERROR);
    if (argc == 3) {
        serv->socket.sock_in.sin_port = htons(atoi(argv[1]));
        serv->path = argv[2];
        return;
    }
    show_help();
    exit((argc == 2 && strcmp(argv[1], "-help") == 0) ? RET_VALID : RET_ERROR);
}
