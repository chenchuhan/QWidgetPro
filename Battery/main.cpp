#include "battery.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Battery w;

    w.setWindowTitle("Battery");

    w.show();
    return a.exec();
}
