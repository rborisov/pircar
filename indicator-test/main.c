#include <stdio.h>
#include <libappindicator/app-indicator.h>

AppIndicator *Indicator = NULL;
GtkWidget *Menu = NULL;

static void indicator_init ()
{
    /* TODO: set actual icon */
    Indicator = app_indicator_new (
                                   "indi-player",
                                   "/usr/share/pixmaps/atom.png",
                                   APP_INDICATOR_CATEGORY_APPLICATION_STATUS
                                   );

    app_indicator_set_attention_icon (Indicator, "indicator-sound");
    app_indicator_set_status (Indicator, APP_INDICATOR_STATUS_ACTIVE);
    //app_indicator_set_label (Indicator, "indi player", NULL);
}

static void quit ()
{
//        player_destroy ();
//            notifier_destroy ();
//                hotkeys_destroy ();
    gtk_main_quit ();
}

static void menu_init ()
{
    GtkWidget* item;
    Menu = gtk_menu_new ();

/*    gchar *xdg_music_path = (gchar*)g_get_user_special_dir (G_USER_DIRECTORY_MUSIC);
    if (!xdg_music_path)
        return;

    GtkWidget* item = path_to_menu (xdg_music_path);
    gtk_menu_append (Menu, item);

    gtk_widget_show_all (item);
*/
    /* Line */
    item = gtk_separator_menu_item_new ();
    gtk_menu_append (Menu, item);
    gtk_widget_show_all (item);

    /* Shuffle */
/*    item = gtk_check_menu_item_new_with_label ("Shuffle");
    gtk_menu_append (Menu, item);
    g_signal_connect (G_OBJECT (item), "activate", G_CALLBACK (call_shuffle), NULL);
    gtk_widget_show_all (item);
*/
    /* Play/pause */
/*    item = gtk_menu_item_new_with_label ("Play/Pause");
    gtk_menu_append (Menu, item);
    g_signal_connect (G_OBJECT (item), "activate", G_CALLBACK (play_or_pause), NULL);
    gtk_widget_show_all (item);
*/
    /* Stop */
/*    item = gtk_menu_item_new_with_label ("Stop");
    gtk_menu_append (Menu, item);
    g_signal_connect (G_OBJECT (item), "activate", G_CALLBACK (stop), NULL);
    gtk_widget_show_all (item);
*/
    /* Previous */
/*    item = gtk_menu_item_new_with_label ("Prev track");
    gtk_menu_append (Menu, item);
    g_signal_connect (G_OBJECT (item), "activate", G_CALLBACK (prev_track), NULL);
    gtk_widget_show_all (item);
*/
    /* Next */
/*    item = gtk_menu_item_new_with_label ("Next track");
    gtk_menu_append (Menu, item);
    g_signal_connect (G_OBJECT (item), "activate", G_CALLBACK (next_track), NULL);
    gtk_widget_show_all (item);
*/
    /* Quit */
    item = gtk_menu_item_new_with_label ("Quit");
    gtk_menu_append (Menu, item);
    g_signal_connect (G_OBJECT (item), "activate", G_CALLBACK (quit), NULL);
    gtk_widget_show_all (item);

    app_indicator_set_menu (Indicator, GTK_MENU (Menu));
}

int main(int argc, char **argv)
{
    gtk_init (&argc, &argv);
    
    printf("Hello World!\n");
    indicator_init();
    menu_init();

    gtk_main ();
    return 0;
}

