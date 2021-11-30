#include "tcpserver.h"
#include "ui_tcpserver.h"

#define TIMEMS qPrintable(QTime::currentTime().toString("HH:mm:ss zzz"))


TcpServer::TcpServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    initForm();
}

TcpServer::~TcpServer()
{
    delete ui;
}

//1. UI
//2. 初始化
//  ① 创建QTcpSever对象
//  ② 建立信号槽，有客户端连接时，触发newConnection信号，代表连接服务器成功
void TcpServer::initForm() {
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

//3. 侦听端口，使得客户端能用此端口访问服务器 (server->listen)
void TcpServer::on_btnListen_clicked()
{
    if (ui->btnListen->text() == "监听") {
        if (server->listen(QHostAddress::Any , 8888)) {
            ui->txtMain->setTextColor(QColor("darkgreen"));
            ui->txtMain->append("监听成功");
            ui->btnListen->setText("关闭");
        } else {
            ui->txtMain->append("监听失败");
        }

    } else {
        socket->abort();
        server->close();
        ui->btnListen->setText("监听");
    }
}

//4. 客户端连接
//  ① 连接成功触发newConnection信号，
//  ② 服务器通过socket 获取客户端连接信息
//  ③ 连接信号槽，接收到客户端数据触发

void TcpServer::newConnection()
{
    //解析所有客户连接
    while (server->hasPendingConnections())
    {
        //连接上后通过socket获取连接信息
        socket = server->nextPendingConnection();
        QString str = QString("[ip:%1,port:%2]").arg(socket->peerAddress().toString()).arg(socket->peerPort());
        //提示连接成功
        ui->txtMain->append(str + " Connect to the server");
        //复选框选项为连接服务器的ip
        ui->cboxListenIP->addItem(str);
        //监听客户端是否有消息发送
        connect(socket, &QTcpSocket::readyRead, this, &TcpServer::readData);
    }
}

//5.1 接收客户端数据 (socket->readAll())
void TcpServer::readData() {

    QByteArray data = socket->readAll();
    if (data.length() <= 0)     return;

    ui->txtMain->setTextColor(QColor("red"));
    QString strData = "Receive: " + data;
    ui->txtMain->append(strData);
}

//5.2 发送到客户端数据 (socket->readAll())
void TcpServer::on_btnSend_clicked()
{
    QString data = ui->cboxData->currentText();
    if (data.length() <= 0)     return;

    QByteArray buffer = data.toUtf8();
    socket->write(buffer);

    QString strData = "Send: " + buffer;
    ui->txtMain->setTextColor(QColor("darkgreen"));
    ui->txtMain->append(strData);
}
