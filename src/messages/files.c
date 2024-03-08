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
    char res[PATH_MAX] = {0};

    if (client == NULL || buffer == NULL || !client_logged(client))
        return;
    if (get_path(client->path, buffer, file_path) == NULL ||
        realpath(file_path, res) == NULL || access(res, F_OK) != 0 ||
        remove(res) != 0) {
        client_write(client, MSG_550);
        return;
    }
    client_write(client, MSG_250);
}

static void list_files(int fd, client_t *client, const char *path)
{
    static const char cmd[] = "ls -l ";
    char ls_cmd[PATH_MAX + LEN_OF(cmd)];
    FILE *ls = NULL;
    char buf[BUFSIZ];

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
        client_fd_write(fd, client, buf, strlen(buf));
    client_write(client, MSG_226);
    pclose(ls);
}

void msg_list(client_t *client, const char *buffer)
{
    char path[PATH_MAX];
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
    char buff[BUFSIZ];
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
    char path[PATH_MAX];

    if (client == NULL || buffer == NULL || !client_logged(client))
        return;
    if (get_path(client->path, buffer, path) == NULL) {
        client_write(client, MSG_450);
        return;
    }
    fork_data_sock(client, path, retrieve_file);
}

static void give_me_da_file(int file, int fd)
{
    char buff[BUFSIZ];
    struct timeval tv = {0};
    fd_set readfds;

    for (ssize_t size = LEN_OF(buff); size > 0;) {
        tv = (struct timeval){.tv_sec = 10, .tv_usec = 0};
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        if (select(fd + 1, &readfds, NULL, NULL, &tv) <= 0) {
            DEBUG_MSG("Envie de crever");
            break;
        }
        size = read(fd, buff, size);
        write(file, buff, size);
        DEBUG("size: %ld", size);
    }
}

static void upload_file(int fd, client_t *client, const char *filename)
{
    int file = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (file == -1) {
        client_write(client, MSG_450);
        return;
    }
    client_write(client, MSG_150);
    give_me_da_file(file, fd);
    client_write(client, MSG_226);
    close(file);
}

void msg_stor(client_t *client, const char *buffer)
{
    char path[PATH_MAX];

    if (client == NULL || buffer == NULL || !client_logged(client))
        return;
    if (get_path(client->path, buffer, path) == NULL) {
        client_write(client, MSG_450);
        return;
    }
    fork_data_sock(client, path, upload_file);
}
