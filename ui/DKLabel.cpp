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
    color: %2;                       \
}\
QLabel:focus\
{\
    margin: 0px;                        \
    padding: 0px;                       \
    background: transparent;            \
    font: %3px;                         \
    color: %4;                       \
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
        arg("black").
        arg(GetWindowFontSize(DKLabelIndex)).
        arg("white"));
}

void DKLabel::InitDKProperty()
{
    setDKStyleSheet();
}

void DKLabel::setFontSize(int fontSize)
{
    setStyleSheet(LABEL_STYLE.
        arg(fontSize).
        arg("black").
        arg(fontSize).
        arg("white"));
}

void DKLabel::setFontColor(const char* color)
{
    setStyleSheet(LABEL_STYLE.
        arg(GetWindowFontSize(DKLabelIndex)).
        arg(color).
        arg(GetWindowFontSize(DKLabelIndex)).
        arg("white"));
}

}//ui
