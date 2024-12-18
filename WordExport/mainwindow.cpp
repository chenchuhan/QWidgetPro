#include "mainwindow.h"
// #include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    // , ui(new Ui::MainWindow)
{
    // ui->setupUi(this);
    // 初始化 UI
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout;

    mainLayout->setContentsMargins(10, 10, 10, 10); // 设置布局边距
    mainLayout->setSpacing(10); // 设置控件之间的间距
    // setMinimumSize(600, 520); // 设置窗口最小宽度为600，高度为400

    // 模板文件选择
    auto *templateLayout = new QHBoxLayout;
    auto *templateLabel = new QLabel("打开模板:", this);
    templatePathEdit = new QLineEdit(this);
    auto *browseTemplateButton = new QPushButton("浏览", this);
    templateLayout->addWidget(templateLabel);
    templateLayout->addWidget(templatePathEdit);
    templateLayout->addWidget(browseTemplateButton);
    connect(browseTemplateButton, &QPushButton::clicked, this, &MainWindow::browseTemplateFile);

    // 输出文件选择
    auto *outputLayout = new QHBoxLayout;
    auto *outputLabel = new QLabel("输出路径:", this);
    outputPathEdit = new QLineEdit(this);
    auto *browseOutputButton = new QPushButton("浏览", this);
    outputLayout->addWidget(outputLabel);
    outputLayout->addWidget(outputPathEdit);
    outputLayout->addWidget(browseOutputButton);
    connect(browseOutputButton, &QPushButton::clicked, this, &MainWindow::browseOutputFile);

    // 键值对表格
    auto *placeholdersLabel = new QLabel("键值对替换:", this);
    placeholdersTable = new QTableWidget(this);
    placeholdersTable->setColumnCount(2);
    placeholdersTable->setHorizontalHeaderLabels({"占位符", "替换值"});
    placeholdersTable->setRowCount(5); // 默认三行

    // 设置默认值
    placeholdersTable->setItem(0, 0, new QTableWidgetItem("[A]"));
    placeholdersTable->setItem(0, 1, new QTableWidgetItem("柯布"));

    placeholdersTable->setItem(1, 0, new QTableWidgetItem("[B]"));
    placeholdersTable->setItem(1, 1, new QTableWidgetItem("阿瑟"));

    placeholdersTable->setItem(2, 0, new QTableWidgetItem("[C]"));
    placeholdersTable->setItem(2, 1, new QTableWidgetItem("杜拉"));

    placeholdersTable->setItem(3, 0, new QTableWidgetItem("[D]"));
    placeholdersTable->setItem(3, 1, new QTableWidgetItem("伊姆斯"));

    // 替换按钮
    replaceButton = new QPushButton("执行替换", this);
    connect(replaceButton, &QPushButton::clicked, this, &MainWindow::replaceInWord);




    // 布局整合
    mainLayout->addLayout(templateLayout);
    mainLayout->addLayout(outputLayout);
    mainLayout->addWidget(placeholdersLabel);
    mainLayout->addWidget(placeholdersTable);
    mainLayout->addWidget(replaceButton);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    setWindowTitle("Word 替换工具");

    resize(500, 400); // 初始窗口大小

    // wordPreview = new QAxWidget(this);
}

MainWindow::~MainWindow()
{
    // delete ui;
}

void MainWindow::browseTemplateFile() {
    QString filePath = QFileDialog::getOpenFileName(this, "选择模板文件", QString(), "Word 文件 (*.docx *.doc)");
    if (!filePath.isEmpty()) {
        templatePathEdit->setText(filePath);

        wordApp = new QAxObject("Word.Application");

        // wordApp = wordPreview->querySubObject("Application");

        if (wordApp->isNull()) {
            qDebug() << "Failed to initialize Word.Application.";
            delete wordApp;
            return ;
        }

        // 隐藏 Word 窗口
        wordApp->setProperty("Visible", false);

        //打开指定文档
        QAxObject *documents = wordApp->querySubObject("Documents");
        QAxObject *document = documents->querySubObject("Open(const QString&)", filePath);
        if (document == nullptr) {
            QMessageBox::critical(this, "错误", "无法打开 Word 文件！");
            return;
        }

        // // 切换到打印预览模式
        // QAxObject *activeDocument = wordApp->querySubObject("ActiveDocument");
        // if (activeDocument) {
        //     activeDocument->dynamicCall("PrintPreview()");
        // }

        // QMessageBox::information(this, "提示", "文档已加载到打印预览。");
    }
}

void MainWindow::browseOutputFile() {
    QString filePath = QFileDialog::getSaveFileName(this, "选择输出文件", QString(), "Word 文件 (*.docx *.doc)");
    if (!filePath.isEmpty()) {
        outputPathEdit->setText(filePath);
    }
}

void MainWindow::replaceInWord() {
    QString templatePath = templatePathEdit->text();
    QString outputPath = outputPathEdit->text();

    if (templatePath.isEmpty() || outputPath.isEmpty()) {
        QMessageBox::warning(this, "错误", "请填写模板路径和输出路径！");
        return;
    }

    QMap<QString, QString> placeholders;
    for (int row = 0; row < placeholdersTable->rowCount(); ++row) {
        QString key = placeholdersTable->item(row, 0) ? placeholdersTable->item(row, 0)->text() : QString();
        QString value = placeholdersTable->item(row, 1) ? placeholdersTable->item(row, 1)->text() : QString();
        if (!key.isEmpty()) {
            placeholders.insert(key, value);
        }
    }

    if (placeholders.isEmpty()) {
        QMessageBox::warning(this, "错误", "请填写至少一个占位符和替换值！");
        return;
    }

    if (replaceMultiple(templatePath, outputPath, placeholders)) {
        QMessageBox::information(this, "成功", "替换完成！");
    } else {
        QMessageBox::critical(this, "失败", "替换失败！");
    }
}

bool MainWindow::replaceMultiple(const QString &templatePath, const QString &outputPath, const QMap<QString, QString> &placeholders) {

    qDebug() << "Received data:" << placeholders;


    qDebug() << "Template Path:" << templatePath;
    qDebug() << "Output Path:" << outputPath;

    if (!QFile::exists(templatePath)) {
        qDebug() << "Template file does not exist:" << templatePath;
        return false;
    }

    qDebug() << "QFile::exists ok" ;


    // 打开模板文件
    QAxObject *documents = wordApp->querySubObject("Documents");
    QAxObject *document = documents->querySubObject("Open(const QString&)", templatePath);

    // 查找占位符并替换
    //使用 Find.Execute 查找占位符，使用 TypeText 方法替换为新内容
    QAxObject *selection = wordApp->querySubObject("Selection");

    // 获取 Find 对象
    QAxObject *find = selection->querySubObject("Find");

    qDebug() << "start placeholde";

    // 遍历占位符键值对, 替换未成功，则有问题
    for (auto it = placeholders.begin(); it != placeholders.end(); ++it) {
        QString placeholder = it.key();
        QString newContent = it.value();

        bool isFound = true;

        //可替换多个，且重复的
        while (isFound) {
            // 查找目标文本并替换
            //            isFound = find->dynamicCall("Execute(const QString&)", placeholder).toBool();
            isFound = find->dynamicCall("Execute(QString, bool, bool, bool, bool, bool, bool, int)",
                                        placeholder,  // 要查找的字符串
                                        false,        // 区分大小写
                                        false,        // 完整单词
                                        false,        // 使用通配符
                                        false,        // 忽略标点符号
                                        false,        // 忽略空格
                                        true,         // 向前查找
                                        1).toBool();   // 查找范围：整个文档

            if (isFound) {
                // 替换文本
                selection->dynamicCall("TypeText(const QString&)", newContent);
            }
        }
    }

    qDebug() << "All Find operation succeed!";

    document->dynamicCall("SaveAs(const QString&)", outputPath);

    // 关闭文档
    document->dynamicCall("Close()");
    wordApp->dynamicCall("Quit()");
    delete wordApp;

    return true;
}

