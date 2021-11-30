#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QtNetwork>

namespace Ui {
class TcpClient;
}

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    TcpClient(QWidget *parent = nullptr);
    ~TcpClient();
   void initForm();

private slots:
    void readData();

private slots:
    void on_btnConnect_clicked();
    void on_btnSend_clicked();

private:
    Ui::TcpClient *ui;
    QTcpSocket *socket;

    QString TcpServerIP;         //服务器IP
    int TcpServerPort;

};
#endif // TCPCLIENT_H
