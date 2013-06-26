#ifndef EVERNOTE_MANAGER_H_
#define EVERNOTE_MANAGER_H_

#include "evernote/NoteStore_types.h"
#include "evernote_session.h"

using namespace boost;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace evernote::edam;

namespace evernote_kindle
{

class EvernoteManager
{
public:
    EvernoteManager();
    ~EvernoteManager();
    
    bool openSession(const QString& host, int port);
    bool exportNote(const EvernoteContent& note);
    
    // prepare content
    bool prepareContent(const EvernoteContent& origin_content, QString& title, QString& content);
private:
    // make sure note store is opened before calling these functions
    bool makeSureNotebookExist(NoteStorePtr note_store, Notebook& duokan_book);
    bool findMetadata(NoteStorePtr note_store,
                     const QString& book_id,
                     const Notebook& duokan_book,
                     NotesMetadataList& note_metadata);
    bool addOrUpdateNote(NoteStorePtr note_store,
                        const Notebook& duokan_book,
                        const NotesMetadataList& note_metadata,
                        QString title,
                        const QString& content);

    QString makeHeader(const QString& book_name, const QString& author);
    
    // dump for testing
    void dump(const QString& content);
    
private:
    typedef std::vector<evernote::edam::Notebook> Notebooks;
    
private:
    boost::shared_ptr<EvernoteSession> session_;
};

};

#endif
