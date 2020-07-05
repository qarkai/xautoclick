/* xAutoClick, Qt GUI
 *
 * Copyright (C) 2010 Ivo van Poorten
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

extern "C" {
#include "gui.h"
#include "main.h"
}

#include "clickwidget.h"

#include <QApplication>
#include <QTimer>

static QApplication *app;
static QTimer *clickTimer;

namespace {

void qt_gui_set_button_sensitive(ClickWidget *clickWidget, button_t button, bool state) {
    clickWidget->setButtonSensitive(button, state);
}

int qt_gui_get_spin_value(ClickWidget *clickWidget, spin_t spin) {
    return clickWidget->getSpinValue(spin);
}

void qt_gui_set_spin_value(ClickWidget *clickWidget, spin_t spin, int value) {
    clickWidget->setSpinValue(spin, value);
}

void qt_gui_main_loop(ClickWidget *clickWidget) {
    clickWidget->show();
    QApplication::exec();
}

void qt_gui_close(ClickWidget *clickWidget) {
    delete clickTimer;
    delete clickWidget;
}

}

void set_alarm(int ms) {
    clickTimer->start(ms);
}

void init_gui(gui_t* gui, int argc, char **argv) {
    static int argn = argc; // QApplication reference to argc workaround
    app = new QApplication(argn, argv);

    clickTimer = new QTimer;
    clickTimer->setSingleShot(true);
    QObject::connect(clickTimer, &QTimer::timeout, common_alarm_callback);

    gui->ctx = new ClickWidget;
    gui->set_button_sensitive = (gui_set_button_sensitive_t)qt_gui_set_button_sensitive;
    gui->get_spin_value = (gui_get_spin_value_t)qt_gui_get_spin_value;
    gui->set_spin_value = (gui_set_spin_value_t)qt_gui_set_spin_value;
    gui->main_loop = (gui_main_loop_t)qt_gui_main_loop;
    gui->close = (gui_close_t)qt_gui_close;
}
