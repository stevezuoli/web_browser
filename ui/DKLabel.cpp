#include "ui/DKLabel.h"    
#include "common/WindowsMetrics.h"

namespace ui
{
using namespace windowsmetrics;
static const QString LABEL_STYLE = "    \
QLabel\
{\
    margin: 0px;                        \
    padding: 0px;                       \
    background: transparent;            \
    font: %1px;                         \
    color: black;                       \
}\
QLabel:focus\
{\
    margin: 0px;                        \
    padding: 0px;                       \
    background: transparent;            \
    font: %2px;                         \
    color: white;                       \
}\
";
DKLabel::DKLabel(QWidget* parent)
    : QLabel(parent)
{
    InitDKProperty();
}

DKLabel::DKLabel(const QString& text, QWidget* parent)
    : QLabel(parent)
{
    setText(text);
    InitDKProperty();
}

void DKLabel::setDKStyleSheet()
{
    setStyleSheet(LABEL_STYLE.
        arg(GetWindowFontSize(DKLabelIndex)).
        arg(GetWindowFontSize(DKLabelIndex)));
}

void DKLabel::InitDKProperty()
{
    setDKStyleSheet();
}

void DKLabel::setFontSize(int fontSize)
{
    QFont labelFont = font();
    labelFont.setPixelSize(fontSize);
    setFont(labelFont);
}

}//ui
