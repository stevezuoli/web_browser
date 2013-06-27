#include "evernote_application.h"

namespace evernote_kindle
{

static const QString DEFAULT_EXPORT_FOLDER = "mnt/us/DK_System/xKindle/evernote";
static const QString DEFAULT_HOST = "sandbox.evernote.com";
static const int DEFAULT_USER_STORE_PORT = 80;
    
EvernoteApplication::EvernoteApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
    // parse arguments
    if (argc >= 2)
    {
        QString path = QString::fromLocal8Bit(argv[1]);
        open(path);
    }
    else
    {
        open(defaultFolder());
    }
}

EvernoteApplication::~EvernoteApplication(void)
{
    
}
    
QString EvernoteApplication::defaultFolder()
{
#ifdef BUILD_FOR_ARM
    return DEFAULT_EXPORT_FOLDER;
#else
    return QDir::homePath();
#endif
}
    
bool EvernoteApplication::findFiles()
{
    QDir::Filters filters = QDir::Files|QDir::NoDotAndDotDot;
    QFileInfoList list = dir_.entryInfoList(filters);
    foreach (QFileInfo info, list)
    {
        QString entry_path = info.absoluteFilePath();
        if (entry_path.endsWith(".xml", Qt::CaseInsensitive))
        {
            qDebug("Found export file:%s", qPrintable(entry_path));
            entries_.push_back(entry_path);
        }
    }
    return !entries_.isEmpty();
}

bool EvernoteApplication::open(const QString& path)
{
    if (!dir_.cd(path))
    {
        dir_.cd(defaultFolder());
    }
    
    if (dir_.exists())
    {
        qDebug("Open evernote folder:%s", qPrintable(path));
        findFiles();
    }
}

bool EvernoteApplication::exportAll()
{
    foreach (QString path, entries_)
    {
        exportFile(path);
    }
}

bool EvernoteApplication::exportFile(const QString& path)
{
    EvernoteContent note;
    if (!note.load(path))
    {
        return false;
    }
    
    QString host = DEFAULT_HOST;
    int port = DEFAULT_USER_STORE_PORT;
    manager_.openSession(host, port);
    return manager_.exportNote(note);
}

};