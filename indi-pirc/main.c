/*
 * Copyright (C) 2015 Roman Borisov <rborisoff@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>
#include "rpc-utils.h"

AppIndicator *Indicator = NULL;
GtkWidget *Menu = NULL;

static void indicator_init ();
static void menu_init ();

int main(int argc, char* argv[])
{
    // Initialize GTK+
    gtk_init(&argc, &argv);
    indicator_init();
    menu_init();

    // Run the main GTK+ event loop
    gtk_main();

    return 0;
}

static void quit ()
{
    gtk_main_quit ();
}

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
}

static void menu_init ()
{
    GtkWidget* item;
    Menu = gtk_menu_new ();
    
    item = gtk_menu_item_new_with_label ("Play/Pause");
    gtk_menu_append (Menu, item);
    g_signal_connect (G_OBJECT (item), "activate", G_CALLBACK (rpc_play_toggle), NULL);
    gtk_widget_show_all (item);

    item = gtk_menu_item_new_with_label ("Quit");
    gtk_menu_append (Menu, item);
    g_signal_connect (G_OBJECT (item), "activate", G_CALLBACK (quit), NULL);
    gtk_widget_show_all (item);

    app_indicator_set_menu (Indicator, GTK_MENU (Menu));
}

