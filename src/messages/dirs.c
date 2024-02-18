/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** misc
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "messages.h"
#include "messages/codes.h"
#include "myftp.h"

void msg_mkd(client_t *client, const char *buffer)
{
    mode_t mode =
        S_IWUSR | S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
    char dir_path[PATH_MAX] = {0};

    if (client == NULL || buffer == NULL || !client_logged(client))
        return;
    if (get_path(client->path, buffer, dir_path) == NULL ||
        mkdir(dir_path, mode) == -1) {
        client_write(client, MSG_550);
        return;
    }
    client_write(client, MSG_257, buffer);
}
