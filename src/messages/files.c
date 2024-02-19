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
#include "fcntl.h"
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
    client_write(client, MSG_150);
    for (struct dirent *file = readdir(dir); file != NULL; file = readdir(dir))
        if (file->d_name[0] != '.')
            client_fd_write(fd, client, "%s\r\n", file->d_name);
    client_write(client, MSG_226);
    closedir(dir);
}

void msg_list(client_t *client, UNUSED const char *buffer)
{
    int fd = 0;

    if (client == NULL || !client_logged(client))
        return;
    fd = client_get_data_sock(client);
    if (fd == -1) {
        client_write(client, MSG_425);
        return;
    }
    list_files(client, fd);
    close(fd);
    client_close_data_sock(client);
}

static void retrieve_file(client_t *client, const char *filename, int fd)
{
    ssize_t size = BUFSIZ;
    char buff[BUFSIZ + 1] = {0};
    int file = 0;

    file = open(filename, O_RDONLY);
    if (file == -1) {
        client_write(client, MSG_450);
        return;
    }
    client_write(client, MSG_150);
    while (size == BUFSIZ) {
        size = read(file, buff, BUFSIZ);
        client_fd_write(fd, client, "%.*s", (int)size, buff);
    }
    client_write(client, MSG_226);
    close(file);
}

void msg_retr(client_t *client, const char *buffer)
{
    int fd = 0;

    if (client == NULL || buffer == NULL || !client_logged(client))
        return;
    fd = client_get_data_sock(client);
    if (fd == -1) {
        client_write(client, MSG_425);
        return;
    }
    printf("buffer: %s\n", buffer);
    retrieve_file(client, buffer, fd);
    close(fd);
    client_close_data_sock(client);
}
