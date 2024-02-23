/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** active
*/

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "client/client.h"
#include "codes.h"
#include "debug.h"
#include "messages.h"

static bool parse_port(const char *buffer, size_t size, char *arr[size])
{
    static char tmp[BUFSIZ];
    char *ptr = tmp;

    if (buffer == NULL || arr == NULL)
        return false;
    strcpy(tmp, buffer);
    for (size_t i = 0; i < size; i++) {
        arr[i] = strtok_r(ptr, ",", &ptr);
        DEBUG(
            "arr[%ld]=%p; ptr=%p; %s: %d\n", i, arr[i], ptr, arr[i],
            atoi(arr[i]));
        if (arr[i] == NULL)
            return false;
    }
    return true;
}

static bool set_sock_addr(client_t *client, const char *buff)
{
    char *arr[6] = {0};

    if (!parse_port(buff, LEN_OF(arr), arr))
        return false;
    client->data_socket.sock_in.sin_family = AF_INET;
    errno = 0;
    client->data_socket.sock_in.sin_addr.s_addr = ntohl(
        (atoi(arr[0]) << 24) | (atoi(arr[1]) << 16) | (atoi(arr[2]) << 8) |
        atoi(arr[3]));
    client->data_socket.sock_in.sin_port =
        ntohs(atoi(arr[4]) << 8 | atoi(arr[5]));
    DEBUG(
        "active mode: %d.%d.%d.%d: port: %d\n", atoi(arr[0]), atoi(arr[1]),
        atoi(arr[2]), atoi(arr[3]), client->data_socket.sock_in.sin_port);
    return (errno == 0);
}

void msg_port(client_t *client, const char *buffer)
{
    if (client == NULL || buffer == NULL || !client_logged(client))
        return;
    client->state = NO_DATA_SOCK;
    if (!set_sock_addr(client, buffer)) {
        client_write(client, MSG_501);
        return;
    }
    client->state = ACTIVE_MODE;
    client_write(client, MSG_200);
}
