#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSerialPort>        //提供访问串口的功能
#include <QSerialPortInfo>    //提供系统中存在的串口的信息

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString byteArrayToHexStr(const QByteArray &data);
    void configSetEnable(bool b);
    void initConfig();


private slots:
    void serial_readyRead();

    //updata port number
    void updataPortNum();

    void on_pushButton_operate_clicked();

    void on_pushButton_send_clicked();

    void on_pushButton_clearRcv_clicked();

    void on_pushButton_clearSend_clicked();

private:
    Ui::MainWindow *ui;


    QSerialPort *serial;

    bool isSerialOpen;

};
#endif // MAINWINDOW_H
