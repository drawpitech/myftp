/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** path
*/

#include <string.h>
#include <sys/stat.h>

#include "messages.h"
#include "client/client.h"
#include "messages/codes.h"
#include "myftp.h"

void msg_cwd(client_t *client, const char *buffer)
{
    struct stat path_stat;
    char path[PATH_MAX] = {0};
    char res[PATH_MAX] = {0};

    if (client == NULL || buffer == NULL || !client_logged(client))
        return;
    if (strcmp(buffer, "") == 0) {
        client_write(client, MSG_501);
        return;
    }
    get_path(client->path, buffer, path);
    if (realpath(path, res) == NULL || lstat(res, &path_stat) != 0 ||
        !S_ISDIR(path_stat.st_mode)) {
        client_write(client, MSG_550);
        return;
    }
    strcpy(client->path, res);
    client_write(client, MSG_250);
}

void msg_cdup(client_t *client, UNUSED const char *buffer)
{
    msg_cwd(client, "..");
}

void msg_pwd(client_t *client, UNUSED const char *buffer)
{
    if (client == NULL || !client_logged(client))
        return;
    client_write(client, MSG_257_CD, client->path);
}

void msg_xpwd(client_t *client, UNUSED const char *buffer)
{
    msg_pwd(client, buffer);
}
