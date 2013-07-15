#ifndef EVERNOTE_KINDLE_TYPES_H_
#define EVERNOTE_KINDLE_TYPES_H_

#include "Base/base.h"
#include <QtCore/QtCore>
#include <QDomDocument>
#include <boost/shared_ptr.hpp>
#include "thrift/transport/THttpClient.h"
#include "thrift/protocol/TBinaryProtocol.h"
#include "evernote/UserStore.h"
#include "evernote/NoteStore.h"
#include "evernote/NoteStore_types.h"
#include "thrift/transport/TSSLSocket.h"

using namespace boost;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace evernote::edam;

namespace evernote_kindle
{
// User info
class EvernoteUser
{
public:
    EvernoteUser() {}
    ~EvernoteUser() {}
    
    bool save();
public:
    QString username;
    QString email;
    QString name;
    QString timezone;
};

// Single annotation item
class KindleAnnotationItem
{
public:
    KindleAnnotationItem() {}
    ~KindleAnnotationItem() {}
    
    bool isValid();
    QString formattedDate() const { return create_time; }
    bool parseElementByTagName(const QString& name, QDomElement& root_node);
    bool parseChapterInfo(QDomElement& root_node);

    bool operator== (const KindleAnnotationItem & other) const;
    bool operator> ( const KindleAnnotationItem & other ) const;
    bool operator< ( const KindleAnnotationItem & other ) const;
    bool operator<= ( const KindleAnnotationItem & other ) const;
    bool operator>= ( const KindleAnnotationItem & other ) const;

private:
    QString* mutableAttributeByName(const QString& name);
    
public:
    QString type; //COMMENT
    QString data_id; //B05521606B0F260A5A6ED670078611D4
    QString color;
    QString tag;
    QString create_time;
    QString last_modify_time;
    QString content;
    QString comment;
    QString chapter_title;
    QString chapter_id;
    QString chapter_num;
    QString para_index;
    QString atom_index;
    QString offset;
};

// Chapter, Annotations
typedef QMultiHash<QString, KindleAnnotationItem> Annotations;
typedef QList<QString> Chapters;
class EvernoteContent
{
public:
    EvernoteContent() {}
    ~EvernoteContent() {}
    
    bool load(const QString& path);
    
private:
    bool open(const QString& path, QDomDocument& dom);
    bool parse(QDomDocument& dom);
    
    bool parseBookID(QDomElement& root_node);
    bool parseAnnotations(QDomElement& root_node);
    bool parseAnnotation(QDomElement& root_node);
    bool parseTitleAndAuthor(const QString& path);

public:
    QString title;
    QString book_id;
    QString author;
    Annotations annotations;
    Chapters chapters;
};
    
QString replaceInvalidCharacters(QString src);

};

#endif
