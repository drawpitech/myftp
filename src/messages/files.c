/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** files
*/

#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "client/client.h"
#include "debug.h"
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

static void list_files(client_t *client, const char *path)
{
    DIR *dir = NULL;

    dir = opendir(path);
    if (dir == NULL) {
        client_write(client, MSG_450);
        return;
    }
    client_write(client, MSG_125);
    client_write(client, MSG_150);
    for (struct dirent *file = readdir(dir); file != NULL; file = readdir(dir))
        if (file->d_name[0] != '.')
            client_data_write(client, "%s\r\n", file->d_name);
    client_write(client, MSG_250);
    DEBUG("fd: %d\n", client->data_socket.fd);
    closedir(dir);
    close(client->data_socket.fd);
    memset(&client->data_socket, 0, sizeof(client->data_socket));
    client->state = NO_DATA_SOCK;
}

void msg_list(client_t *client, const char *buffer)
{
    char path[PATH_MAX] = {0};

    if (client == NULL || buffer == NULL || !client_logged(client))
        return;
    if (get_path(client->path, buffer, path) == NULL) {
        client_write(client, MSG_450);
        return;
    }
    list_files(client, path);
}
