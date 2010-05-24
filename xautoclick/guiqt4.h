#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class MyWidget : public QWidget {
    Q_OBJECT

    public:
        MyWidget(QWidget *parent = 0);
        QPushButton *buttons[3];
        QSpinBox *spins[4];

    public slots:
        void tap(void);
        void stop(void);
        void start(void);
};

