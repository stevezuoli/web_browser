#include <string>
#include <time.h>
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

int main(){
  string host = "app.yinxiang.com";
  int port = 80;
  
  string devtoken = "xxxxxxxxxxxxxxx";
  
  Note* newNote = new Note;
  Note* retNote = new Note;
  newNote->title = string("another evernote");
  newNote->content = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE en-note SYSTEM \"http://xml.evernote.com/pub/enml2.dtd\"><en-note><span>this is a  another hello test to evernote api.</span> </en-note>";
  newNote->__isset.title = true;
  newNote->__isset.content = true;

  Notebook retNotebook;
  Notebook newNotebook;
  newNotebook.name = "apinotebook2";
  newNotebook.__isset.name = true;

  shared_ptr<TTransport> transport (new THttpClient (host, port, "/edam/user"));
  shared_ptr<TProtocol> protocol (new TBinaryProtocol(transport));
  UserStoreClient userStore(protocol);

  
  
  time_t  t1, t2, t3, t4; 
  try {
    time(&t1);
    transport->open();
    time(&t2);
    string noteStoreUrl;
    userStore.getNoteStoreUrl(noteStoreUrl,devtoken);
    time(&t3);
  } catch(apache::thrift::TException te) {
    cerr << "TException:"<<te.what()<<"|" <<endl;        
  } catch(std::exception e) {
    cerr << "Error:"<<e.what()<<"|" <<endl;
  } catch(...) {
    cerr << "Error:unknown error" <<endl;
  }
  transport->close();
  time(&t4);
  cout << "Time" <<" t1 "<<t1<<" t2 "<<t2<<" t3 "<<t3<<" t4 "<<t4<<endl;

  string nshost="app.yinxiang.com";
  int nsport=443;
  string nspath="/edam/note/s1";

  shared_ptr<TSSLSocketFactory> sslSocketFactory(new TSSLSocketFactory());
  sslSocketFactory->loadTrustedCertificates("/etc/ssl/certs/ca-bundle.crt");
  shared_ptr<TSSLSocket> sslSocket = sslSocketFactory->createSocket(nshost, 443);
  shared_ptr<TTransport> bufferedTransport(new TBufferedTransport(sslSocket));
  shared_ptr<TTransport> noteStoreHttpClient (new THttpClient(bufferedTransport, nshost, nspath));
  shared_ptr<TProtocol> nsprotocol(new TBinaryProtocol(noteStoreHttpClient));
  
  time(&t1);
  noteStoreHttpClient->open();
  time(&t2);
  NoteStoreClient noteStore(nsprotocol);
  try {
    noteStore.createNote(*retNote, devtoken, *newNote);
  time(&t3);
  } catch(EDAMUserException ee) {
    cerr << "EException:"<<ee.errorCode<<"|"<<ee.parameter <<endl;        
  } catch(apache::thrift::TException te) {
    cerr << "TException:"<<te.what()<<"|" <<endl;        
  } catch(std::exception e) {
    cerr << "Error:"<<e.what()<<"|" <<endl;
  } catch(...) {
    cerr << "Error:unknown error" <<endl;
  }

  if (noteStoreHttpClient->isOpen()){
    noteStoreHttpClient->close();
  }
  time(&t4);
  
  cout << "Time2" <<" t1 "<<t1<<" t2 "<<t2-t1<<" t3 "<<t3-t2<<" t4 "<<t4-t3<<endl;
  
  return 0;
}

