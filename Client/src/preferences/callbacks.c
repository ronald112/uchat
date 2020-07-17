#include "client.h"

void mx_on_toggle_vnoti(GtkWidget *widget, gpointer data) {
    t_info *info = data;
    t_preferences *pref = info->windows->pref;

    info->cl_data->profile->vs_noty = mx_get_tactive(pref->vnotify);
    mx_upd_prof_build_json_wrapper(info);
}

void mx_on_toggle_snoti(GtkWidget *widget, gpointer data) {
    t_info *info = data;
    t_preferences *pref = info->windows->pref;

    info->cl_data->profile->sound_noty = mx_get_tactive(pref->snotify);
    mx_upd_prof_build_json_wrapper(info);
}

void mx_on_toggle_enoti(GtkWidget *widget, gpointer data) {
    t_info *info = data;
    t_preferences *pref = info->windows->pref;

    info->cl_data->profile->email_noty = mx_get_tactive(pref->enotify);
    mx_upd_prof_build_json_wrapper(info);
}

void mx_on_change_password(GtkWidget *widget, gpointer data) {
    t_info *info = data;
    t_preferences *pref = info->windows->pref;

    mx_change_password_build(info, info->windows->cp);
}

void mx_on_theme_switch(GtkWidget *widget, gboolean state, gpointer data) {
    t_info *info = data;
    t_preferences *pref = info->windows->pref;

    if (state)
        mx_css_from_file(info, "./Resources/css/chat_dark.css");
    else
        mx_css_from_file(info, "./Resources/css/chat_light.css");
}

void mx_on_del_profile(GtkWidget *widget, gpointer data) {
    t_info *info = data;
    t_preferences *pref = info->windows->pref;

    mx_delete_profile_build(info, info->windows->dp);
}
