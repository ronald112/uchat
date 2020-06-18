#pragma once

#include "uchat.h"

/*    Defines for widgets placement */
#define WIDTH 1280
#define HEIGHT 720

/*    Define for укорачивания(не нашёл англ слова в голове) */
#define MX_BOX_V GTK_ORIENTATION_VERTICAL
#define MX_CB G_CALLBACK
#define MX_GSIG_CON g_signal_connect

/* Types for sending jsons */
typedef enum {
    mx_make_register,
    mx_failed_register,
    mx_success_register,
    mx_make_authorization,
    mx_failed_authorization,
    mx_success_authorization,
    mx_make_deletion,
    mx_failed_deletion,
    mx_success_deletion,
    mx_make_change_password,
    mx_failed_change_password,
    mx_success_change_password,
    mx_make_update_profile,
    mx_success_update_profile,
    mx_get_client_data,
    mx_send_client_data,
    mx_get_client_contacts,
    mx_send_client_contacts,
    mx_get_client_chats,
    mx_send_client_chats,
    mx_get_client_messages,
    mx_send_client_messages,
    mx_make_new_chat,
    mx_send_message,
} t_actions;
