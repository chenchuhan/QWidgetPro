#include "mainwindow.h"

#include <QApplication>

#include "qpdfium.h"
#include <QDebug>


void PdfTest() {
    QPdfium pdf("D:\\23_Outproject\\32_ZNPicture\\Src\\QtPdfiumTest2\\mytest.pdf");
    if (pdf.isValid()) {
        qDebug() << "PDF loaded successfully!";
        QPdfiumPage page = pdf.page(0);
        QImage image = page.image(1.0); // 渲染为3倍大小
        // image.save("output.png");

        if (image.isNull()) {
            qDebug() << "Failed to render image.";
            return;
        }

    } else {
        qDebug() << "Failed to load PDF!";
    }
}

int main(int argc, char *argv[])
{
    // PdfTest();

    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}
