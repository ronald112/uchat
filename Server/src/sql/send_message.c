#include "server.h"

// static int callback(void *data, int argc, char **argv, char **cols) {
//     return 0;
// }

static int get_id(void *data, int argc, char **argv, char **cols) {
    // cJSON_AddNumberToObject(data, "id", atoi(argv[0]));
    // printf("user id = %d\n", atoi(argv[0]));
    // printf("user email = %s\n", argv[1]);

    int *cli = data;
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (cli[i] == 0) {
            cli[i] = atoi(argv[0]);
            break;
        }
    }
    return 0;
}

static void get_all_users(sqlite3 *db, cJSON *jsn) {
    char *query = NULL;
    char *err = NULL;
    int rc = 0;

    int *uid = malloc(MAX_CLIENTS * sizeof(int));
    bzero(uid, MAX_CLIENTS);

    // cJSON *arr = cJSON_CreateArray();
// cJSON *x_json = cJSON_CreateNumber(x);
// cJSON_AddItemToArray(arr, x_json);

    asprintf(&query, "SELECT uc.user_id, up.email FROM users_chats AS uc "
            "JOIN users_profiles AS up ON uc.user_id = up.user_id "
            "AND uc.chat_id = '%i' AND uc.user_id != '%i';",
            cJSON_GetObjectItem(jsn, "chat_id")->valueint,
            cJSON_GetObjectItem(jsn, "uid")->valueint);

    rc = sqlite3_exec(db, query, get_id, uid, &err);
    if (mx_check(rc, err, "get all users") != SQLITE_OK) {
        // cJSON_SetNumberValue(cJSON_GetObjectItem(jsn, "json_type"),
        //                     failed_change_password);
    }
    else {
        int count = 0;
        for (int i = 0; uid[i] != 0; ++i) {
            count++;
        }
        int *new_uid = malloc(count * sizeof(int));
        for (int i = 0; i < count; ++i) {
            new_uid[i] = uid[i];
        }
        free(uid);
        cJSON_AddItemToObject(jsn, "clients_id", cJSON_CreateIntArray(new_uid, count));

        if (count == 1) {
            cJSON_AddStringToObject(jsn, "email", "get email from db");
        }
    }
    free(query);
}

cJSON *mx_send_message(sqlite3 *db, cJSON *jsn) {
    char *query = NULL;
    char *err = NULL;
    int rc = 0;

    asprintf(&query, "INSERT INTO messages VALUES (NULL, '%i', '%i', '%i', "
            "datetime('now', 'localtime'), '%s');",
            cJSON_GetObjectItem(jsn, "uid")->valueint,
            cJSON_GetObjectItem(jsn, "chat_id")->valueint,
            cJSON_GetObjectItem(jsn, "type")->valueint,
            cJSON_GetObjectItem(jsn, "content")->valuestring);
    rc = sqlite3_exec(db, query, NULL, jsn, &err);

    if (mx_check(rc, err, "send message") != SQLITE_OK) {
        // cJSON_SetNumberValue(cJSON_GetObjectItem(jsn, "json_type"),
        //                     failed_change_password);
    }
    else {
        // cJSON_SetNumberValue(cJSON_GetObjectItem(jsn, "json_type"),
        //                     success_change_password);
        get_all_users(db, jsn);
    }
    free(query);
    return jsn;
}
