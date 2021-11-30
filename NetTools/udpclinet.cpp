#include "udpclinet.h"
#include "ui_udpclinet.h"

UdpClinet::UdpClinet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UdpClinet)
{
    ui->setupUi(this);
    initForm();
}

UdpClinet::~UdpClinet()
{
    delete ui;
}

//1. UI
//2. 初始化
//① 创建 QTcpSocket 套接字对象
//② 建立socket 接收缓冲数据的信号槽（readyRead）
void UdpClinet::initForm()
{
    socket = new QUdpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
}

//3. 接收UDP数据
void UdpClinet::readData()
{
    QHostAddress host;
    quint16 port;
    QByteArray data;

    while (socket->hasPendingDatagrams()) {
        data.resize(socket->pendingDatagramSize());
        socket->readDatagram(data.data(), data.size(), &host, &port);
        QString str = QString("[%1:%2] %3").arg(host.toString()).arg(port).arg(QString(data));

        ui->txtMain->setTextColor(QColor("red"));
        ui->txtMain->append(str);
    }
}

//4. 发送UDP数据 (socket->connectToHost)
//  在连接按键中，获取手动输入的服务器IP地址和端口号
void UdpClinet::on_btnSend_clicked()
{
    UdpServerIP = ui->txtServerIP->text().trimmed();
    UdpServerPort = ui->txtServerPort->text().trimmed().toInt();

    QString data = ui->cboxData->currentText();
    if (data.length() <= 0) {
        return;
    }

    QByteArray buffer = data.toUtf8();

    socket->writeDatagram(buffer, QHostAddress(UdpServerIP), UdpServerPort);

    QString str = QString("[发送：%1:%2] %3").arg(UdpServerIP).arg(UdpServerPort).arg(data);

    ui->txtMain->setTextColor(QColor("darkgreen"));
    ui->txtMain->append(str);
}
