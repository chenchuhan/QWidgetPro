#ifndef UDPCLINET_H
#define UDPCLINET_H

#include <QWidget>
#include <QtNetwork>

namespace Ui {
class UdpClinet;
}

class UdpClinet : public QWidget
{
    Q_OBJECT

public:
    explicit UdpClinet(QWidget *parent = nullptr);
    void initForm();
    ~UdpClinet();

private:
    Ui::UdpClinet *ui;
    QUdpSocket *socket;

    QString UdpServerIP;
    int UdpServerPort;

private slots:
    void readData();

private slots:
    void on_btnSend_clicked();
};

#endif // UDPCLINET_H
