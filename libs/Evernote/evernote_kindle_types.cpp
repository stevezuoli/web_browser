#include <string>
#include <QApplication>
#include "evernote_kindle_types.h"
#include "Database/evernote_token.h"
#include "device/fat.h"

using namespace web_database;
using namespace std;
using namespace boost;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace evernote::edam;

namespace evernote_kindle
{

QString replaceInvalidCharacters(QString src)
{
    QString result = src.replace("&", "&amp;");
    result = result.replace("<", "&lt;");
    result = result.replace(">", "&gt;");
    result = result.replace("'", "&apos;");
    result = result.replace("\"", "&quot;");
    return result;
}
    
bool EvernoteUser::save()
{
    QDomDocument doc("EvernoteUser");
    QDomElement root = doc.createElement("EvernoteUser");
    doc.appendChild(root);
    
    QDomElement user_name_element = doc.createElement("username");
    root.appendChild(user_name_element);
    QDomText user_name = doc.createTextNode(username);
    user_name_element.appendChild(user_name);
    
    QDomElement email_element = doc.createElement("email");
    root.appendChild(email_element);
    QDomText e_mail = doc.createTextNode(email);
    email_element.appendChild(e_mail);
    
    QDomElement name_element = doc.createElement("name");
    root.appendChild(name_element);
    QDomText name_text = doc.createTextNode(name);
    name_element.appendChild(name_text);
    
    QDomElement timezone_element = doc.createElement("timezone");
    root.appendChild(timezone_element);
    QDomText time_zone = doc.createTextNode(timezone);
    timezone_element.appendChild(time_zone);
    
    QString xml = doc.toString();
    qDebug("EvernoteToken:%s", qPrintable(xml));
    
    QString path = EvernoteToken::home();
    QDir dir(path);
    if (!dir.exists(EvernoteToken::dirName()))
    {
        if (!dir.mkdir(EvernoteToken::dirName()))
        {
            return false;
        }
    }
    
    if (dir.cd(EvernoteToken::dirName()))
    {
        // Change folder attribute.
        changeToHidden(dir.absolutePath().toLocal8Bit().constData());
        
        path = dir.filePath("evernote_user.xml");
        QFile file(path);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream TextStream(&file);
            TextStream << xml;
            file.close();
            return true;
        }
    }
    return false;
}
    
bool EvernoteContent::load(const QString& path)
{
    QDomDocument dom;
    if (!open(path, dom))
    {
        return false;
    }
    
    parseTitleAndAuthor(path);
    return parse(dom);
}

bool EvernoteContent::open(const QString& path, QDomDocument& dom)
{
    QFile xml_file(path);
    if(!xml_file.open(QIODevice::ReadOnly))
    {
        qDebug("Opening file failed");
        return false;
    }

    bool ret = true;
    if (!dom.setContent(&xml_file))
    {
        qDebug("Loading xml file failed");
        ret = false;
    }
    xml_file.close();
    return ret;
}
    
bool EvernoteContent::parse(QDomDocument& dom)
{
    QDomElement root = dom.documentElement();
    
    // Get book id
    if (!parseBookID(root))
    {
        return false;
    }
    
    // Parse annotations
    return parseAnnotations(root);
}
    
bool EvernoteContent::parseTitleAndAuthor(const QString& path)
{
    // TODO. The title and author is kept in file name
    // To make an elegant way later
    QFileInfo file_info(path);
    QString file_name = file_info.fileName();
    QString title_author = file_name.split('.').at(0);
    QStringList book_info = title_author.split('_');
    title = book_info[0];
    if (book_info.size() > 1)
    {
        author = book_info[1];
    }
    return true;
}
    
bool EvernoteContent::parseBookID(QDomElement& root_node)
{
    QDomNodeList book_ids = root_node.elementsByTagName("BookID");
    if (book_ids.isEmpty())
    {
        return false;
    }
    
    // use the first one
    QDomNode id_node = book_ids.at(0);
    QDomText id_text = id_node.firstChild().toText();
    if (!id_text.isNull())
    {
        book_id = id_text.data();
        return true;
    }
    return false;
}
    
bool EvernoteContent::parseAnnotations(QDomElement& root_node)
{
    QDomNodeList annotation_nodes = root_node.elementsByTagName("ReadingDataItem");
    if (annotation_nodes.isEmpty())
    {
        return false;
    }
    
    for (int i = 0; i < annotation_nodes.size(); i++)
    {
        // parse each annotation
        QDomNode annotation_node = annotation_nodes.at(i);
        QDomElement annotation_element = annotation_node.toElement();
        if (!annotation_node.isNull())
        {
            parseAnnotation(annotation_element);
        }
    }
    return true;
}

bool EvernoteContent::parseAnnotation(QDomElement& root_node)
{
    // TODO. Implement it
    KindleAnnotationItem item;

    item.parseElementByTagName("Type", root_node);
    item.parseElementByTagName("DataID", root_node);
    item.parseElementByTagName("Color", root_node);
    item.parseElementByTagName("Tag", root_node);
    item.parseElementByTagName("CreateTime", root_node);
    item.parseElementByTagName("LastModifyTime", root_node);
    item.parseElementByTagName("RefContent", root_node);
    item.parseElementByTagName("Content", root_node);
    
    // parse chapter
    item.parseChapterInfo(root_node);
    if (item.isValid())
    {
        chapters.insert(item.chapter_id);
        annotations.insert(item.chapter_id, item);
        return true;
    }
    return false;
}

bool KindleAnnotationItem::isValid()
{
    return !content.isEmpty();
}

bool KindleAnnotationItem::KindleAnnotationItem::parseChapterInfo(QDomElement& root_node)
{
    QDomNodeList refer_positions = root_node.elementsByTagName("BeginRefPos");
    if (refer_positions.isEmpty())
    {
        return false;
    }
    QDomElement refer_position = refer_positions.at(0).toElement();
    parseElementByTagName("ChapterID", refer_position);
    parseElementByTagName("ChapterIndex", refer_position);
    return !chapter_num.isEmpty();
}
    
bool KindleAnnotationItem::parseElementByTagName(const QString& name, QDomElement& root_node)
{
    QString* attribute = mutableAttributeByName(name);
    QDomNodeList type_nodes = root_node.elementsByTagName(name);
    if (!type_nodes.isEmpty() && attribute != 0)
    {
        // use the first one
        QDomNode type_node = type_nodes.at(0);
        QDomText type_text = type_node.firstChild().toText();
        if (!type_text.isNull())
        {
            *attribute = type_text.data();
            return true;
        }
    }
    return false;
}

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
QString* KindleAnnotationItem::mutableAttributeByName(const QString& name)
{
    if (name.compare("Type", Qt::CaseInsensitive) == 0)
    {
        return &type;
    }
    else if (name.compare("DataID", Qt::CaseInsensitive) == 0)
    {
        return &data_id;
    }
    else if (name.compare("Color", Qt::CaseInsensitive) == 0)
    {
        return &color;
    }
    else if (name.compare("Tag", Qt::CaseInsensitive) == 0)
    {
        return &tag;
    }
    else if (name.compare("CreateTime", Qt::CaseInsensitive) == 0)
    {
        return &create_time;
    }
    else if (name.compare("LastModifyTime", Qt::CaseInsensitive) == 0)
    {
        return &last_modify_time;
    }
    else if (name.compare("RefContent", Qt::CaseInsensitive) == 0)
    {
        return &content;
    }
    else if (name.compare("Content", Qt::CaseInsensitive) == 0)
    {
        return &comment;
    }
    else if (name.compare("ChapterID", Qt::CaseInsensitive) == 0)
    {
        return &chapter_id;
    }
    else if (name.compare("ChapterIndex", Qt::CaseInsensitive) == 0)
    {
        return &chapter_num;
    }
    return 0;
}

}
