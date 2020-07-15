#include "client.h"

t_chat *mx_chat_build(t_info *info, char *chat_name, int cid) {
    t_chat *chat = malloc(sizeof(t_chat));

    chat->builder = gtk_builder_new();
    gtk_builder_add_from_file(chat->builder, "./Resources/glade/chat.glade", NULL);

    chat->entry = GTK_WIDGET(gtk_builder_get_object(chat->builder, "entry"));
    chat->main_box = GTK_WIDGET(gtk_builder_get_object(chat->builder, "main_box"));
    chat->message_box = GTK_WIDGET(gtk_builder_get_object(chat->builder, "message_box"));
    chat->img_dialog = GTK_WIDGET(gtk_builder_get_object(chat->builder, "img_dialog"));
    chat->ffilter = GTK_FILE_FILTER(gtk_builder_get_object(chat->builder, "filter"));
    chat->msg_list = mx_create_list();
    gtk_builder_connect_signals(chat->builder, chat);
    gtk_file_filter_add_pixbuf_formats(chat->ffilter);

    chat->edit = 0;
    chat->cid = cid;
    chat->chat_name = chat_name;
    chat->info = info;

    gtk_widget_show(chat->main_box);
    return chat;
}

void mx_chat_destroy(t_list_node *chat_node) {
    t_chat *chat = chat_node->data;
    t_list_node *next = chat_node->next;
    t_list_node *prev = chat_node->prev;

    gtk_widget_destroy(chat->main_box);
    free(chat_node->data);
    
    prev->next = next;
    next->prev = prev;
}
