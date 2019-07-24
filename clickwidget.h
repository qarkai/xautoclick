#ifndef __CLICKWIDGET_H__
#define __CLICKWIDGET_H__

#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <QWidget>

class ClickWidget : public QWidget {
    Q_OBJECT

    public:
        ClickWidget(QWidget *parent = 0);
        void startTimer(int ms);
        int getSpinValue(int spin);
        void setSpinValue(int spin, int value);
        void setButtonSensitive(int button, int state);

    public slots:
        void tap(void);
        void stop(void);
        void start(void);
        void timer_done(void);

    private:
        QPushButton *buttons[3];
        QSpinBox *spins[4];
        QTimer *timer;
};

#endif /* __CLICKWIDGET_H__ */
