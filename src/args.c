/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** args
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "myftp.h"

static void show_help(void)
{
    printf(
        "USAGE: ./myftp port path\n"
        "\tport is the port number on which the server socket listens\n"
        "\tpath is the path to the home directory for the Anonymous user\n");
}

static int get_port(const char *str)
{
    char *endptr = NULL;
    int port = 0;

    errno = 0;
    port = (int)strtol(str, &endptr, 10);
    if (errno != 0 || (endptr != NULL && *endptr != '\0')) {
        errno = ERANGE;
        exit(ret_error("strtol", RET_ERROR));
    }
    return port;
}

static char *get_path(const char *str)
{
    char *path = realpath(str, NULL);
    struct stat path_stat;

    if (path == NULL)
        exit(ret_error("realpath", RET_ERROR));
    if (lstat(str, &path_stat) != 0 || !S_ISDIR(path_stat.st_mode)) {
        errno = ENOTDIR;
        exit(ret_error("realpath", RET_ERROR));
    }
    return path;
}

void args_get(char **argv, int argc, server_t *serv)
{
    char *endptr = NULL;

    if (serv == NULL || argv == NULL || argc <= 0)
        exit(RET_ERROR);
    if (argc != 3) {
        show_help();
        exit((argc != 2 || strcmp(argv[1], "-help") != 0) * RET_ERROR);
    }
    serv->socket.sock_in.sin_port = htons(get_port(argv[1]));
    serv->path = get_path(argv[2]);
}
