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

#ifndef __GUIQT3_H__
#define __GUIQT3_H__

/* ------------------------------------------------------------------------- */

#include <qvariant.h>
#include <qmainwindow.h>

/* ------------------------------------------------------------------------- */

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class QLabel;
class QPushButton;
class QSpinBox;

class qtautoclick : public QMainWindow
{
    Q_OBJECT

public:
    qtautoclick( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~qtautoclick();

    QSpinBox* spinInterval;
    QLabel* textLabel3;
    QSpinBox* spinPredelay;
    QSpinBox* spinRandom;
    QSpinBox* spinNumber;
    QLabel* textLabel2;
    QLabel* textLabel4;
    QPushButton* tapButton;
    QPushButton* startButton;
    QPushButton* stopButton;
    QLabel* textLabel1;

public slots:
    virtual void tapButton_clicked();
    virtual void startButton_clicked();
    virtual void stopButton_clicked();
    virtual void timerDone();

protected:

protected slots:
    virtual void languageChange();
};

/* ------------------------------------------------------------------------- */

#endif

/* ------------------------------------------------------------------------- */

