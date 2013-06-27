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
    bool opened = false;
    if (argc >= 2)
    {
        QString path = QString::fromLocal8Bit(argv[1]);
        opened = open(path);
    }
    else
    {
        opened = open(defaultFolder());
    }
    
    if (opened)
    {
        QString host = DEFAULT_HOST;
        int port = DEFAULT_USER_STORE_PORT;
        manager_.createSession(host, port);
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
        return findFiles();
    }
    return false;
}

// Make sure session has been opened before exporting notes
bool EvernoteApplication::exportAll()
{
    if (entries_.isEmpty())
    {
        return false;
    }
    
    manager_.openSession();
    foreach (QString path, entries_)
    {
        exportFile(path);
    }
    
    // it costs a lot of time to close session, just exit app directly.
    //manager_.closeSession();
    return true;
}

bool EvernoteApplication::exportFile(const QString& path)
{
    EvernoteContent note;
    if (!note.load(path))
    {
        return false;
    }
    return manager_.exportNote(note);
}

};