#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

class MyWidget : public QWidget {
    Q_OBJECT

    public:
        MyWidget(QWidget *parent = 0);
        QPushButton *buttons[3];
        QSpinBox *spins[4];
        QTimer *timer;

    public slots:
        void tap(void);
        void stop(void);
        void start(void);
        void timer_done(void);
};

