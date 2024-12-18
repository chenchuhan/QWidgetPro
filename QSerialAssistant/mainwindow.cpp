#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isSerialOpen(false)
{
    ui->setupUi(this);

    //初始化配置
    initConfig();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConfig() {

    //创建对象，并建立信号槽
    serial = new QSerialPort(this);
    //读函数的信号槽， 具体见博客
    QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::serial_readyRead);
    //端口号下拉框升级后的点击事件
    QObject::connect(ui->comboBox_port, SIGNAL(clicked()), this, SLOT(updataPortNum()));

    //添加端口号
    updataPortNum();
    ui->comboBox_port->setCurrentIndex(0);

    //添加波特率
    QStringList baudList;
    baudList << "115200" << "57600" << "9600" ;
    ui->comboBox_baud->addItems(baudList);
    ui->comboBox_baud->setCurrentText("115200");

    //添加停止位
    QStringList stopBitsList;
    stopBitsList << "1" << "1.5" << "2";
    ui->comboBox_stop->addItems(stopBitsList);
    ui->comboBox_stop->setCurrentText("1");

    //添加数据位
    QStringList dataBitsList;
    dataBitsList << "8" << "7" << "6";
    ui->comboBox_data->addItems(dataBitsList);
    ui->comboBox_data->setCurrentText("8");

    //添加校验位
    QStringList checkList;
    checkList << "NO" << "EVEN"<< "ODD" ;
    ui->comboBox_check->addItems(checkList);
    ui->comboBox_check->setCurrentText("NO");

    ui->pushButton_operate->setText("OPEN");
    ui->textEdit_send->setText("123456789\r\n");
}

//--槽函数，点击端口下拉框的时候更新
void MainWindow:: updataPortNum(void) {

    //清除串口号
    ui->comboBox_port->clear();

    //遍历 QSerialPortInfo, 添加到串口下拉框中
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        ui->comboBox_port->addItem(info.portName());
    }
}

QString MainWindow::byteArrayToHexStr(const QByteArray &data)
{
    QString temp = "";
    QString hex = data.toHex();
    for (int i = 0; i < hex.length(); i = i + 2) {
        temp += hex.mid(i, 2) + " ";
    }

    return temp.trimmed().toUpper();
}

//
void MainWindow::serial_readyRead()
{
    //从界面中读取以前收到的数据
    QString recv = ui->textEdit_rcv->toPlainText();

    //从接收缓冲区中读取数据
    QByteArray buffer = serial->readAll();

    //hex 显示
    QString str = byteArrayToHexStr(buffer);

    //界面中读取的数据中加入刚读取的数据
    recv += QString(str);

    //清除显示
    ui->textEdit_rcv->clear();

    //更新显示
    ui->textEdit_rcv->append(recv);
}


void MainWindow::configSetEnable(bool b)
{
    ui->comboBox_port->setEnabled(b);
    ui->comboBox_baud->setEnabled(b);
    ui->comboBox_stop->setEnabled(b);
    ui->comboBox_data->setEnabled(b);
    ui->comboBox_check->setEnabled(b);

    //
    ui->pushButton_send->setEnabled(!b);
}

void MainWindow::on_pushButton_operate_clicked()
{
    if (ui->pushButton_operate->text() == QString("OPEN")) {

        const QString portnameStr = ui->comboBox_port->currentText();

        QSerialPortInfo info(portnameStr);
        if(info.isBusy()){
            qDebug()<< "The serial port is occupied" <<portnameStr;
            return;
        }

        ui->pushButton_operate->setText("CLOSE");
        //清空缓冲区
        serial->flush();
        //设置端口号
        serial->setPortName(portnameStr);
        //设置波特率
        serial->setBaudRate( static_cast<QSerialPort::BaudRate> (ui->comboBox_baud->currentText().toInt()) );
        //设置停止位
        serial->setStopBits( static_cast<QSerialPort::StopBits> (ui->comboBox_stop->currentText().toInt()));
        //设置数据位
        serial->setDataBits( static_cast<QSerialPort::DataBits> (ui->comboBox_data->currentText().toInt()) );
        //设置校验
        serial->setParity  ( static_cast<QSerialPort::Parity>   (ui->comboBox_check->currentIndex()));
        //设置流控
        serial->setFlowControl(QSerialPort::NoFlowControl);

        isSerialOpen = serial->open(QIODevice::ReadWrite);
        if (!isSerialOpen) {
            qDebug()<< QString("Failed to open serial port:") << portnameStr << serial->errorString();
            serial->clearError();
            configSetEnable(true);
        }
        else {
            qDebug()<< QString("The serial port is open1: %1").arg(portnameStr);
            qDebug()<< QString("The serial port is open2: %1").arg(serial->portName());
            configSetEnable(false);
        }

        qDebug()<< QString("isOpen:%1").arg(serial->isOpen());

    }
    else {
         ui->pushButton_operate->setText("OPEN");
         serial->close();
         configSetEnable(true);
    }
}

//
void MainWindow::on_pushButton_send_clicked()
{
    //简单文本框用 toPlainText() 取文本框的内容 toUtf8 是转换成utf8格式的字节流
    QByteArray data = ui->textEdit_send->toPlainText().toUtf8();
    serial->write(data);
}

void MainWindow::on_pushButton_clearRcv_clicked()
{
    ui->textEdit_rcv->clear();
}

void MainWindow::on_pushButton_clearSend_clicked()
{
    ui->textEdit_send->clear();
}
