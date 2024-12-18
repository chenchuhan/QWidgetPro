#ifndef QPDFIUM_H
#define QPDFIUM_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QWeakPointer>
#include <QtCore/QSharedPointer>
#include <QtCore/QVector>

#include "qpdfiumpage.h"
#include "qpdfiumglobal.h"

QT_BEGIN_NAMESPACE

class CPDF_Document;
class CPDF_Page;
class PageHolder;

class Q_PDFIUM_EXPORT QPdfium
{

public:
    enum Status {
        NOT_LOADED = -1,
        SUCCESS = 0,
        FILE_ERROR,
        FORMAT_ERROR,
        PASSWORD_ERROR,
        HANDLER_ERROR,
        FILE_NOT_FOUND_ERROR
    };


    explicit QPdfium();
    QPdfium(QString filename, QString password = QString());

    virtual ~QPdfium();

    bool isValid() const;
    QString filename() const;
    int pageCount() const;
    Status status() const;
    QPdfiumPage page(int i);

public slots:
    Status loadFile(QString filename, QString password = QString());

private:
    Q_DISABLE_COPY(QPdfium)

    QSharedPointer<CPDF_Document> m_document;
    QVector<QWeakPointer<PageHolder>> m_pages;
    QString m_filename;
    int m_pageCount;
    Status m_status;
    QPdfium::Status parseError(int err);
};

QT_END_NAMESPACE

#endif // QPDFIUM_H
