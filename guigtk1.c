/* xAutoClick -- GTK1 GUI
 *
 * Copyright (C) 2006 Ivo van Poorten
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
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <gtk/gtk.h>

#include "main.h"

static Display *display;

static GtkWidget *gAutoClick, *tap_button, *stop_button, *start_button;
static GtkWidget *predelay_spin, *interval_spin, *random_spin;
static GtkWidget *nrofclicks_spin;

void click_mouse_button(void) {
    XTestFakeButtonEvent(display, 1, True, CurrentTime);
    XTestFakeButtonEvent(display, 1, False, CurrentTime);
    XFlush(display);
}

static gboolean myalarm(gpointer data) {
    common_alarm_callback();
    return FALSE;   /* call only once; FALSE destroys timer */
}

void set_alarm(int ms) {
    g_timeout_add(ms, myalarm, NULL);
}

int get_spin_value(spin_t spin) {

    switch(spin) {
    case SPIN_PREDELAY:
        return gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(predelay_spin));
        break;
    case SPIN_INTERVAL:
        return gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(interval_spin));
        break;
    case SPIN_RANDOM:
        return gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(random_spin));
        break;
    case SPIN_NUMBER:
        return gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(nrofclicks_spin));
        break;
    }

    return 0;
}

void set_spin_value(spin_t spin, int value) {

    switch(spin) {
    case SPIN_PREDELAY:
        break;
    case SPIN_INTERVAL:
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(interval_spin), value);
        break;
    case SPIN_RANDOM:
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(random_spin), value);
        break;
    case SPIN_NUMBER:
        break;
    }
}

void set_button_sensitive(button_t button, int state) {

    switch(button) {
    case BUTTON_TAP:
        gtk_widget_set_sensitive(tap_button, state ? TRUE : FALSE);
        break;
    case BUTTON_STOP:
        gtk_widget_set_sensitive(stop_button, state ? TRUE : FALSE);
        break;
    case BUTTON_START:
        gtk_widget_set_sensitive(start_button, state ? TRUE : FALSE);
        break;
    }
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

static void add_widget(GtkObject *root, gchar *name, gchar *type, GtkWidget *widget)
{
    gchar *key = g_strdup_printf("%s_%s", name, type);

    gtk_widget_ref(widget);
    gtk_object_set_data_full(root, key, widget,
                             (GtkDestroyNotify) gtk_widget_unref);
    g_free(key);
    gtk_widget_show(widget);
}

static GtkWidget *create_labeled_spin(GtkObject *root,
                                      GtkWidget *vbox,
                                      gchar *spin_type,
                                      const gchar *label_text,
                                      gdouble spin_value, gdouble spin_min_value)
{
    GtkObject *adj;
    GtkWidget *hbox;
    GtkWidget *label;
    GtkWidget *spin;
    const gchar *key;

    hbox = gtk_hbox_new (FALSE, 0);
    add_widget(root, spin_type, "hbox", hbox);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);

    label = gtk_label_new (label_text);
    add_widget(root, spin_type, "label", label);
    gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
    /*gtk_label_set_justify (GTK_LABEL (predelay_label), GTK_JUSTIFY_FILL);*/

    label = gtk_label_new ("");
    add_widget(root, spin_type, "empty_label", label);
    gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, FALSE, 0);

    adj = gtk_adjustment_new (spin_value, spin_min_value, INT_MAX, 1, 10, 0);

    spin = gtk_spin_button_new (GTK_ADJUSTMENT (adj), 1, 0);
    add_widget(root, spin_type, "spin", spin);
    gtk_box_pack_start (GTK_BOX (hbox), spin, FALSE, FALSE, 0);

    gtk_widget_set_usize (spin, 64, -2);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin), TRUE);

    return spin;
}

static GtkWidget *create_labeled_button(GtkObject *root,
                                        GtkWidget *hbox,
                                        gchar *button_type,
                                        const gchar *button_text,
                                        void *on_button_clicked_func)
{
    GtkWidget *button;

    button = gtk_button_new_with_label (button_text);
    add_widget(root, button_type, "button", button);
    gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (button), 2);
    gtk_signal_connect (GTK_OBJECT (button), "clicked",
                        GTK_SIGNAL_FUNC (on_button_clicked_func),
                        NULL);

    return button;
}

static GtkWidget *create_gAutoClick(void) {
  GtkObject *gAutoClick_obj;
  GtkWidget *gAutoClick;
  GtkWidget *vbox;
  GtkWidget *hbox;

  gAutoClick = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gAutoClick_obj = GTK_OBJECT (gAutoClick);
  gtk_object_set_data (gAutoClick_obj, "gAutoClick", gAutoClick);

  gtk_container_set_border_width (GTK_CONTAINER (gAutoClick), 4);
  gtk_window_set_title (GTK_WINDOW (gAutoClick), "gAutoClick");
/*  gtk_window_set_position (GTK_WINDOW (gAutoClick), GTK_WIN_POS_CENTER); */
  gtk_window_set_policy (GTK_WINDOW (gAutoClick), FALSE, FALSE, FALSE);

  vbox = gtk_vbox_new (FALSE, 0);
  add_widget(gAutoClick_obj, "", "vbox", vbox);
  gtk_container_add (GTK_CONTAINER (gAutoClick), vbox);

  predelay_spin = create_labeled_spin(gAutoClick_obj, vbox, "predelay", "Pre-delay  ", 2000, 0);
  interval_spin = create_labeled_spin(gAutoClick_obj, vbox, "interval", "Interval  ", 1000, 0);
  random_spin = create_labeled_spin(gAutoClick_obj, vbox, "random", "Random +/-  ", 250, 0);
  nrofclicks_spin = create_labeled_spin(gAutoClick_obj, vbox, "nrofclicks", "# of clicks  ", 32, 1);

  hbox = gtk_hbox_new (FALSE, 0);
  add_widget(gAutoClick_obj, "", "hbox", hbox);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);

  tap_button = create_labeled_button(gAutoClick_obj, hbox, "tap", "Tap", on_tap_button_clicked);
  stop_button = create_labeled_button(gAutoClick_obj, hbox, "stop", "Stop", on_stop_button_clicked);
  start_button = create_labeled_button(gAutoClick_obj, hbox, "start", "Start", on_start_button_clicked);

  gtk_signal_connect (gAutoClick_obj, "delete_event",
                      GTK_SIGNAL_FUNC (gtk_main_quit),
                      NULL);
  return gAutoClick;
}

int init_gui(int argc, char **argv) {

    if (!(display = XOpenDisplay(NULL))) {
        fprintf(stderr, "Unable to open X display\n");
        return 0;
    }

    gtk_set_locale();
    gtk_init(NULL, NULL);

    gAutoClick = create_gAutoClick();
    gtk_widget_show(gAutoClick);

    return 1;
}

void close_gui(void) {
    XCloseDisplay(display);
}

void main_loop(void) {
    gtk_main();
}
