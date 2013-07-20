#ifndef DUOKAN_TEXT_LAYOUT_H__
#define DUOKAN_TEXT_LAYOUT_H__

#include <Qt/QtCore>
#include <Qt/QtGui>

namespace framework
{

int ellipsisText(const QString &org,
                 const QFontMetrics &fm,
                 const int width,
                 const Qt::TextElideMode mode,
                 QString & output);


int calculateSingleLineLayout(QTextLayout & layout,
                              const QFont & font,
                              const QString & text,
                              const Qt::Alignment align,
                              const QRect & rect,
                              const Qt::TextElideMode ellipsis = Qt::ElideRight);


int calculateMultiLinesLayout(QTextLayout & layout,
                              const QFont & font,
                              const QString & text,
                              const Qt::Alignment align,
                              const QRect & rect);

int calculateMultiLinesLayout(const QFont & font,
                              const QString & text,
                              int width);

int calculateMultiLinesLayout(QTextLayout & layout,
                              const QFont & font,
                              const QString & text,
                              const Qt::Alignment align,
                              const QRect & rect,
                              int lines);

QString calculateMultiLinesText(const QFont & font,
                              const QString & text,
                              const Qt::Alignment align,
                              int width,
                              int lines);

void drawSingleLineText(QPainter &painter,
                        const QFont & font,
                        const QString & text,
                        const Qt::Alignment align,
                        const QRect & rect);

}  // namespace framework

#endif
