#include <string>
#include "evernote_session.h"
#include "Database/token_ring.h"

using namespace std;
using namespace boost;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace evernote::edam;
using namespace web_database;

namespace evernote_kindle
{

static const int NOTESTORE_PORT = 443;

EvernoteSession::EvernoteSession(const QString& host, int port)
    : host_(host)
    , port_(port)
{
    // Load token from token ring
    EvernoteToken token_record;
    token_record.mutableHost() = host_;
    TokenRing token_ring;
    token_ring.evernoteToken(token_record);
    token_ = token_record.token();

    // contruct user client
    us_transport_.reset(new THttpClient(host_.toStdString(), port_, "/edam/user"));
    us_protocol_.reset(new TBinaryProtocol(us_transport_));
    user_store_.reset(new UserStoreClient(us_protocol_));

    // contruct note store

}

EvernoteSession::~EvernoteSession()
{
}

bool EvernoteSession::initializeNoteStore()
{
    bool ret = true;
    std::string ns_url;
    try
    {
        us_transport_->open();
        user_store_->getNoteStoreUrl(ns_url, token_.toStdString());
    } catch(apache::thrift::TException te) {
        cerr << "TException:" << te.what() << "|" <<endl;
        ret = false;
    } catch(std::exception e) {
        cerr << "Error:" <<e.what() << "|" <<endl;
        ret = false;
    } catch(...) {
        cerr << "Error:unknown error" <<endl;
        ret = false;
    }

    us_transport_->close();
    if (!ret || ns_url.empty())
    {
        return false;
    }

    QString ns_url_str(ns_url.c_str());
    qDebug("Note Store URL:%s", qPrintable(ns_url_str));

    QStringList ns_url_content = ns_url_str.split(host_);
    if (ns_url_content.size() < 2)
    {
        return false;
    }

    note_store_host_ = host_;
    int nsport = NOTESTORE_PORT;
    note_store_path_ = ns_url_content[1];
    qDebug("Note Store Path:%s", qPrintable(note_store_path_));

    ssl_socket_factory_.reset(new TSSLSocketFactory());
    //ssl_socket_factory_->loadTrustedCertificates("/etc/ssl/certs/ca-bundle.crt");

    ssl_socket_ = ssl_socket_factory_->createSocket(note_store_host_.toStdString(), nsport);
    buffered_transport_.reset(new TBufferedTransport(ssl_socket_));
    ns_http_client_.reset(new THttpClient(buffered_transport_,
                                          note_store_host_.toStdString(),
                                          note_store_path_.toStdString()));
    ns_protocol_.reset(new TBinaryProtocol(ns_http_client_));
    note_store_.reset(new NoteStoreClient(ns_protocol_));
    return true;
}

}
