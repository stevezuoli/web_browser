#include <string>
#include <QApplication>
#include "evernote_kindle_types.h"

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
    
bool EvernoteContent::load(const QString& path)
{
    QDomDocument dom;
    if (!open(path, dom))
    {
        return false;
    }
    
    return true;
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
    
    // TODO.
    return false;
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
    return false;
}

}
