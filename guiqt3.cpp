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

extern "C" {
#include "main.h"
#include <stdio.h>
}

#include "guiqt3.h"

#include <qapplication.h>
#include <qvariant.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qaction.h>
#include <qtimer.h>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

#undef QT_NO_COMPAT
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
//#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
//#error "This file was generated using the moc from 3.1.1. It"
//#error "cannot be used with the include files from this version of Qt."
//#error "(The moc has changed too much.)"
//#endif

/* ------------------------------------------------------------------------- */

static Display *display;
static qtautoclick *mainwindow;
static QApplication *a;

/* ------------------------------------------------------------------------- */

void set_button_sensitive(button_t button, int state) {

    switch(button) {
    case BUTTON_TAP:
        mainwindow->tapButton->setEnabled(state ? TRUE : FALSE);
        break;
    case BUTTON_STOP:
        mainwindow->stopButton->setEnabled(state ? TRUE : FALSE);
        break;
    case BUTTON_START:
        mainwindow->startButton->setEnabled(state ? TRUE : FALSE);
        break;
    }
}

/* ------------------------------------------------------------------------- */

void set_spin_value(spin_t spin, int value) {

    switch(spin) {
    case SPIN_PREDELAY:
        mainwindow->spinPredelay->setValue(value);
        break;
    case SPIN_INTERVAL:
        mainwindow->spinInterval->setValue(value);
        break;
    case SPIN_RANDOM:
        mainwindow->spinRandom->setValue(value);
        break;
    case SPIN_NUMBER:
        mainwindow->spinNumber->setValue(value);
        break;
    }
}

/* ------------------------------------------------------------------------- */

int get_spin_value(spin_t spin) {

    switch(spin) {
    case SPIN_PREDELAY:
        return mainwindow->spinPredelay->value();
        break;
    case SPIN_INTERVAL:
        return mainwindow->spinInterval->value();
        break;
    case SPIN_RANDOM:
        return mainwindow->spinRandom->value();
        break;
    case SPIN_NUMBER:
        return mainwindow->spinNumber->value();
        break;
    }

    return 0;
}

/* ------------------------------------------------------------------------- */

void qtautoclick::timerDone(void) {

    common_alarm_callback();
}

/* ------------------------------------------------------------------------- */

void set_alarm(int ms) {
    static QTimer *timer = NULL;

    if (!timer) {
        timer = new QTimer(mainwindow);
        mainwindow->connect(timer, SIGNAL(timeout()), mainwindow, SLOT(timerDone()) );
    }

    timer->start(ms, TRUE);
}

/* ------------------------------------------------------------------------- */

void click_mouse_button(void) {

    XTestFakeButtonEvent(display, 1, True, CurrentTime);
    XTestFakeButtonEvent(display, 1, False, CurrentTime);
    XFlush(display);
}

/* ------------------------------------------------------------------------- */

void qtautoclick::tapButton_clicked() {
    common_tap_button();
}

/* ------------------------------------------------------------------------- */

void qtautoclick::startButton_clicked() {
    common_start_button();
}

/* ------------------------------------------------------------------------- */

void qtautoclick::stopButton_clicked() {
    common_stop_button();
}

/* ------------------------------------------------------------------------- */

const char *qtautoclick::className() const {
    return "qtautoclick";
}

/* ------------------------------------------------------------------------- */

QMetaObject *qtautoclick::metaObj = 0;
static QMetaObjectCleanUp cleanUp_qtautoclick( "qtautoclick", &qtautoclick::staticMetaObject );

/* ------------------------------------------------------------------------- */

#ifndef QT_NO_TRANSLATION
QString qtautoclick::tr( const char *s, const char *c ) {
    if ( qApp )
        return qApp->translate( "qtautoclick", s, c, QApplication::DefaultCodec );
    else
        return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString qtautoclick::trUtf8( const char *s, const char *c ) {
    if ( qApp )
        return qApp->translate( "qtautoclick", s, c, QApplication::UnicodeUTF8 );
    else
        return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

/* ------------------------------------------------------------------------- */

QMetaObject* qtautoclick::staticMetaObject() {
    if ( metaObj )
        return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"tapButton_clicked", 0, 0 };
    static const QUMethod slot_1 = {"startButton_clicked", 0, 0 };
    static const QUMethod slot_2 = {"stopButton_clicked", 0, 0 };
    static const QUMethod slot_3 = {"languageChange", 0, 0 };
    static const QUMethod slot_4 = {"timerDone", 0, 0 };
    static const QMetaData slot_tbl[] = {
        { "tapButton_clicked()", &slot_0, QMetaData::Public },
        { "startButton_clicked()", &slot_1, QMetaData::Public },
        { "stopButton_clicked()", &slot_2, QMetaData::Public },
        { "languageChange()", &slot_3, QMetaData::Protected },
        { "timerDone()", &slot_4, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
        "qtautoclick", parentObject,
        slot_tbl, 5,
        0, 0,
#ifndef QT_NO_PROPERTIES
        0, 0,
        0, 0,
#endif // QT_NO_PROPERTIES
        0, 0 );
    cleanUp_qtautoclick.setMetaObject( metaObj );
    return metaObj;
}

/* ------------------------------------------------------------------------- */

void* qtautoclick::qt_cast( const char* clname ) {
    if ( !qstrcmp( clname, "qtautoclick" ) )
        return this;
    return QMainWindow::qt_cast( clname );
}

/* ------------------------------------------------------------------------- */

bool qtautoclick::qt_invoke( int _id, QUObject* _o ) {
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: tapButton_clicked(); break;
    case 1: startButton_clicked(); break;
    case 2: stopButton_clicked(); break;
    case 3: languageChange(); break;
    case 4: timerDone(); break;
    default:
        return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

/* ------------------------------------------------------------------------- */

bool qtautoclick::qt_emit( int _id, QUObject* _o ) {
    return QMainWindow::qt_emit(_id,_o);
}

/* ------------------------------------------------------------------------- */

#ifndef QT_NO_PROPERTIES
bool qtautoclick::qt_property( int id, int f, QVariant* v) {
    return QMainWindow::qt_property( id, f, v);
}

bool qtautoclick::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


/* ------------------------------------------------------------------------- */

qtautoclick::qtautoclick( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl ) {
    (void)statusBar();
    if (!name) setName( "qtautoclick" );
    setEnabled(TRUE);
    setMinimumSize(QSize(32, 32));
    setMaximumSize(QSize(16384, 16384));
    setCentralWidget(new QWidget(this, "qt_central_widget"));

    spinInterval = new QSpinBox(centralWidget(), "spinInterval");
    spinInterval->setGeometry(QRect(85, 30, 55, 23));
    spinInterval->setMaxValue(1<<30);
    spinInterval->setMinValue(1);
    spinInterval->setValue(1000);

    textLabel3 = new QLabel( centralWidget(), "textLabel3" );
    textLabel3->setGeometry( QRect( 5, 60, 80, 20 ) );

    spinPredelay = new QSpinBox( centralWidget(), "spinPredelay" );
    spinPredelay->setGeometry( QRect( 85, 5, 55, 23 ) );
    spinPredelay->setMaxValue( 1<<30 );
    spinPredelay->setMinValue( 1 );
    spinPredelay->setValue( 2000 );

    spinRandom = new QSpinBox( centralWidget(), "spinRandom" );
    spinRandom->setGeometry( QRect( 85, 55, 55, 23 ) );
    spinRandom->setMaxValue( 10240 );
    spinRandom->setMinValue( 1 );
    spinRandom->setValue( 64 );

    spinNumber = new QSpinBox( centralWidget(), "spinNumber" );
    spinNumber->setGeometry( QRect( 85, 80, 55, 23 ) );
    spinNumber->setMaxValue( 10240 );
    spinNumber->setMinValue( 1 );
    spinNumber->setValue( 32 );

    textLabel2 = new QLabel( centralWidget(), "textLabel2" );
    textLabel2->setGeometry( QRect( 5, 35, 80, 20 ) );

    textLabel4 = new QLabel( centralWidget(), "textLabel4" );
    textLabel4->setGeometry( QRect( 5, 85, 80, 20 ) );

    tapButton = new QPushButton( centralWidget(), "tapButton" );
    tapButton->setGeometry( QRect( 0, 110, 40, 30 ) );

    startButton = new QPushButton( centralWidget(), "startButton" );
    startButton->setGeometry( QRect( 90, 110, 50, 30 ) );

    stopButton = new QPushButton( centralWidget(), "stopButton" );
    stopButton->setEnabled( FALSE );
    stopButton->setGeometry( QRect( 40, 110, 50, 30 ) );

    textLabel1 = new QLabel( centralWidget(), "textLabel1" );
    textLabel1->setGeometry( QRect( 5, 10, 80, 20 ) );

    languageChange();
    resize(QSize(144, 170).expandedTo(minimumSizeHint()));

    connect(tapButton, SIGNAL(clicked()), this, SLOT(tapButton_clicked()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stopButton_clicked()));
    connect(startButton, SIGNAL(clicked()), this, SLOT(startButton_clicked()));
}

/* ------------------------------------------------------------------------- */

qtautoclick::~qtautoclick() {
    // Qt deletes all childs
}

/* ------------------------------------------------------------------------- */

void qtautoclick::languageChange() {
    setCaption( tr( "qtAutoClick" ) );
    setIconText( tr( "qtautoclick" ) );
    textLabel3->setText( tr( "Random +/-" ) );
    textLabel2->setText( tr( "Interval" ) );
    textLabel4->setText( tr( "# of clicks" ) );
    tapButton->setText( tr( "Tap" ) );
    startButton->setText( tr( "Start" ) );
    stopButton->setText( tr( "Stop" ) );
    textLabel1->setText( tr( "Pre-delay" ) );
}

/* ------------------------------------------------------------------------- */

int init_gui(int argc, char **argv) {
    display = XOpenDisplay(NULL);

    if (!display) {
        fprintf(stderr, "Unable to open X display\n");
        return 0;
    }

    a = new QApplication (argc, argv);
    mainwindow = new qtautoclick;
    mainwindow->show();
    a->connect( a, SIGNAL( lastWindowClosed() ), a, SLOT( quit() ) );
    return 1;
}

/* ------------------------------------------------------------------------- */

void close_gui(void) {
    XCloseDisplay(display);
}

/* ------------------------------------------------------------------------- */

void main_loop(void) {
    a->exec();
}

/* ------------------------------------------------------------------------- */

