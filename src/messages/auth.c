/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** auth
*/

#include <string.h>

#include "codes.h"
#include "messages.h"

// Great security here
static const struct {
    char *username;
    char *password;
} USERS[] = {
    {"Anonymous", ""},
    {"admin", "admin"},
};

void msg_user(client_t *client, const char *buffer)
{
    if (client == NULL || buffer == NULL)
        return;
    client->logged = false;
    for (size_t i = 0; i < LEN_OF(USERS); i++) {
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
    client->logged = false;
    if (client->username[0] == '\0') {
        client_write(client, MSG_503);
        return;
    }
    for (size_t i = 0; i < LEN_OF(USERS); i++) {
        if (strcmp(USERS[i].username, client->username) != 0)
            continue;
        if (strcmp(USERS[i].password, buffer) != 0) {
            client_write(client, MSG_530);
            return;
        }
        client->logged = true;
        client_write(client, MSG_230);
        return;
    }
    client_write(client, MSG_530);
}

bool client_logged(client_t *client)
{
    if (client == NULL)
        return false;
    if (client->logged)
        return true;
    client_write(client, MSG_530);
    return false;
}
