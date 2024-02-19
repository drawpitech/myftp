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

static void list_files(client_t *client, int fd)
{
    DIR *dir = NULL;

    dir = opendir(client->path);
    if (dir == NULL) {
        client_write(client, MSG_450);
        return;
    }
    client_write(client, MSG_125);
    for (struct dirent *file = readdir(dir); file != NULL; file = readdir(dir))
        if (file->d_name[0] != '.')
            client_fd_write(fd, client, "%s\r\n", file->d_name);
    client_write(client, MSG_226);
    closedir(dir);
    close(fd);
}

void msg_list(client_t *client, const char *buffer)
{
    int fd = 0;

    if (client == NULL || buffer == NULL || !client_logged(client))
        return;
    fd = client_get_data_sock(client);
    if (fd == -1) {
        client_write(client, MSG_425);
        return;
    }
    list_files(client, fd);
    client_close_data_sock(client);
}
