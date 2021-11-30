#include "tcpclient.h"
#include "ui_tcpclient.h"

TcpClient::TcpClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    initForm();
}

TcpClient::~TcpClient()
{
    delete ui;
}

//1 ui

//2. 初始化
//① 创建 QTcpSocket 套接字对象
//② 建立socket 接收缓冲数据的信号槽（readyRead）
void TcpClient::initForm()
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
}


//3. 连接服务器 (socket->connectToHost)
//  在连接按键中，获取手动输入的服务器IP地址和端口号
void TcpClient::on_btnConnect_clicked()
{
    TcpServerIP = ui->txtServerIP->text().trimmed();
    TcpServerPort = ui->txtServerPort->text().trimmed().toInt();

    if (ui->btnConnect->text() == "连接") {
        socket->abort();
        socket->connectToHost(TcpServerIP, static_cast<quint16>(TcpServerPort));
        ui->btnConnect->setText("断开");
    } else {
        socket->abort();
        ui->btnConnect->setText("连接");
    }
}

//4.1 接收服务器数据 (socket->readAll())
void TcpClient::readData() {
    QByteArray data = socket->readAll();
    if (data.length() <= 0)     return;

    QString strData = "[接收]  " + TcpServerIP + " " + QString::number(TcpServerPort) + " " + data + "";
    ui->txtMain->setTextColor(QColor("red"));
    ui->txtMain->append(strData);
}

// 4.2发送数据到服务器
void TcpClient::on_btnSend_clicked()
{
    QString data = ui->cboxData->currentText();
    if (data.length() <= 0)                 return;

    QByteArray buffer = data.toUtf8();
    socket->write(buffer);

    QString strData = "[发送]  " + TcpServerIP + " " + QString::number(TcpServerPort) + " " + buffer + "";
    ui->txtMain->setTextColor(QColor("darkgreen"));
    ui->txtMain->append(strData);
}

