#include "uchat.h"

/* Maximum bytes that can be send() or recv() via net by one call.
 * It's a good idea to test sending one byte by one.
 */
#define MX_MAX_SEND_SIZE 1024

/* Size of send queue (messages). */
#define MX_MAX_MESSAGES_BUFFER_SIZE 10

#define MX_SENDER_MAXSIZE 128
#define MX_DATA_MAXSIZE 512

#define MX_NO_SOCKET -1

typedef struct sockaddr_in t_saddr;

typedef struct  s_message {                         // t_message
    char sender[128];
    char data[512];
}               t_message;

typedef struct s_message_queue {                    // t_message_queue
    int size;
    t_message *data;
    int current;
}               t_message_queue;

typedef struct  s_peer {                            // t_peer
    int socket;
    struct sockaddr_in addres;

    t_message_queue send_buffer;  // Messages that waiting for send.

    /* Buffered sending message.
    *
    * In case we doesn't send whole message per one call send().
    * And current_sending_byte is a pointer to the part of data
    * that will be send next call.
    */
    t_message sending_buffer;
    size_t current_sending_byte;

    /* The same for the receiving message. */
    t_message receiving_buffer;
    size_t current_receiving_byte;
}               t_peer;

typedef struct  s_sock {                            // t_sock
    int master_socket;
    int addrlen;
    t_saddr address;
    int opt;
    int max_sd;

    t_peer connection_list[MAX_CLIENTS];

    int client_sockets[MAX_CLIENTS];

    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;

    char buffer[1024];
    char read_stdin_buffer[1024]; // buffer for server stdin
    int valread;
}               t_sock;

typedef struct  s_info {
    t_sock *sock;
}               t_info;


/* Utils */

    /* True utils for easy init */
void mx_initialize_zero_int_arr(int *arr, int size);
void mx_strip_newline(char *s);

    /* Peer.c */
int mx_delete_peer(t_peer *peer);
int mx_create_peer(t_peer *peer);
char *mx_peer_get_addres_str(t_peer *peer);
int mx_peer_add_to_send(t_peer *peer, t_message *message);

    /* Message_queue.c */
int mx_create_message_queue(int queue_size, t_message_queue *queue);
void mx_delete_message_queue(t_message_queue *queue);
int mx_enqueue(t_message_queue *queue, t_message *message);
int mx_dequeue(t_message_queue *queue, t_message *message);
int mx_dequeue_all(t_message_queue *queue);

    /* Message.c */
int mx_prepare_message(char *sender, char *data, t_message *message);
int mx_print_message(t_message *message);
int mx_handle_received_message(t_info *info, t_message *message);

/* Sockets */

    /* Create and initialazing */
void mx_socket_bind_to_port(t_sock *sock);
void mx_socket_specify_maximum_connections_to_master(int sock, int con_num);
void mx_set_nonblock_for_stdin(void);
t_sock *mx_sockets_create_struct(int port);
void mx_sockets_initialize(t_sock *sock, int port);

void mx_socket_bind_to_port(t_sock *sock);
void mx_socket_specify_maximum_connections_to_master(int sock, int con_num);

    /* Loop func */
void mx_sockets_loop(t_info *info);
void mx_loop_handler(t_info *info);
int mx_handle_read_from_stdin(t_info *info);
void mx_handle_new_connection(t_info *info);
void mx_handle_disconnect(t_sock *sock, t_peer *client);
void mx_handle_incoming_data(t_info *info);

    /* Message handeling */
int mx_receive_from_peer(t_info *info, t_peer *peer,
                         int (*message_handler)(t_info *, t_message *));
int mx_send_to_peer(t_info *info, t_peer *peer);

/* Signals */

// int mx_setup_signals();


void mx_shutdown_properly(t_info *info, int code);
