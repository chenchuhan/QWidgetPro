#include "udpserver.h"
#include "ui_udpserver.h"

UdpServer::UdpServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UdpServer)
{
    ui->setupUi(this);
    initForm();
}

UdpServer::~UdpServer()
{
    delete ui;
}

//1. ui
//2. 初始化
//① 创建 QUdpSocket 套接字对象
//② 建立socket 接收缓冲数据的信号槽（readyRead）
void UdpServer::initForm()
{
    socket = new QUdpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
}

//3.开启监听, IP不指定,端口自定义
void UdpServer::on_btnListen_clicked()
{
    UdpServerPort = ui->txtListenPort->text().trimmed().toInt();
    if (ui->btnListen->text() == "监听") {
        if (socket->bind(QHostAddress::AnyIPv4, UdpServerPort)) {
            ui->btnListen->setText("关闭");
        }
    } else {
        socket->abort();
        ui->btnListen->setText("监听");
    }
}

//4. 接收UDP客户端数据，获取UDP客户端IP和端口号，并保存到 listWidget 中
void UdpServer::readData()
{
    QHostAddress host;
    quint16 port;
    QByteArray data;

    while (socket->hasPendingDatagrams()) {
        data.resize(socket->pendingDatagramSize());
        socket->readDatagram(data.data(), data.size(), &host, &port);

        //ip
        QString ip = host.toString();
        ip = ip.replace("::ffff:", "");
        if (ip.isEmpty()) {
            continue;
        }

        QString str = QString("[%1:%2] %3").arg(ip).arg(port).arg(QString(data));
        ui->txtMain->setTextColor(QColor("red"));
        ui->txtMain->append(str);

        //添加到 listWidget 列表中
        QString listData = QString("%1:%2").arg(ip).arg(port);
        for (int i = 0; i < ui->listWidget->count(); i++) {
            QString s = ui->listWidget->item(i)->text();
            if (listData == s) {
                return;
            }
        }
        ui->listWidget->addItem(listData);
        ui->labCount->setText(QString("%1个客户端").arg(ui->listWidget->count()));
    }
}

//5. 发送数据到UDP客户端
//   利用已保存的客户端IP和端口号发送
void UdpServer::on_btnSend_clicked()
{
    QString data = ui->cboxData->currentText();
    if (data.length() <= 0) {
        return;
    }

    int row = ui->listWidget->currentRow();
    if (row >= 0) {
        QString str = ui->listWidget->item(row)->text();
        QStringList list = str.split(":");
        QString ip = list.at(0);
        quint16 port = list.at(1).toInt();

        socket->writeDatagram(data.toUtf8(), QHostAddress(ip), port);

        QString strPrint = QString("[%1:%2] %3").arg(ip).arg(port).arg(data);
        ui->txtMain->setTextColor(QColor("darkgreen"));
        ui->txtMain->append(strPrint);
    }
}

