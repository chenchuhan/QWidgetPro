#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QWidget>
#include <QtNetwork>

namespace Ui {
class UdpServer;
}

class UdpServer : public QWidget
{
    Q_OBJECT

public:
    explicit UdpServer(QWidget *parent = nullptr);
    ~UdpServer();
    void initForm();

private:
    Ui::UdpServer *ui;
    QUdpSocket *socket;
    int UdpServerPort;

private slots:
    void readData();

private slots:
    void on_btnListen_clicked();
    void on_btnSend_clicked();
};

#endif // UDPSERVER_H
