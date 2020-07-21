#include "client.h"

static void add_cnt_bld_json(int gid, const int coid, const int uid, int s_sock) {
    cJSON *jobj = cJSON_CreateObject();

    cJSON_AddNumberToObject(jobj, "json_type", make_add_new_contact);
    cJSON_AddNumberToObject(jobj, "uid", uid);
    cJSON_AddNumberToObject(jobj, "coid", coid);
    cJSON_AddNumberToObject(jobj, "gid", gid);

    mx_send_message_handler(jobj, s_sock);
    cJSON_Delete(jobj);
}

static int get_uid(const t_list *list, const char *u_l, const char *c_l) {
    t_list_node *node = list ? list->head : NULL;

    for (; node; node = node->next) {
        if (strcmp(((t_user *)node->data)->login, u_l) == 0) {
            return ((t_user *)node->data)->id;
        }
    }
    return -1;
}

static int get_gid(const t_list *list, char *gname) {
    t_list_node *node = list ? list->head : NULL;

    for (; node; node = node->next) {
        if (strcmp(((t_group *)node->data)->name, gname) == 0) {
            return ((t_group *)node->data)->id;
        }
    }
    mx_strdel(&gname);
    return 0;
}

void mx_add_cnt_json_wrapper(t_contact_add *ac) {
    const char *usr_login = gtk_entry_get_text(GTK_ENTRY(ac->entry));
    int gid = get_gid(ac->info->cl_data->cont_grp_names, gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(ac->combobox)));
    int coid = get_uid(ac->info->cl_data->tmp_users, usr_login, ac->info->cl_data->profile->login);

    if (coid != -1) {
        add_cnt_bld_json(gid, coid, ac->info->cl_data->profile->id, ac->info->sock->sock);
        mx_wait_for_json(ac->info, failed_add_new_contact, success_add_new_contact);
        if (mx_get_jtype(ac->info, success_add_new_contact)) {
            mx_get_json_contacts(ac->info);
        }
        else {
            mx_dialog_warning_create(NULL, "Failed add new_contact!\n");
        }
    }
    else {
        mx_dialog_warning_create(NULL, "The contact does not exist or this is your login!\n");
    }
}
