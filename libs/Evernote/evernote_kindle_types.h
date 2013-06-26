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

/*
    <ReadingDataItem>
    <Type>COMMENT</Type>
    <DataID>B05521606B0F260A5A6ED670078611D4</DataID>
    <Color></Color>
    <Tag></Tag>
    <CreateTime>2013-05-24T10:15:07</CreateTime>
    <LastModifyTime>2013-05-24T10:15:07</LastModifyTime>
    <RefContent>daily</RefContent>
    <BeginRefPos>
    <ChapterID>cover</ChapterID>
    <Offset>573</Offset>
    <ChapterIndex>0</ChapterIndex>
    <ParaIndex>0</ParaIndex>
    <AtomIndex>7</AtomIndex>
    </BeginRefPos>
    <EndRefPos>
    <ChapterID>cover</ChapterID>
    <Offset>578</Offset>
    <ChapterIndex>0</ChapterIndex>
    <ParaIndex>0</ParaIndex>
    <AtomIndex>12</AtomIndex>
    </EndRefPos>
    </ReadingDataItem>
*/

// Single annotation item
class KindleAnnotationItem
{
public:
    KindleAnnotationItem() {}
    ~KindleAnnotationItem() {}
    
    QString formattedDate() const { return create_time; }
public:
    QString type; //COMMENT
    QString data_id; //B05521606B0F260A5A6ED670078611D4
    QString color;
    QString tag;
    QString create_time;
    QString last_modify_time;
    QString content;
    QString comment;
    QString chapter_id;
};

// Chapter, Annotations
typedef QMultiHash<QString, KindleAnnotationItem> Annotations;
typedef QSet<QString> Chapters;
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
