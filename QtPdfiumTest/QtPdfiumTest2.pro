QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -execution-charset:utf-8  # 让程序执行时使用utf-8字符集
QMAKE_CXXFLAGS += -source-charset:utf-8     # 告诉vc编译器识别源文件编码类型是utf-8


CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    formTest.ui \
    mainwindow.ui

INCLUDEPATH +=  $$PWD/qpdfium/include \
                $$PWD/qpdfium/libs

DEPENDPATH += $$PWD/qpdfium/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L$$PWD/qpdfium/libs/ -lQt5Pdfium


# LIBS += -L$$PWD/qpdfium/libs/ -lQt5Pdfium -lQt5Pdfiumd

# win32:CONFIG(release, debug|release): LIBS += -L$$PWD/qpdfium/libs/ -lQt5Pdfium
# else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/qpdfium/libs/ -lQt5Pdfiumd

# INCLUDEPATH += $$PWD/qpdfium/libs
# DEPENDPATH += $$PWD/qpdfium/libs

# win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/qpdfium/libs/libQt5Pdfium.a
# else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/qpdfium/libs/libQt5Pdfiumd.a
# else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/qpdfium/libs/Qt5Pdfium.lib
# else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/qpdfium/libs/Qt5Pdfiumd.lib
