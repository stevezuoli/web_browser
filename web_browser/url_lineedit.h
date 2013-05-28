#ifndef __WEBBROWSER_URLLINEEDIT_H__
#define __WEBBROWSER_URLLINEEDIT_H__

#include <QtCore/QUrl>
#include <QtGui/QWidget>
#include <QtGui/QStyleOptionFrame>
#include "ui/DKLineEdit.h"

using namespace ui;

namespace webbrowser
{
//class ClearButton;
class ExLineEdit : public QWidget
{
    Q_OBJECT

public:
    ExLineEdit(QWidget *parent = 0);

    inline QLineEdit *lineEdit() const { return m_lineEdit; }

    void setLeftWidget(QWidget *widget);
    QWidget *leftWidget() const;

    QSize sizeHint() const;

    QVariant inputMethodQuery(Qt::InputMethodQuery property) const;
protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void inputMethodEvent(QInputMethodEvent *e);
    bool event(QEvent *event);

protected:
    void updateGeometries();
    void initStyleOption(QStyleOptionFrameV2 *option) const;

    QWidget *m_leftWidget;
    QLineEdit *m_lineEdit;
    //ClearButton *m_clearButton;
};

class UrlIconLabel;
class BrowserView;
class UrlLineEdit : public ExLineEdit
{
    Q_OBJECT

public:
    UrlLineEdit(QWidget *parent = 0);
    void setWebView(BrowserView *webView);

protected:
    void paintEvent(QPaintEvent *event);
    void focusOutEvent(QFocusEvent *event);

private slots:
    void webViewUrlChanged(const QUrl &url);
    void webViewIconChanged();

private:
    QLinearGradient generateGradient(const QColor &color) const;
    BrowserView *m_webView;
    UrlIconLabel *m_iconLabel;
    QColor m_defaultBaseColor;

};

}

#endif // __WEBBROWSER_URLLINEEDIT_H__

