#ifndef __WEBBROWSER_URLLINEEDIT_H__
#define __WEBBROWSER_URLLINEEDIT_H__

#include <QtCore/QUrl>
#include <QtGui/QWidget>
#include <QtGui/QPushButton>
#include <QtGui/QStyleOptionFrame>
#include "ui/DKLineEdit.h"

using namespace ui;

namespace webbrowser
{
class ClearButton : public QAbstractButton
{
    Q_OBJECT
public:
    ClearButton(QWidget* parent = 0);
protected:
    virtual void paintEvent(QPaintEvent* e);

public slots:
    void textChanged(const QString& text);
};//ClearButton

class ExLineEdit : public QWidget
{
    Q_OBJECT

public:
    ExLineEdit(QWidget *parent = 0);

    inline QLineEdit *lineEdit() const { return line_edit_; }

    void setLeftWidget(QWidget *widget);
    QWidget *leftWidget() const;

    QSize sizeHint() const;
    void setModifyLineEditTextAutomatically(bool automatically)
    {
        modify_line_edit_text_automatically_ = automatically;
    }

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

    QWidget *left_widget_;
    QLineEdit *line_edit_;
    //ClearButton *clear_button_;
    QPushButton *refresh_button_;
    QPushButton *stop_button_;
    bool modify_line_edit_text_automatically_;
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
    void webViewProgressChanged(const int progress, const int total);
    void onCloseButtonClicked();
    void onTextEditChanegd(const QString& text);

private:
    void updateRightButton(bool showReload);

private:
    BrowserView *web_view_;
    UrlIconLabel *icon_label_;
    QColor default_base_color_;
    int progress_;
};

}

#endif // __WEBBROWSER_URLLINEEDIT_H__

