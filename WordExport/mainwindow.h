#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMap>
#include <QAxObject>
#include <QAxWidget>

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

private slots:
    void browseTemplateFile();
    void browseOutputFile();
    void replaceInWord();

private:
    QLineEdit *templatePathEdit;
    QLineEdit *outputPathEdit;
    QTableWidget *placeholdersTable;
    QPushButton *replaceButton;
    bool replaceMultiple(const QString &templatePath, const QString &outputPath, const QMap<QString, QString> &placeholders);
    QAxObject *wordApp = nullptr;
    QAxWidget *wordPreview = nullptr;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
