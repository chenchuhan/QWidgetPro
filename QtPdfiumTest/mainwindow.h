#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFileDialog>
#include <QDesktopServices>
#include <QTime>
#include <QImage>
#include <QPixmap>
#include <QWheelEvent>
#include <QDebug>

#include "qpdfium.h"


#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <QFileDialog>

//Item
#include <QWheelEvent>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QRadioButton>
#include <QPrinter>
#include <QButtonGroup>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initWidget();  //         初始化参数

    void showPdfData(int curPage);
    void updateScaledPixmap();

protected:
    void wheelEvent(QWheelEvent *event) override;

private slots:
    void goToPreviousPage();       // 上一页槽函数
    void goToNextPage();           // 下一页槽函数

    void loadPdf();
    void printPdf();

private:
    Ui::MainWindow *ui;

    int m_curPage;                  //当前页
    int m_totalPage;                //总页数
    double  zoomScale;              //缩放比例
    bool    isPdfValid = false;     //打开pdf是否有效
    QString m_saveImagePath;        //图像保存路径
    QPixmap _showPixmap;           //界面显示图像

    //Item1
    QPushButton *_prevPageButton; // 上一页按钮
    QPushButton *_nextPageButton; // 下一页按钮
    QLabel      *_pageInfoLabel;  // 当前页/总页码显示

    //Item2
    QPdfium *m_pdfium = nullptr;    //QPdfium对象
    QLabel *m_pdfPreview;           // 用于显示PDF预览
    QPushButton *m_loadButton;      // 加载PDF按钮
    QPushButton *m_printButton;     // 打印按钮

    //基本参数
    int _currentPage;               // 当前页码
    int _totalPages;                // 总页数
    QComboBox *m_printerList;       // 打印设备列表
    QRadioButton *m_currentPage;    // 打印范围：当前页
    QRadioButton *m_allPages;       // 打印范围：所有页
    QButtonGroup *m_rangeGroup;     // 打印范围按钮组
    void populatePrinters();        // 获取系统打印机
};
#endif // MAINWINDOW_H
