/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** utils
*/

#include <string.h>
#include <sys/stat.h>

#include "myftp.h"
#include "unistd.h"

char *get_path(const char *parent, const char *child, char result[PATH_MAX])
{
    if (parent == NULL || child == NULL || result == NULL)
        return NULL;
    if (child[0] == '/') {
        strcpy(result, child);
    } else {
        strcpy(result, parent);
        strcat(result, "/");
        strcat(result, child);
    }
    return result;
}

int ret_error(const char *name, int value)
{
    dprintf(STDERR_FILENO, "%s: ", __progname);
    perror(name);
    return value;
}
