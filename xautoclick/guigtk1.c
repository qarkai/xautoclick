/* ------------------------------------------------------------------------- */

/*
 * $Id$
 * 
 * xAutoClick
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

/* ------------------------------------------------------------------------- */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <gtk/gtk.h>

#include "main.h"

/* ------------------------------------------------------------------------- */

static Display *display;

static GtkWidget *gAutoClick, *tap_button, *stop_button, *start_button;
static GtkWidget *predelay_spin, *interval_spin, *random_spin;
static GtkWidget *nrofclicks_spin;

/* ------------------------------------------------------------------------- */

void click_mouse_button(void) {

    XTestFakeButtonEvent(display, 1, True, CurrentTime);
    XTestFakeButtonEvent(display, 1, False, CurrentTime);
    XFlush(display);
}

/* ------------------------------------------------------------------------- */

static gboolean myalarm(gpointer data) {

    common_alarm_callback();

    return FALSE;   /* call only once; FALSE destroys timer */
}

/* ------------------------------------------------------------------------- */

void set_alarm(int ms) {
    g_timeout_add(ms, myalarm, NULL);
}

/* ------------------------------------------------------------------------- */

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

/* ------------------------------------------------------------------------- */

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

/* ------------------------------------------------------------------------- */

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

/* ------------------------------------------------------------------------- */

static void on_tap_button_clicked(GtkButton *button, gpointer user_data) {

    common_tap_button();

    return;
}

/* ------------------------------------------------------------------------- */

static void on_stop_button_clicked(GtkButton *button, gpointer user_data) {

    common_stop_button();

    return;
}

/* ------------------------------------------------------------------------- */

static void on_start_button_clicked(GtkButton *button, gpointer user_data) {

    common_start_button();

    return;
}

/* ------------------------------------------------------------------------- */

static GtkWidget *create_gAutoClick(void) {
  GtkWidget *gAutoClick;
  GtkWidget *vbox1;
  GtkWidget *hbox1;
  GtkWidget *predelay_label;
  GtkWidget *label8;
  GtkObject *predelay_spin_adj;
  GtkWidget *hbox2;
  GtkWidget *interval_label;
  GtkWidget *label9;
  GtkObject *interval_spin_adj;
  GtkWidget *hbox3;
  GtkWidget *random_label;
  GtkWidget *label10;
  GtkObject *random_spin_adj;
  GtkWidget *hbox5;
  GtkWidget *nrofclicks_label;
  GtkWidget *label11;
  GtkObject *nrofclicks_spin_adj;
  GtkWidget *hbox4;

  gAutoClick = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_object_set_data (GTK_OBJECT (gAutoClick), "gAutoClick", gAutoClick);
  gtk_container_set_border_width (GTK_CONTAINER (gAutoClick), 4);
  gtk_window_set_title (GTK_WINDOW (gAutoClick), "gAutoClick");
/*  gtk_window_set_position (GTK_WINDOW (gAutoClick), GTK_WIN_POS_CENTER); */
  gtk_window_set_policy (GTK_WINDOW (gAutoClick), FALSE, FALSE, FALSE);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox1);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "vbox1", vbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (gAutoClick), vbox1);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox1);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "hbox1", hbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox1, TRUE, TRUE, 0);

  predelay_label = gtk_label_new ("Pre-delay  ");
  gtk_widget_ref (predelay_label);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "predelay_label", predelay_label,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (predelay_label);
  gtk_box_pack_start (GTK_BOX (hbox1), predelay_label, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (predelay_label), GTK_JUSTIFY_FILL);

  label8 = gtk_label_new ("");
  gtk_widget_ref (label8);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "label8", label8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label8);
  gtk_box_pack_start (GTK_BOX (hbox1), label8, TRUE, FALSE, 0);

  predelay_spin_adj = gtk_adjustment_new (2000, 0, 10240, 1, 10, 10);
  predelay_spin = gtk_spin_button_new (GTK_ADJUSTMENT (predelay_spin_adj), 1, 0);
  gtk_widget_ref (predelay_spin);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "predelay_spin", predelay_spin,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (predelay_spin);
  gtk_box_pack_start (GTK_BOX (hbox1), predelay_spin, FALSE, FALSE, 0);
  gtk_widget_set_usize (predelay_spin, 64, -2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (predelay_spin), TRUE);

  hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox2);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "hbox2", hbox2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox2, TRUE, TRUE, 0);

  interval_label = gtk_label_new ("Interval  ");
  gtk_widget_ref (interval_label);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "interval_label", interval_label,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (interval_label);
  gtk_box_pack_start (GTK_BOX (hbox2), interval_label, FALSE, FALSE, 0);

  label9 = gtk_label_new ("");
  gtk_widget_ref (label9);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "label9", label9,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label9);
  gtk_box_pack_start (GTK_BOX (hbox2), label9, TRUE, FALSE, 0);

  interval_spin_adj = gtk_adjustment_new (1000, 0, 10240, 1, 10, 10);
  interval_spin = gtk_spin_button_new (GTK_ADJUSTMENT (interval_spin_adj), 1, 0);
  gtk_widget_ref (interval_spin);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "interval_spin", interval_spin,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (interval_spin);
  gtk_box_pack_start (GTK_BOX (hbox2), interval_spin, FALSE, FALSE, 0);
  gtk_widget_set_usize (interval_spin, 64, -2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (interval_spin), TRUE);

  hbox3 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox3);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "hbox3", hbox3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox3);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox3, TRUE, TRUE, 0);

  random_label = gtk_label_new ("Random +/-  ");
  gtk_widget_ref (random_label);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "random_label", random_label,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (random_label);
  gtk_box_pack_start (GTK_BOX (hbox3), random_label, FALSE, FALSE, 0);

  label10 = gtk_label_new ("");
  gtk_widget_ref (label10);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "label10", label10,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label10);
  gtk_box_pack_start (GTK_BOX (hbox3), label10, TRUE, FALSE, 0);

  random_spin_adj = gtk_adjustment_new (250, 0, 1024, 1, 10, 10);
  random_spin = gtk_spin_button_new (GTK_ADJUSTMENT (random_spin_adj), 1, 0);
  gtk_widget_ref (random_spin);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "random_spin", random_spin,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (random_spin);
  gtk_box_pack_start (GTK_BOX (hbox3), random_spin, FALSE, FALSE, 0);
  gtk_widget_set_usize (random_spin, 64, -2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (random_spin), TRUE);

  hbox5 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox5);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "hbox5", hbox5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox5);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox5, TRUE, TRUE, 0);

  nrofclicks_label = gtk_label_new ("# of clicks  ");
  gtk_widget_ref (nrofclicks_label);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "nrofclicks_label", nrofclicks_label,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (nrofclicks_label);
  gtk_box_pack_start (GTK_BOX (hbox5), nrofclicks_label, FALSE, FALSE, 0);

  label11 = gtk_label_new ("");
  gtk_widget_ref (label11);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "label11", label11,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label11);
  gtk_box_pack_start (GTK_BOX (hbox5), label11, TRUE, FALSE, 0);

  nrofclicks_spin_adj = gtk_adjustment_new (32, 1, 1024, 1, 10, 10);
  nrofclicks_spin = gtk_spin_button_new (GTK_ADJUSTMENT (nrofclicks_spin_adj), 1, 0);
  gtk_widget_ref (nrofclicks_spin);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "nrofclicks_spin", nrofclicks_spin,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (nrofclicks_spin);
  gtk_box_pack_start (GTK_BOX (hbox5), nrofclicks_spin, FALSE, FALSE, 0);
  gtk_widget_set_usize (nrofclicks_spin, 64, -2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (nrofclicks_spin), TRUE);

  hbox4 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox4);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "hbox4", hbox4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox4);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox4, TRUE, TRUE, 0);

  tap_button = gtk_button_new_with_label ("Tap");
  gtk_widget_ref (tap_button);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "tap_button", tap_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (tap_button);
  gtk_box_pack_start (GTK_BOX (hbox4), tap_button, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (tap_button), 2);

  stop_button = gtk_button_new_with_label ("Stop");
  gtk_widget_ref (stop_button);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "stop_button", stop_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (stop_button);
  gtk_box_pack_start (GTK_BOX (hbox4), stop_button, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (stop_button), 2);

  start_button = gtk_button_new_with_label ("Start");
  gtk_widget_ref (start_button);
  gtk_object_set_data_full (GTK_OBJECT (gAutoClick), "start_button", start_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (start_button);
  gtk_box_pack_start (GTK_BOX (hbox4), start_button, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (start_button), 2);

  gtk_signal_connect (GTK_OBJECT (gAutoClick), "delete_event",
                      GTK_SIGNAL_FUNC (gtk_main_quit),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (tap_button), "clicked",
                      GTK_SIGNAL_FUNC (on_tap_button_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (stop_button), "clicked",
                      GTK_SIGNAL_FUNC (on_stop_button_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (start_button), "clicked",
                      GTK_SIGNAL_FUNC (on_start_button_clicked),
                      NULL);

  return gAutoClick;
}

/* ------------------------------------------------------------------------- */

int init_gui(int argc, char **argv) {

    display = XOpenDisplay(NULL);

    if (!display) {
        fprintf(stderr, "Unable to open X display\n");
        return 0;
    }

    gtk_set_locale();
    gtk_init(NULL, NULL);

    gAutoClick = create_gAutoClick();
    gtk_widget_show(gAutoClick);

    return 1;
}

/* ------------------------------------------------------------------------- */

void close_gui(void) {

    XCloseDisplay(display);
}

/* ------------------------------------------------------------------------- */

void main_loop(void) {

    gtk_main();

    return;
}

/* ------------------------------------------------------------------------- */

