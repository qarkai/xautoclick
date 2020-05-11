#ifndef __CLICKWIDGET_H__
#define __CLICKWIDGET_H__

#include <QWidget>

class QPushButton;
class QSpinBox;

class ClickWidget : public QWidget {
    Q_OBJECT

    public:
        ClickWidget(QWidget *parent = 0);
        int getSpinValue(int spin);
        void setSpinValue(int spin, int value);
        void setButtonSensitive(int button, bool state);

    private:
        QPushButton *buttons[3];
        QSpinBox *spins[4];
};

#endif /* __CLICKWIDGET_H__ */
