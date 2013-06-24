#include "ui/DKLabel.h"    
#include "common/WindowsMetrics.h"

namespace ui
{
using namespace windowsmetrics;
static const QString LABEL_STYLE = "    \
    padding: 0px;                       \
    background: transparent;            \
    font: %1px;                         \
    color: black;                       \
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
    setStyleSheet(LABEL_STYLE.arg(GetWindowFontSize(DKLabelIndex)));
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
