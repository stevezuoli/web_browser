#include "evernote_application.h"

namespace evernote_kindle
{

static const QString DEFAULT_EXPORT_FOLDER = "mnt/us/DK_System/xKindle/evernote";
static const QString DEFAULT_HOST = "sandbox.evernote.com";
static const int DEFAULT_USER_STORE_PORT = 80;
    
EvernoteApplication::EvernoteApplication(int &argc, char **argv)
    //: QApplication(argc, argv)
    : host_(DEFAULT_HOST)
{
    if (argc >= 3)
    {
        host_ = QString::fromLocal8Bit(argv[2]);
    }
}

EvernoteApplication::~EvernoteApplication(void)
{
}
    
bool EvernoteApplication::createSession()
{
    int port = DEFAULT_USER_STORE_PORT;
    return manager_.createSession(host_, port);
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
        QString entry_path = QString::fromLocal8Bit(info.absoluteFilePath().toLocal8Bit().constData());
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
    
    manager_.openNoteSession();
    foreach (QString path, entries_)
    {
        exportFile(path);
    }
    
    // it costs a lot of time to close session, just exit app directly.
    //manager_.closeNoteSession();
    ::exit(0);
    return true;
}

bool EvernoteApplication::exportFile(const QString& path)
{
    EvernoteContent note;
    if (!note.load(path))
    {
        return false;
    }
    if (manager_.exportNote(note))
    {
        return removeFile(path);
    }
    return false;
}
    
bool EvernoteApplication::removeFile(const QString& path)
{
    QFile file(path);
    return file.remove();
}

bool EvernoteApplication::getUser()
{
    // TODO. save to user file
    manager_.openUserSession();
    EvernoteUser user;
    if (manager_.getUser(user))
    {
        user.save();
    }
    manager_.closeUserSession();
    return true;
}

void EvernoteApplication::onError(const QString& error_str)
{
    // ignore analyze the string, just exit
    //qApp->exit();
    ::exit(0);
}

};