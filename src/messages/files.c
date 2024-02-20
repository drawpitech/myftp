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

static void list_files(client_t *client, int fd, const char *path)
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
    for (; fgets(buf, BUFSIZ, ls) != NULL; memset(buf, 0, BUFSIZ)) {
        client_fd_write(fd, client, buf, BUFSIZ);
        write(1, buf, BUFSIZ);
    }
    client_write(client, MSG_226);
    pclose(ls);
}

void msg_list(client_t *client, const char *buffer)
{
    int fd = 0;
    static char path[PATH_MAX];
    struct stat path_stat;

    if (client == NULL || !client_logged(client))
        return;
    fd = client_get_data_sock(client);
    if (fd == -1) {
        client_write(client, MSG_425);
        return;
    }
    if (get_path(client->path, buffer, path) == NULL ||
        lstat(path, &path_stat) != 0 || !S_ISDIR(path_stat.st_mode)) {
        client_write(client, MSG_450);
        return;
    }
    list_files(client, fd, path);
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
        client_fd_write(fd, client, buff, size);
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
