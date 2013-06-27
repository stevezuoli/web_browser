#ifndef EVERNOTE_APPLICATION_H_
#define EVERNOTE_APPLICATION_H_

#ifndef _WIN32
#pragma GCC system_header
#endif

#include <QApplication>
#include "Evernote/evernote_manager.h"

namespace evernote_kindle
{

class EvernoteApplication : public QApplication
{
    Q_OBJECT;

public:
    EvernoteApplication(int &argc, char **argv);
    virtual ~EvernoteApplication(void);

public Q_SLOTS:
    bool open(const QString& path);
    bool exportAll();
    bool exportFile(const QString& path);

private Q_SLOTS:
    void onError(const QString& error_str);
    
private:
    static QString defaultFolder();
    bool findFiles();
    bool removeFile(const QString& path);
    
private:
    typedef QVector<QString> Entries;

private:
    QDir dir_;
    Entries entries_;
    EvernoteManager manager_;
};

};  // namespace evernote_kindle

#endif