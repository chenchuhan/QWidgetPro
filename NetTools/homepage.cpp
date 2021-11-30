#include "homepage.h"
#include "ui_homepage.h"

#include "tcpclient.h"
#include "tcpserver.h"
#include "udpclinet.h"
#include "udpserver.h"

homePage::homePage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::homePage)
{
    ui->setupUi(this);
    this->initForm();
}

void homePage::initForm()
{
    ui->tabWidget->addTab(new TcpClient, "TCP Client");
    ui->tabWidget->addTab(new TcpServer, "TCP Server");
    ui->tabWidget->addTab(new UdpClinet, "UDP Client");
    ui->tabWidget->addTab(new UdpServer, "UDP Server");
}

homePage::~homePage()
{
    delete ui;
}

