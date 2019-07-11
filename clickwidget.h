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
        QPushButton *buttons[3];
        QSpinBox *spins[4];
        QTimer *timer;

    public slots:
        void tap(void);
        void stop(void);
        void start(void);
        void timer_done(void);
};

#endif /* __CLICKWIDGET_H__ */
