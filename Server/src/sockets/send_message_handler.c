#include "server.h"

int mx_send_check(t_sock *sock, t_peer *client, int n) {
    if (n < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("Peer is not ready right now, try again later.\n");
        }
        else {
            perror("Send() from peer error");
            mx_handle_disconnect(sock, client);
        }
    }
    // we have read as many as possible
    // else if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    //     return -1;
    // }
    else if (n == 0) {
        printf("Peer can't accept data right now, try again later.\n");
        // return -1;
    }
    return 0;
}

static void send_one(t_sock *sock, t_peer *peer, int sd) {
    int n;

    n = send(sd, peer->send_buff, strlen(peer->send_buff), MSG_DONTWAIT);
    if (n <= 0) {
        mx_send_check(sock, peer, n);
    }
}

static cJSON *create_peice(int type, char *str) {
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddNumberToObject(obj, "p_type", type);
    cJSON_AddStringToObject(obj, "piece", str);

    return obj;
}

static void send_large(t_sock *sock, t_peer *p, int sd, char *large_message) {
    unsigned long i = 0;
    char str[MX_MAX_SEND_SIZE / 2];

    while (large_message[i]) {
        strncpy(str, &large_message[i], MX_MAX_SEND_SIZE / 2 - 1);
        i += strlen(str);
        if (i == strlen(large_message)) {
            mx_json_to_sending_buffer(p->send_buff,
                                      create_peice(big_msg_end, str));
            send_one(sock, p, sd);
            break;
        }
        else {
            mx_json_to_sending_buffer(p->send_buff,
                                      create_peice(big_msg, str));
            send_one(sock, p, sd);
        }
        sleep(1);
    }
}

void mx_send_message_handler(t_sock *sock, t_peer *peer, cJSON *bd, int sd) {
    char *root = cJSON_Print(bd);
    char *large_message;
    int len;

    mx_print_serv_out(bd, root);
    // printf("root size = %lu\n", strlen(root));        //// for test
    // printf("%s\n", root);                     //// for test
    if (MX_TYPE(bd) == file_msg)
        mx_send_file(sock, peer, bd, sd);
    else {
        if (strlen(root) < 2 * MX_MAX_SEND_SIZE / 3) {
            mx_json_to_sending_buffer(peer->send_buff, create_peice(one_msg, root));
            send_one(sock, peer, sd);
        }
        else {
            large_message = strdup(root);
            len = strlen(large_message);
            // printf("message = %s\n", large_message);      //// for test
            send_large(sock, peer, sd, large_message);
            mx_strdel(&large_message);
        }
    }
    bzero(peer->send_buff, sizeof(peer->send_buff));
}
