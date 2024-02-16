/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** auth
*/

#include <string.h>

#include "messages.h"
#include "codes.h"

// Great security here
static const struct {
    char *username;
    char *password;
} USERS[] = {
    {"Anonymous", ""},
    {"admin", "admin"},
    {NULL, NULL},
};

void msg_user(client_t *client, const char *buffer)
{
    if (client == NULL || buffer == NULL)
        return;
    for (int i = 0; USERS[i].username != NULL; i++) {
        if (strcmp(USERS[i].username, buffer) != 0)
            continue;
        strcpy(client->username, buffer);
        if (strcmp(USERS[i].password, "") != 0) {
            client_write(client, MSG_331);
            return;
        }
        client_write(client, MSG_230);
        return;
    }
    client_write(client, MSG_530);
}

void msg_pass(client_t *client, const char *buffer)
{
    if (client == NULL || buffer == NULL)
        return;
    if (client->username[0] == '\0') {
        client_write(client, MSG_503);
        return;
    }
    for (int i = 0; USERS[i].username != NULL; i++) {
        if (strcmp(USERS[i].username, client->username) != 0)
            continue;
        if (strcmp(USERS[i].password, buffer) != 0) {
            client_write(client, MSG_530);
            return;
        }
        client_write(client, MSG_230);
        return;
    }
    client_write(client, MSG_530);
}
