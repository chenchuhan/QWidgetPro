#pragma execution_character_set("utf-8")

#include "battery.h"
#include "qpainter.h"
#include "qtimer.h"
#include "qdebug.h"
#include "ui_battery.h"

Battery::Battery(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Battery)
    , _currentValue(10)
    , _margin(3)
    , _minValue(0)
    , _maxValue(100)
    , _isForward(true)
    , _batteryWidth(50)
    , _batteryHeight(20)

{
//    ui->setupUi(this);       //驱动UI设计师
//    setFixedSize(300, 180);  //固定大小
    setBaseSize(350, 180);

    ///--设置了个定时器，周期 10ms ，输入值 inputValue()
    inputTimer = new QTimer(this);
    inputTimer->setInterval(100);
    connect(inputTimer, SIGNAL(timeout()), this, SLOT(inputValue()));
    inputTimer->start();
}

Battery::~Battery()
{
    if (inputTimer->isActive()) {
        inputTimer->stop();
    }
    delete ui;
}

///--1. 绘制事件
/*只要窗口部件需要被重绘就被调用, 每个要显示输出的窗口部件必须实现它。
这个事件处理器可以在子类中被重新实现来接收绘制事件。 它可以是repaint()或update()的结果。
很多窗口部件在当它们被请求时，它们很简单地重新绘制整个界面，但是一些窗口部件通过仅仅绘制被请求的区域QPaintEvent::region()进行优化，例如，QListView和QCanvas就是这样做的。
原文链接：https://blog.csdn.net/u012151242/article/details/78947024
*/
void Battery::paintEvent(QPaintEvent *)
{
    //绘制准备工作,启用反锯齿
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    ///--1.1 绘制边框和头部
    drawBorder(&painter);
    ///--1.2 绘制填充
    drawBg(&painter);
    ///--1.3 内部百分比
    drawText(&painter);
}

///--1.1 绘制边框和头部
void Battery::drawBorder(QPainter *painter)
{
    //保存状态
    painter->save();

    //设置笔的颜色和粗细
    painter->setPen(QPen(Qt::gray, 5));
    //没有画刷填充
    painter->setBrush(Qt::NoBrush);
    //绘制给定的圆角矩形              矩形        xRadius       yRadius
//    painter->drawRoundedRect(batteryRect, borderRadius, borderRadius);

    //电池边框居中
    batteryRect = QRectF((width()-_batteryWidth)/2, (height()-_batteryHeight)/2, _batteryWidth, _batteryHeight);
    painter->drawRoundedRect(batteryRect, 2, 2);

    //电池头部：画一条直线
    painter->setPen(QPen(Qt::gray, 5));
    QLineF line(batteryRect.topRight().x()+5, batteryRect.topRight().y()+5, batteryRect.topRight().x()+5, batteryRect.bottomRight().y()-5);
    painter->drawLine(line);

    //回复保存的状态
    painter->restore();
}

///--1.2 绘制填充
void Battery::drawBg(QPainter *painter)
{
    painter->save();

    //确定画刷颜色
    if(_currentValue<=10) {
        painter->setBrush(QColor(204, 38, 38));  //红
    }
    else if (_currentValue <= 20) {
        painter->setBrush(QColor(198, 163, 0));  //黄
    }
    else {
        painter->setBrush(QColor(50, 205, 51));  //绿
    }

    //当前电量转化为宽
    double width = _currentValue * (batteryRect.width() - (_margin * 2)) / 100;

    //确定左上角位置
    QPointF topLeft(batteryRect.topLeft().x() + _margin, batteryRect.topLeft().y() + _margin);
    //确定变化的右下角, 最小给个10, 显示内部的填充
    QPointF bottomRight(batteryRect.topLeft().x() + width + _margin, batteryRect.bottomRight().y() - _margin);
    QRectF rect(topLeft, bottomRight);

    //没有线宽
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(rect, 5, 5);
    painter->restore();
}

///--1.3 内部百分比
void Battery::drawText(QPainter *painter) {

    painter->save();

    painter->setPen(Qt::black);
    painter->setFont(QFont("Arial",11));

    QString value = QString::number(_currentValue) + "%";

    //文本居中的好方法
    painter->drawText(batteryRect,Qt::AlignCenter,value);

    painter->restore();
}

//1. 驱动、输入
void Battery::inputValue()
{
    if(_isForward)      _currentValue += 1;
    else                _currentValue -= 1;

    if(_currentValue >= 45)  {
        _currentValue = 45;
        _isForward = false;
    }
    if(_currentValue <= _minValue) {
        _currentValue = _minValue;
        _isForward = true;
    }
    //重绘，将调用paintEvent()函数
    this->update();
}


///---设置画刷, 渐变的颜色
//    QLinearGradient batteryGradient(QPointF(0, 0), QPointF(0, height()));
//    if (currentValue <= alarmValue) {
//        batteryGradient.setColorAt(0.0, alarmColorStart);
//        batteryGradient.setColorAt(1.0, alarmColorEnd);
//    } else {
//        batteryGradient.setColorAt(0.0, normalColorStart);
//        batteryGradient.setColorAt(1.0, normalColorEnd);
//    }
//    painter->setBrush(batteryGradient);  //画刷填充
