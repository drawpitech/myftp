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
        client_write(client, MSG_550);
        return;
    }
    client_write(client, MSG_250);
}

static void list_files(int fd, client_t *client, const char *path)
{
    static const char cmd[] = "ls -l ";
    static char ls_cmd[PATH_MAX + LEN_OF(cmd)];
    FILE *ls = NULL;
    static char buf[BUFSIZ];

    strcpy(ls_cmd, cmd);
    strcat(ls_cmd, path);
    ls = popen(ls_cmd, "r");
    if (ls == NULL) {
        client_write(client, MSG_450);
        return;
    }
    client_write(client, MSG_150);
    memset(buf, 0, BUFSIZ);
    for (; fgets(buf, BUFSIZ, ls) != NULL; memset(buf, 0, BUFSIZ))
        client_fd_write(fd, client, buf, BUFSIZ);
    client_write(client, MSG_226);
    pclose(ls);
}

void msg_list(client_t *client, const char *buffer)
{
    static char path[PATH_MAX];
    struct stat path_stat;

    if (client == NULL || !client_logged(client))
        return;
    if (get_path(client->path, buffer, path) == NULL ||
        lstat(path, &path_stat) != 0 || !S_ISDIR(path_stat.st_mode)) {
        client_write(client, MSG_450);
        return;
    }
    fork_data_sock(client, path, list_files);
}

static void retrieve_file(int fd, client_t *client, const char *filename)
{
    static char buff[BUFSIZ];
    int file = open(filename, O_RDONLY);

    if (file == -1) {
        client_write(client, MSG_450);
        return;
    }
    client_write(client, MSG_150);
    for (ssize_t size = sizeof(buff); size == sizeof(buff);) {
        size = read(file, buff, sizeof(buff));
        client_fd_write(fd, client, buff, size);
    }
    client_write(client, MSG_226);
    close(file);
}

void msg_retr(client_t *client, const char *buffer)
{
    static char path[PATH_MAX];

    if (client == NULL || buffer == NULL || !client_logged(client))
        return;
    if (get_path(client->path, buffer, path) == NULL) {
        client_write(client, MSG_450);
        return;
    }
    fork_data_sock(client, path, retrieve_file);
}

static void upload_file(int fd, client_t *client, const char *filename)
{
    static char buff[BUFSIZ];
    int file = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);

    if (file == -1) {
        client_write(client, MSG_450);
        return;
    }
    client_write(client, MSG_150);
    for (ssize_t size = sizeof(buff); size == sizeof(buff);) {
        size = read(fd, buff, sizeof(buff));
        write(file, buff, size);
    }
    client_write(client, MSG_226);
    close(file);
}

void msg_stor(client_t *client, const char *buffer)
{
    static char path[PATH_MAX];

    if (client == NULL || buffer == NULL || !client_logged(client))
        return;
    if (get_path(client->path, buffer, path) == NULL) {
        client_write(client, MSG_450);
        return;
    }
    fork_data_sock(client, buffer, upload_file);
}
