#include "server.h"

void mx_handle_disconnect(t_sock *sock, t_peer *client) {
    cJSON *json;

    getpeername(client->socket, (struct sockaddr *)&sock->address,
                (socklen_t *)&sock->addrlen);
    printf("Host disconnected, ip %s:%d\n" ,
            inet_ntoa(sock->address.sin_addr), ntohs(sock->address.sin_port));
    json = mx_this_uid_login_or_logout(client->uid, this_uid_logout);
    mx_send_message_all(sock, client, json, client->uid);

    // Close the socket and mark as MX_NO_SOCKET in list for reuse
    close(client->socket);
    client->socket = MX_NO_SOCKET;
    client->uid = -1;
    if (client->large_message)
        mx_strdel(&client->large_message);
    client->m_type = 0;
    bzero(client->send_buff, MX_MAX_SEND_SIZE);
    bzero(client->recv_buff, MX_MAX_SEND_SIZE);
}

void mx_handle_incoming_data(t_info *info) {
    int sd = 0;
    t_sock *s = info->sock;

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        sd = info->sock->connection_list[i].socket;
        if (sd != MX_NO_SOCKET && FD_ISSET(sd, &s->readfds)) {
            if (mx_receive_from_peer(info, &s->connection_list[i]) != 0)
                mx_handle_disconnect(s, &s->connection_list[i]);
            else
                mx_receive_message_handler(info, &s->connection_list[i]);
        }
    }
}
