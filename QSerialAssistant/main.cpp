#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //…Ë÷√√˚◊÷£¨∞Ê±æ
    w.setWindowTitle("SerialAssistant_v0.1--2021.08.05--QWidget");

    w.show();

    return a.exec();
}
