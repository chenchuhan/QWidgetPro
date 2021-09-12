#ifndef BATTERY_H
#define BATTERY_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Battery; }
QT_END_NAMESPACE

class Battery : public QMainWindow

{
    Q_OBJECT

public:
    Battery(QWidget *parent = nullptr);
    ~Battery();

protected:
    void paintEvent(QPaintEvent *);
    void drawBorder(QPainter *painter);
    void drawBg(QPainter *painter);
    void drawText(QPainter *painter);

private slots:
    void inputValue();

private:
    Ui::Battery *ui;
    double _currentValue;
    int _margin;
    double _minValue;                //最小值
    double _maxValue;                //最大值
    bool _isForward;                 //是否往前移
    int _batteryWidth;
    int _batteryHeight;

    QRectF batteryRect;             //电池主体区域
    QTimer *inputTimer;                  //绘制定时器
};

#endif // BATTERY_H
