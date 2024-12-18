#include "mainwindow.h"
 // #include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPrinterInfo>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    // , ui(new Ui::MainWindow)
{
    // 设置窗口标题
    setWindowTitle("PDF Viewer & Printer");

    this->initWidget();

    // 主控件和布局
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // PDF 预览区域
    m_pdfPreview = new QLabel("PDF 预览区域", this);
    m_pdfPreview->setFixedSize(700, 880);
    m_pdfPreview->setStyleSheet("border: 1px solid gray; background-color: #f0f0f0;");
    m_pdfPreview->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_pdfPreview);


    //增加一行页码相关，页码控制区域
    //上一页 当前页/总页码 下一页
    QHBoxLayout *pageControlLayout = new QHBoxLayout();

    _prevPageButton = new QPushButton("上一页", this);
    _pageInfoLabel = new QLabel("0 / 0", this);
    _nextPageButton = new QPushButton("下一页", this);
    _pageInfoLabel->setAlignment(Qt::AlignCenter);

    // 将控件添加到水平布局中
    pageControlLayout->addWidget(_prevPageButton);
    pageControlLayout->addWidget(_pageInfoLabel);
    pageControlLayout->addWidget(_nextPageButton);

    layout->addLayout(pageControlLayout);

    // 打印设置区域
    QHBoxLayout *printerLayout = new QHBoxLayout();
    m_printerList = new QComboBox(this);

    m_currentPage = new QRadioButton("当前页", this);
    m_allPages = new QRadioButton("所有页", this);
    m_allPages->setChecked(true); // 默认打印所有页
    m_rangeGroup = new QButtonGroup(this);
    m_rangeGroup->addButton(m_currentPage);
    m_rangeGroup->addButton(m_allPages);

    printerLayout->addWidget(new QLabel("打印机:", this));
    printerLayout->addWidget(m_printerList);
    printerLayout->addWidget(m_currentPage);
    printerLayout->addWidget(m_allPages);

    layout->addLayout(printerLayout);

    // 按钮区域
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_loadButton = new QPushButton("加载PDF", this);
    m_printButton = new QPushButton("打印PDF", this);

    buttonLayout->addWidget(m_loadButton);
    buttonLayout->addWidget(m_printButton);
    layout->addLayout(buttonLayout);

    setCentralWidget(centralWidget);

    // 填充打印机列表
    populatePrinters();

    // 信号与槽连接
    connect(_prevPageButton, &QPushButton::clicked, this, &MainWindow::goToPreviousPage);
    connect(_nextPageButton, &QPushButton::clicked, this, &MainWindow::goToNextPage);
    connect(m_loadButton, &QPushButton::clicked, this, &MainWindow::loadPdf);
    connect(m_printButton, &QPushButton::clicked, this, &MainWindow::printPdf);

}

MainWindow::~MainWindow()
{
    m_pdfium = nullptr;
    delete m_pdfium;
}

//初始化界面
void MainWindow::initWidget()
{
    //初始化变量
    _currentPage = 0;               // 当前页码
    _totalPages = 0;                // 总页数
}

void MainWindow::populatePrinters()
{
    // 获取系统打印机列表
    QList<QPrinterInfo> printers = QPrinterInfo::availablePrinters();
    for (const QPrinterInfo &printer : printers) {
        m_printerList->addItem(printer.printerName());
    }
}

//鼠标滚轮事件，放大缩小
void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(!isPdfValid)
    {
        return;
    }

    //根据鼠标滚轮的方向来缩放图像
    const QPoint numDegrees = event->angleDelta() / 8;
    const int numSteps = numDegrees.y() / 15;

    if(numSteps != 0)
    {
        //更新缩放比例
        if(numSteps > 0)
        {
            //放大
            zoomScale *= 1.1;
            zoomScale = qMin(zoomScale, 3.0);

        }
        else
        {
            //缩小,但要确保缩放比例不会小于某个最小值（比如0.1）
            zoomScale /= 1.1;
            zoomScale = qMax(zoomScale, 0.5);
        }

        //更新缩放后的图像并显示
        updateScaledPixmap();
    }
}

//更新缩放图像
void MainWindow::updateScaledPixmap()
{
    //计算缩放后的尺寸
    QSize scaledSize = _showPixmap.size() * zoomScale;

    //缩放图像
    QPixmap scaledPixmap = _showPixmap.scaled(scaledSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    //更新QLabel的Pixmap
    m_pdfPreview->setPixmap(scaledPixmap);
}

void MainWindow::loadPdf()
{
    // 打开文件对话框加载 PDF
    QString fileName = QFileDialog::getOpenFileName(this, "选择PDF文件", "D:/23_Outproject/32_ZNPicture/Src/QtPdfiumTest2", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        isPdfValid = false;
        return;
    }

    m_pdfium = new QPdfium();

    // 加载 PDF 文件
    m_pdfium->loadFile(fileName);
    isPdfValid = m_pdfium->isValid();
    if (!isPdfValid) {
        m_pdfPreview->setText("加载PDF失败！");
        return;
    }

    _totalPages = m_pdfium->pageCount(); // 更新总页数
    _currentPage = 0;                    // 当前页重置为第一页
    _pageInfoLabel->setText(QString("%1 / %2").arg(_currentPage+1).arg(_totalPages));

    //预览第一页
    QPdfiumPage page = m_pdfium->page(_currentPage);
    if (page.isValid()) {
        QImage image = page.image(1.0); // 渲染为 QImage

        _showPixmap = QPixmap::fromImage(image).scaled (
            m_pdfPreview->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation);

        m_pdfPreview->setPixmap(_showPixmap);
    }
}

//上一页
void MainWindow::goToPreviousPage()
{
    if (_currentPage > 0) {
        _currentPage--;
        _pageInfoLabel->setText(QString("%1 / %2").arg(_currentPage+1).arg(_totalPages));

        QPdfiumPage page = m_pdfium->page(_currentPage);
        if (page.isValid()) {
            QImage image = page.image(1.0);

            _showPixmap = QPixmap::fromImage(image).scaled (
                m_pdfPreview->size(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation);

            m_pdfPreview->setPixmap(_showPixmap);

        }
    }
}

//下一页
void MainWindow::goToNextPage()
{
    if (_currentPage < _totalPages-1) {
        _currentPage++;
        _pageInfoLabel->setText(QString("%1 / %2").arg(_currentPage+1).arg(_totalPages));

        QPdfiumPage page = m_pdfium->page(_currentPage);
        if (page.isValid()) {
            QImage image = page.image(1.0);

            _showPixmap = QPixmap::fromImage(image).scaled (
                m_pdfPreview->size(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation);

            m_pdfPreview->setPixmap(_showPixmap);
        }
    }
}


void MainWindow::printPdf()
{
    // 调用打印功能
    if (!m_pdfium || !m_pdfium->isValid() || m_pdfium->pageCount() == 0) {
        m_pdfPreview->setText("未加载有效PDF，无法打印！");
        return;
    }


    // 创建打印机对象
    QPrinter printer(QPrinter::HighResolution);

    printer.setPrinterName(m_printerList->currentText());

    //是用来设置打印页面的方向，指定打印内容的布局是 纵向 (Portrait) 还是 横向 (Landscape)。
    printer.setOrientation(QPrinter::Portrait);
    printer.setPageSize(QPrinter::A4);

    // 设置打印范围
    int fromPage = 1;
    int toPage = m_pdfium->pageCount();
    if (m_currentPage->isChecked()) {
        fromPage = toPage = 1; // 当前页为第一页
    }

    // 打印 PDF
    QPainter painter(&printer);
    for (int i = fromPage; i <= toPage; ++i) {
        QPdfiumPage page = m_pdfium->page(i);
        if (!page.isValid()) continue;

        QImage image = page.image(1.0);
        QRect targetRect = printer.pageRect();
        QPixmap pixmap = QPixmap::fromImage(image).scaled (
            targetRect.size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation);
        painter.drawPixmap(0, 0, pixmap);

        if (i < toPage)
            printer.newPage();
    }
    painter.end();
}
