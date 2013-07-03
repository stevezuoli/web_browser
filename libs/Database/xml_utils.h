#ifndef XML_UTILS_H_
#define XML_UTILS_H_

#include "Base/base.h"
#include <QtCore/QtCore>
#include <QtXml>

namespace web_database
{

bool appendXmlTextNode(const QString& name,
                       const QString& value,
                       QDomDocument& doc,
                       QDomElement& parent);

};

#endif  // XML_UTILS_H_

