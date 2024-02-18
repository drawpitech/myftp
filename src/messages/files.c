/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** files
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "messages.h"
#include "messages/codes.h"
#include "myftp.h"

void msg_dele(client_t *client, const char *buffer)
{
    char file_path[PATH_MAX] = {0};

    if (client == NULL || buffer == NULL || !client_logged(client))
        return;
    if (get_path(client->path, buffer, file_path) == NULL ||
        unlink(file_path) == -1) {
        client_write(client, MSG_450);
        return;
    }
    client_write(client, MSG_250);
}
