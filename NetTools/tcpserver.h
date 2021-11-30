#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include <QtNetwork>

namespace Ui {
class TcpServer;
}

class TcpServer : public QWidget
{
    Q_OBJECT

public:
    explicit TcpServer(QWidget *parent = nullptr);
    ~TcpServer();
    void initForm();

private:
    Ui::TcpServer *ui;
    QTcpServer *server;
    QTcpSocket *socket;

private slots:
    void newConnection();
    void readData();

private slots:
    void on_btnListen_clicked();
    void on_btnSend_clicked();
};

#endif // TCPSERVER_H
