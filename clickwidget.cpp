/* xAutoClick, Qt 4 GUI
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

#include "clickwidget.h"

extern "C" {
#include "main.h"
#include <limits.h>
}

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

ClickWidget::ClickWidget(QWidget *parent) : QWidget(parent) {
    QString label[SPINS_COUNT] = { "Pre-delay", "Interval", "Random +/-", "# of clicks" };
    QString butnames[BUTTONS_COUNT] = { "Tap", "Stop", "Start" };

    QVBoxLayout *vbox = new QVBoxLayout;

    for (int c = 0; c < SPINS_COUNT; c++) {
        QHBoxLayout *layout = new QHBoxLayout;
        spins[c] = new QSpinBox;
        spins[c]->setMinimum(1);
        spins[c]->setMaximum(INT_MAX);

        layout->addWidget(new QLabel(label[c]));
        layout->addWidget(spins[c]);
        vbox->addLayout(layout);
    }

    QHBoxLayout *layout = new QHBoxLayout;

    for (int c = 0; c < BUTTONS_COUNT; c++)
    {
        layout->addWidget(buttons[c] = new QPushButton(butnames[c]));
    }

    connect(buttons[BUTTON_TAP], SIGNAL(clicked()), this, SLOT(tap(void)));
    connect(buttons[BUTTON_STOP], SIGNAL(clicked()), this, SLOT(stop(void)));
    connect(buttons[BUTTON_START], SIGNAL(clicked()), this, SLOT(start(void)));

    vbox->addLayout(layout);
    setWindowTitle("Qt4AutoClick");
    setLayout(vbox);

    timer = new QTimer;
    timer->setSingleShot(TRUE);
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_done()));
}

void ClickWidget::startTimer(int ms) {
    timer->start(ms);
}

int ClickWidget::getSpinValue(int spin) {
    return spins[spin]->value();
}

void ClickWidget::setSpinValue(int spin, int value) {
    spins[spin]->setValue(value);
}

void ClickWidget::setButtonSensitive(int button, bool state) {
    buttons[button]->setEnabled(state);
}

void ClickWidget::tap(void) {
    common_tap_button();
}

void ClickWidget::stop(void) {
    common_stop_button();
}

void ClickWidget::start(void) {
    common_start_button();
}

void ClickWidget::timer_done(void) {
    common_alarm_callback();
}
