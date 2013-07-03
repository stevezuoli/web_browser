#include "xml_utils.h"

namespace web_database
{

bool appendXmlTextNode(const QString& name,
                       const QString& value,
                       QDomDocument& doc,
                       QDomElement& parent)
{
    QDomElement new_element = doc.createElement(name);
    parent.appendChild(new_element);
    QDomText text_node = doc.createTextNode(value);
    new_element.appendChild(text_node);
    return true;
}

}

