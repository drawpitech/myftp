/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** type
*/

#include <string.h>

#include "myftp.h"
#include "client/client.h"
#include "messages.h"
#include "messages/codes.h"

static const struct {
    char *str;
    data_type_t type;
} STR_TO_TYPE[] = {
    {"I", DT_IMAGE},
    {"A", DT_ASCII},
    {"L", DT_LOCAL},
};

void msg_type(client_t *client, const char *buffer)
{
    if (client == NULL || buffer == NULL || !client_logged(client))
        return;
    for (size_t i = 0; i < LEN_OF(STR_TO_TYPE); i++) {
        if (strcmp(buffer, STR_TO_TYPE[i].str) != 0)
            continue;
        client->data_type = STR_TO_TYPE[i].type;
        client_write(client, MSG_200);
        return;
    }
    client_write(client, MSG_500);
}
