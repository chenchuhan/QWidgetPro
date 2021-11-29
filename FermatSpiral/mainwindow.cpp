#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("费马曲线");
    setupfermatSpiralDemo(ui->customPlot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupfermatSpiralDemo(QCustomPlot *customPlot)
{
  // create empty curve objects:
  QCPCurve *fermatSpiral = new QCPCurve(customPlot->xAxis, customPlot->yAxis);

  // generate the curve data points:
  const int pointCount = 500;
  QVector<QCPCurveData> dataSpiral(pointCount),dataSpiral1(pointCount);

  double count =    7.5;        //圈数
  double disInner = 1;          //内径
  double interval = 1;          //圈距

  for (int i=0; i<pointCount; ++i)
  {
    double phi = i/static_cast<double>((pointCount-1));
    double dis = count * interval * phi;                //外径 - 内径
    dataSpiral[i] = QCPCurveData(i, (disInner + dis)*qCos(count*phi*2*M_PI), (disInner + dis)*qSin(count*phi*2*M_PI));

  }
  // pass the data to the curves; we know t (i in loop above) is ascending, so set alreadySorted=true (saves an extra internal sort):
  fermatSpiral->data()->set(dataSpiral, true);

  customPlot->axisRect()->setupFullAxesBox();
  customPlot->rescaleAxes();
}

