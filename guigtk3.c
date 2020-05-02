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

#include <stdio.h>
#include <limits.h>
#include <gtk/gtk.h>

#include "main.h"

static GtkWidget *gAutoClick;
static GtkWidget *buttons[BUTTONS_COUNT];
static GtkWidget *spins[SPINS_COUNT];

static gboolean myalarm(gpointer data) {
    common_alarm_callback();
    return FALSE;   /* call only once; FALSE destroys timer */
}

void set_alarm(int ms) {
    g_timeout_add(ms, myalarm, NULL);
}

int get_spin_value(spin_t spin) {
    return gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spins[spin]));
}

void set_spin_value(spin_t spin, int value) {
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spins[spin]), value);
}

void set_button_sensitive(button_t button, bool state) {
    gtk_widget_set_sensitive(buttons[button], state);
}

static void on_tap_button_clicked(GtkButton *button, gpointer user_data) {
    common_tap_button();
}

static void on_stop_button_clicked(GtkButton *button, gpointer user_data) {
    common_stop_button();
}

static void on_start_button_clicked(GtkButton *button, gpointer user_data) {
    common_start_button();
}

static void add_widget(GObject *root, GtkWidget *widget)
{
    static guint n = 0;
    gchar *key = g_strdup_printf("widget_%u", n++);

    g_object_ref(widget);
    g_object_set_data_full(root, key, widget, g_object_unref);
    g_free(key);
    gtk_widget_show(widget);
}

static GtkWidget *create_labeled_spin(GObject *root,
                                      GtkWidget *vbox,
                                      const gchar *label_text,
                                      gdouble spin_min_value)
{
    GtkAdjustment *adj;
    GtkWidget *hbox;
    GtkWidget *label;
    GtkWidget *spin;

    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    add_widget(root, hbox);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);

    label = gtk_label_new (label_text);
    add_widget(root, label);
    gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
    /*gtk_label_set_justify (GTK_LABEL (predelay_label), GTK_JUSTIFY_FILL);*/

    label = gtk_label_new ("");
    add_widget(root, label);
    gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, FALSE, 0);

    adj = gtk_adjustment_new (spin_min_value, spin_min_value, INT_MAX, 1, 10, 0);

    spin = gtk_spin_button_new (adj, 1, 0);
    add_widget(root, spin);
    gtk_box_pack_start (GTK_BOX (hbox), spin, FALSE, FALSE, 0);

    gtk_widget_set_size_request (spin, 64, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin), TRUE);

    return spin;
}

static void create_spins(GObject *root, GtkWidget *box) {
    struct spin_param {
        const char* text;
        int min_value;
    } spin_params[SPINS_COUNT] = {
        {"Pre-delay  ", 0},
        {"Interval  ", 0},
        {"Random +/-  ", 0},
        {"# of clicks  ", 1}
    };

    for (int c = 0; c < SPINS_COUNT; ++c)
        spins[c] = create_labeled_spin(root, box, spin_params[c].text, spin_params[c].min_value);
}

static GtkWidget *create_labeled_button(GObject *root,
                                        GtkWidget *hbox,
                                        const gchar *button_text,
                                        GCallback on_button_clicked_func)
{
    GtkWidget *button;

    button = gtk_button_new_with_label (button_text);
    add_widget(root, button);
    gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (button), 2);
    g_signal_connect (G_OBJECT (button), "clicked",
                      on_button_clicked_func, NULL);

    return button;
}

static void create_buttons(GObject *root, GtkWidget *box) {
    struct btn_param {
        const char* text;
        GCallback callback;
    } btn_params[BUTTONS_COUNT] = {
        {"Tap", G_CALLBACK (on_tap_button_clicked)},
        {"Stop", G_CALLBACK (on_stop_button_clicked)},
        {"Start", G_CALLBACK (on_start_button_clicked)}
    };

    for (int c = 0; c < BUTTONS_COUNT; ++c)
        buttons[c] = create_labeled_button(root, box, btn_params[c].text, btn_params[c].callback);
}

static void gautoclick_exit(void)
{
    set_options();
    gtk_main_quit();
}

static GtkWidget *create_gAutoClick(void) {
    GObject *gAutoClick_obj;
    GtkWidget *gAutoClick_win;
    GtkWidget *vbox;
    GtkWidget *hbox;

    gAutoClick_win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gAutoClick_obj = G_OBJECT (gAutoClick_win);
    g_object_set_data (gAutoClick_obj, "gAutoClick", gAutoClick_win);

    gtk_container_set_border_width (GTK_CONTAINER (gAutoClick_win), 4);
    gtk_window_set_title (GTK_WINDOW (gAutoClick_win), "gAutoClick");
    /*  gtk_window_set_position (GTK_WINDOW (gAutoClick), GTK_WIN_POS_CENTER); */
    gtk_window_set_resizable(GTK_WINDOW (gAutoClick_win), FALSE);

    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    add_widget(gAutoClick_obj, vbox);
    gtk_container_add (GTK_CONTAINER (gAutoClick_win), vbox);

    create_spins(gAutoClick_obj, vbox);

    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    add_widget(gAutoClick_obj, hbox);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);

    create_buttons(gAutoClick_obj, hbox);

    g_signal_connect (gAutoClick_obj, "delete_event",
                      G_CALLBACK (gautoclick_exit), NULL);
    return gAutoClick_win;
}

int init_gui(int argc, char **argv) {
    gtk_init(NULL, NULL);

    gAutoClick = create_gAutoClick();

    get_options();

    gtk_widget_show(gAutoClick);

    return 1;
}

void close_gui(void) {
}

void main_loop(void) {
    gtk_main();
}
