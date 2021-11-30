#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class homePage; }
QT_END_NAMESPACE

class homePage : public QMainWindow
{
    Q_OBJECT

public:
    homePage(QWidget *parent = nullptr);
    ~homePage();

private:
    Ui::homePage *ui;

private slots:
    void initForm();

};
#endif // HOMEPAGE_H
