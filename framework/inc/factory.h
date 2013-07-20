
#ifndef DUOKAN_CONTENT_VIEW_FACTORY_H_
#define DUOKAN_CONTENT_VIEW_FACTORY_H_

#include "content_view.h"

namespace framework
{

class ViewFactory
{
public:
    ViewFactory();
    virtual ~ViewFactory();

public:
    virtual ContentView * createView(QWidget *parent, const QString &type = QString());
};

};  // namespace framework

#endif      // DUOKAN_CONTENT_VIEW_FACTORY_H_
