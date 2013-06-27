#include <string>
#include "evernote_session.h"

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
    token();
}

EvernoteSession::~EvernoteSession()
{
}
 
QString EvernoteSession::token()
{
    if (token_ == 0)
    {
        token_.reset(new EvernoteToken);
        token_->mutableHost() = host_;
        TokenRing token_ring;
        token_ring.evernoteToken(*token_);
    }
    return token_->token();
}

boost::shared_ptr<UserStoreClient> EvernoteSession::userStoreClient()
{
    if (user_store_ != 0)
    {
        return user_store_;
    }
    
    // contruct user client
    us_transport_.reset(new THttpClient(host_.toStdString(), port_, "/edam/user"));
    us_protocol_.reset(new TBinaryProtocol(us_transport_));
    user_store_.reset(new UserStoreClient(us_protocol_));
    return user_store_;
}

QString EvernoteSession::retrieveNoteStoreUrl()
{
    if (us_transport_ == 0 || user_store_ == 0)
    {
        return "";
    }
    
    std::string ns_url;
    try
    {
        us_transport_->open();
        user_store_->getNoteStoreUrl(ns_url, token_->token().toStdString());
    } catch(apache::thrift::TException te) {
    cerr << "TException:" << te.what() << "|" <<endl;
    } catch(std::exception e) {
    cerr << "Error:" <<e.what() << "|" <<endl;
    } catch(...) {
    cerr << "Error:unknown error" <<endl;
    }
    us_transport_->close();
    return QString(ns_url.c_str());
}

boost::shared_ptr<NoteStoreClient> EvernoteSession::noteStoreClient()
{
    if (note_store_ != 0)
    {
        return note_store_;
    }
    
    // read from db
    QString ns_url = token_->noteStoreUrl();
    // or read from server
    //QString ns_url = retrieveNoteStoreUrl();
    qDebug("Note Store URL:%s", qPrintable(ns_url));
    
    QStringList ns_url_content = ns_url.split(host_);
    if (ns_url_content.size() < 2)
    {
        return note_store_;
    }
    
    note_store_host_ = host_;
    int nsport = NOTESTORE_PORT;
    note_store_path_ = ns_url_content[1];
    qDebug("Note Store Path:%s", qPrintable(note_store_path_));
    
    ssl_socket_factory_.reset(new TSSLSocketFactory());
    // No local certificate
    //ssl_socket_factory_->loadTrustedCertificates("/etc/ssl/certs/ca-bundle.crt");
    
    // No need to authenticate
    ssl_socket_factory_->authenticate(false);
    
    ssl_socket_ = ssl_socket_factory_->createSocket(note_store_host_.toStdString(), nsport);
    buffered_transport_.reset(new TBufferedTransport(ssl_socket_));
    ns_http_client_.reset(new THttpClient(buffered_transport_,
                                     note_store_host_.toStdString(),
                                     note_store_path_.toStdString()));
    ns_protocol_.reset(new TBinaryProtocol(ns_http_client_));
    note_store_.reset(new NoteStoreClient(ns_protocol_));
    return note_store_;
}
    
bool EvernoteSession::openUserStore(boost::shared_ptr<UserStoreClient>& us_store)
{
    boost::shared_ptr<TProtocol> us_protocol = us_store->getInputProtocol();
    boost::shared_ptr<TTransport> us_transport = us_protocol->getTransport();
    try {
        us_transport->open();
    } catch(apache::thrift::TException te) {
        cerr << "TException:" << te.what()<< "|" <<endl;
        return false;
    } catch(std::exception e) {
        cerr << "Error:" << e.what() << "|" <<endl;
        return false;
    } catch(...) {
        cerr << "Error:unknown error" <<endl;
        return false;
    }
    return true;
}

bool EvernoteSession::closeUserStore(boost::shared_ptr<UserStoreClient>& us_store)
{
    boost::shared_ptr<TProtocol> us_protocol = us_store->getInputProtocol();
    boost::shared_ptr<TTransport> us_transport = us_protocol->getTransport();
    us_transport->close();
    return true;
}

bool EvernoteSession::openNoteStore(boost::shared_ptr<NoteStoreClient>& ns_store)
{
    boost::shared_ptr<TProtocol> ns_protocol = ns_store->getInputProtocol();
    boost::shared_ptr<TTransport> ns_transport = ns_protocol->getTransport();
    
    try {
        ns_transport->open();
    } catch(apache::thrift::TException te) {
        cerr << "TException:" << te.what()<< "|" <<endl;
        return false;
    } catch(std::exception e) {
        cerr << "Error:" << e.what() << "|" <<endl;
        return false;
    } catch(...) {
        cerr << "Error:unknown error" <<endl;
        return false;
    }
    return true;
}
bool EvernoteSession::closeNoteStore(boost::shared_ptr<NoteStoreClient>& ns_store)
{
    boost::shared_ptr<TProtocol> ns_protocol = ns_store->getInputProtocol();
    boost::shared_ptr<TTransport> ns_transport = ns_protocol->getTransport();
    ns_transport->close();
    return true;
}

}
