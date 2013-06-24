#ifndef EVERNOTE_SESSION_H_
#define EVERNOTE_SESSION_H_

#include "Base/base.h"
#include <QtCore/QtCore>
#include <boost/shared_ptr.hpp>
#include "thrift/transport/THttpClient.h"
#include "thrift/protocol/TBinaryProtocol.h"
#include "evernote/UserStore.h"
#include "evernote/NoteStore.h"
#include "thrift/transport/TSSLSocket.h"

using namespace std;
using namespace boost;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace evernote::edam;

namespace evernote_kindle
{

class EvernoteSession
{
public:
    EvernoteSession(const QString& host, int port);
    ~EvernoteSession();

    bool initializeNoteStore();
private:
    // app.yinxiang.com or www.evernote.com
    QString host_;
    int port_;
    QString token_;

    // notestore host
    QString note_store_host_;
    QString note_store_path_;

    // content (reader from KindleApp)
    QString title_;
    QString content_;

    // notebook
    QString notebook_name_;

    // Thrift objects
    boost::shared_ptr<TTransport>        us_transport_;
    boost::shared_ptr<TProtocol>         us_protocol_;
    boost::shared_ptr<UserStoreClient>   user_store_;

    boost::shared_ptr<TSSLSocketFactory> ssl_socket_factory_;
    boost::shared_ptr<TSSLSocket>        ssl_socket_;
    boost::shared_ptr<TTransport>        buffered_transport_;
    boost::shared_ptr<TTransport>        ns_http_client_;
    boost::shared_ptr<TProtocol>         ns_protocol_;
    boost::shared_ptr<NoteStoreClient>   note_store_;
};

};

#endif
