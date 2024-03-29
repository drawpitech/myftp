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

static const char HELP_MSG[] = {
    "USAGE: %s port path\n"
    "\tport is the port number on which the server socket listens\n"
    "\tpath is the path to the home directory for the Anonymous user\n"};

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

static void set_path(const char *str, server_t *serv)
{
    struct stat path_stat;
    char buf[PATH_MAX] = {0};

    if (get_path(".", str, buf) == NULL) {
        errno = ENOENT;
        exit(ret_error("get_path", RET_ERROR));
    }
    if (realpath(buf, serv->path) == NULL)
        exit(ret_error("realpath", RET_ERROR));
    if (lstat(serv->path, &path_stat) != 0 || !S_ISDIR(path_stat.st_mode)) {
        errno = ENOTDIR;
        exit(ret_error("lstat", RET_ERROR));
    }
}

void args_get(char **argv, int argc, server_t *serv)
{
    if (serv == NULL || argv == NULL || argc <= 0)
        exit(RET_ERROR);
    if (argc != 3) {
        printf(HELP_MSG, argv[0]);
        exit((argc != 2 || strcmp(argv[1], "-help") != 0) * RET_ERROR);
    }
    serv->socket.sock_in.sin_port = htons(get_port(argv[1]));
    set_path(argv[2], serv);
}
