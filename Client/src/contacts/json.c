#include "client.h"

static void send_contacts_request(const t_info *info) {
    cJSON *jobj = cJSON_CreateObject();

    cJSON_AddNumberToObject(jobj, "json_type", get_client_contacts);
    cJSON_AddNumberToObject(jobj, "uid", info->cl_data->profile->id);

    mx_send_message_handler(jobj, info->sock->sock);
    cJSON_Delete(jobj);
}

static t_contact *get_contact(const cJSON *iterator) {
    t_contact *c = malloc(sizeof(t_contact));

    c->cid = cJSON_GetObjectItem(iterator, "coid")->valueint;
    c->login = cJSON_GetObjectItem(iterator, "login")->valuestring;
    c->f_name = cJSON_GetObjectItem(iterator, "fname")->valuestring;
    c->s_name = cJSON_GetObjectItem(iterator, "sname")->valuestring;
    c->email = cJSON_GetObjectItem(iterator, "email")->valuestring;
    c->stat = cJSON_GetObjectItem(iterator, "status")->valuestring;
    c->active = cJSON_GetObjectItem(iterator, "active")->valueint;
    c->grp_id = cJSON_GetObjectItem(iterator, "gid")->valueint;
    return c;
}

static void save_contacts(const t_info *info) {
    if (cJSON_IsObject(info->json)) {
        cJSON *iterator = NULL;
        cJSON *contacts = cJSON_GetObjectItem(info->json, "contacts");

        if (cJSON_IsArray(contacts)) {
            cJSON_ArrayForEach(iterator, contacts) {
                mx_push_back(info->cl_data->contacts, get_contact(iterator));
            }
        }
        else
            fprintf(stderr, "user contact json saving error\n");
    }
    else
        fprintf(stderr, "user contact json saving error\n");
}

void mx_get_json_contacts(t_info *info) {
    send_contacts_request(info);
    mx_wait_for_json(info, success_client_contacts, failed_client_contacts);
    if (mx_get_jtype(info, success_client_contacts)) {
        if (MX_MALLOC_SIZE(info->cl_data->contacts) > 0) {
            mx_clr_custom_lst(info->cl_data->contacts);
        }
        info->cl_data->contacts = mx_create_list();
        save_contacts(info);
        mx_upd_groups_list(info);
    }
    else {
        mx_dialog_warning_create(NULL, "Failed get client contacts!");
    }
}
