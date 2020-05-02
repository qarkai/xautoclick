#ifndef __CLICKWIDGET_H__
#define __CLICKWIDGET_H__

#include <QPushButton>
#include <QSpinBox>
#include <QWidget>

class ClickWidget : public QWidget {
    Q_OBJECT

    public:
        ClickWidget(QWidget *parent = 0);
        int getSpinValue(int spin);
        void setSpinValue(int spin, int value);
        void setButtonSensitive(int button, bool state);

    public slots:
        void tap(void);
        void stop(void);
        void start(void);

    private:
        QPushButton *buttons[3];
        QSpinBox *spins[4];
};

#endif /* __CLICKWIDGET_H__ */
