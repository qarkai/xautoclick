/* xAutoClick -- GTK3 GUI
 *
 * Copyright (C) 2006 Ivo van Poorten, 2019 Arkadiy Illarionov
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "gui.h"
#include "main.h"

#include <gtk/gtk.h>

#include <limits.h>

typedef struct gtk_gui_ctx {
    GtkWidget *window;
    GtkWidget *spins[SPINS_COUNT];
    GtkWidget *buttons[BUTTONS_COUNT];
} gtk_gui_t;

static gboolean myalarm(G_GNUC_UNUSED gpointer data) {
    common_alarm_callback();
    return FALSE;   /* call only once; FALSE destroys timer */
}

void set_alarm(int ms) {
    g_timeout_add(ms, myalarm, NULL);
}

static int gtk_gui_get_spin_value(gtk_gui_t* ctx, spin_t spin) {
    return gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ctx->spins[spin]));
}

static void gtk_gui_set_spin_value(gtk_gui_t* ctx, spin_t spin, int value) {
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(ctx->spins[spin]), value);
}

static void gtk_gui_set_button_sensitive(gtk_gui_t* ctx, button_t button, bool state) {
    gtk_widget_set_sensitive(ctx->buttons[button], state);
}

static void gtk_gui_main_loop(gtk_gui_t* ctx) {
    gtk_widget_show(ctx->window);
    gtk_main();
}

static void gtk_gui_close(gtk_gui_t* ctx) {
    free(ctx);
}

static void add_widget(GObject *root, GtkWidget *widget) {
    static guint n = 0;
    gchar *key = g_strdup_printf("widget_%u", n++);

    g_object_ref(widget);
    g_object_set_data_full(root, key, widget, g_object_unref);
    g_free(key);
    gtk_widget_show(widget);
}

static gboolean
add_suffix (GtkSpinButton *spin, gchar* suffix)
{
   GtkAdjustment *adjustment;
   gchar *text;
   int value;

   adjustment = gtk_spin_button_get_adjustment (spin);
   value = (int)gtk_adjustment_get_value (adjustment);
   text = g_strdup_printf ("%d %s", value, suffix);
   gtk_entry_set_text (GTK_ENTRY (spin), text);
   g_free (text);

   return TRUE;
}

static GtkWidget *create_labeled_spin(GObject *root,
                                      GtkWidget *vbox,
                                      const spin_param_t* param) {
    GtkAdjustment *adj;
    GtkWidget *hbox;
    GtkWidget *label;
    GtkWidget *spin;

    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 5);
    add_widget(root, hbox);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);

    label = gtk_label_new (param->descr);
    add_widget(root, label);
    gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
    /*gtk_label_set_justify (GTK_LABEL (predelay_label), GTK_JUSTIFY_FILL);*/

    label = gtk_label_new ("");
    add_widget(root, label);
    gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, FALSE, 0);

    adj = gtk_adjustment_new (param->min_value, param->min_value, INT_MAX, 1, 10, 0);

    spin = gtk_spin_button_new (adj, 1, 0);
    if (param->suffix && *param->suffix != '\0')
        g_signal_connect (G_OBJECT (spin), "output",
                          G_CALLBACK (add_suffix), (gpointer) param->suffix);
    else
        gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin), TRUE);

    add_widget(root, spin);
    gtk_box_pack_start (GTK_BOX (hbox), spin, FALSE, FALSE, 0);

    gtk_widget_set_size_request (spin, 64, 0);

    return spin;
}

static GtkWidget *create_labeled_button(GObject *root,
                                        GtkWidget *hbox,
                                        const gchar *button_text,
                                        GCallback on_button_clicked_func) {
    GtkWidget *button;

    button = gtk_button_new_with_label (button_text);
    add_widget(root, button);
    gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (button), 2);
    g_signal_connect (G_OBJECT (button), "clicked",
                      on_button_clicked_func, NULL);

    return button;
}

static void create_buttons(gtk_gui_t* ctx, GObject *root, GtkWidget *box, const char** names) {
    GCallback callbacks[BUTTONS_COUNT] = {
        G_CALLBACK (common_tap_button),
        G_CALLBACK (common_stop_button),
        G_CALLBACK (common_start_button)
    };

    for (int c = 0; c < BUTTONS_COUNT; ++c)
        ctx->buttons[c] = create_labeled_button(root, box, names[c], callbacks[c]);
}

static gboolean gautoclick_exit(GtkWidget *widget, G_GNUC_UNUSED GdkEvent *event, G_GNUC_UNUSED gpointer user_data) {
    gtk_widget_hide(widget);
    gtk_main_quit();
    return TRUE;
}

static void create_gAutoClick(gtk_gui_t* ctx, const spin_param_t* spin_params, const char** button_names) {
    GObject *gAutoClick_obj;
    GtkWidget *gAutoClick_win;
    GtkWidget *vbox;
    GtkWidget *hbox;

    gAutoClick_win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gAutoClick_obj = G_OBJECT (gAutoClick_win);
    g_object_set_data (gAutoClick_obj, "gAutoClick", gAutoClick_win);

    gtk_container_set_border_width (GTK_CONTAINER (gAutoClick_win), 4);
    gtk_window_set_icon_name (GTK_WINDOW (gAutoClick_win), "xautoclick");
    gtk_window_set_title (GTK_WINDOW (gAutoClick_win), "gAutoClick");
    /*  gtk_window_set_position (GTK_WINDOW (gAutoClick), GTK_WIN_POS_CENTER); */
    gtk_window_set_resizable(GTK_WINDOW (gAutoClick_win), FALSE);

    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    add_widget(gAutoClick_obj, vbox);
    gtk_container_add (GTK_CONTAINER (gAutoClick_win), vbox);

    for (int c = 0; c < SPINS_COUNT; ++c)
        ctx->spins[c] = create_labeled_spin(gAutoClick_obj, vbox, &spin_params[c]);

    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    add_widget(gAutoClick_obj, hbox);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);

    create_buttons(ctx, gAutoClick_obj, hbox, button_names);

    g_signal_connect (gAutoClick_obj, "delete-event",
                      G_CALLBACK (gautoclick_exit), NULL);

    ctx->window = gAutoClick_win;
}

void init_gui(gui_t* gui, const spin_param_t* spin_params, const char** button_names, int argc, char **argv) {
    gtk_gui_t* ctx;

    ctx = calloc(1, sizeof(gtk_gui_t));
    if (!ctx) {
        fprintf(stderr, "Can't allocate memory for GTK3 GUI\n");
        return;
    }

    gtk_init(&argc, &argv);

    create_gAutoClick(ctx, spin_params, button_names);

    gui->ctx = ctx;
    gui->set_button_sensitive = (gui_set_button_sensitive_t)gtk_gui_set_button_sensitive;
    gui->get_spin_value = (gui_get_spin_value_t)gtk_gui_get_spin_value;
    gui->set_spin_value = (gui_set_spin_value_t)gtk_gui_set_spin_value;
    gui->main_loop = (gui_main_loop_t)gtk_gui_main_loop;
    gui->close = (gui_close_t)gtk_gui_close;
}
