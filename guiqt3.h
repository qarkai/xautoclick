#ifndef __GUIQT3_H__
#define __GUIQT3_H__

#include <qvariant.h>
#include <qmainwindow.h>

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

#endif
