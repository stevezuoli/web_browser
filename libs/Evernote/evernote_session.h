#ifndef EVERNOTE_SESSION_H_
#define EVERNOTE_SESSION_H_

#include "evernote_kindle_types.h"
#include "Database/token_ring.h"

using namespace std;
using namespace boost;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace evernote::edam;

using namespace web_database;

namespace evernote_kindle
{

typedef boost::shared_ptr<UserStoreClient> UserStorePtr;
typedef boost::shared_ptr<NoteStoreClient> NoteStorePtr;
    
class EvernoteSession
{
public:
    EvernoteSession(const QString& host, int port);
    ~EvernoteSession();

    boost::shared_ptr<UserStoreClient> userStoreClient();
    boost::shared_ptr<NoteStoreClient> noteStoreClient();

    QString token();
    
    static bool openUserStore(boost::shared_ptr<UserStoreClient>& us_store);
    static bool closeUserStore(boost::shared_ptr<UserStoreClient>& us_store);
    
    static bool openNoteStore(boost::shared_ptr<NoteStoreClient>& ns_store);
    static bool closeNoteStore(boost::shared_ptr<NoteStoreClient>& ns_store);

private:
    QString retrieveNoteStoreUrl();
    
private:
    // app.yinxiang.com or www.evernote.com
    QString host_;
    int port_;
    boost::shared_ptr<EvernoteToken> token_;

    // notestore host
    QString note_store_host_;
    QString note_store_path_;

    // content (reader from KindleApp)
    QString title_;
    QString content_;

    // notebook
    QString notebook_name_;

    // Thrift objects
    boost::shared_ptr<TTransport>           us_transport_;
    boost::shared_ptr<TProtocol>            us_protocol_;
    UserStorePtr                          user_store_;

    boost::shared_ptr<TSSLSocketFactory>    ssl_socket_factory_;
    boost::shared_ptr<TSSLSocket>           ssl_socket_;
    boost::shared_ptr<TTransport>         buffered_transport_;
    boost::shared_ptr<TTransport>         ns_http_client_;
    boost::shared_ptr<TProtocol>          ns_protocol_;
    NoteStorePtr                        note_store_;
};

};

#endif
