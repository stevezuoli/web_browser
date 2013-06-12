#ifndef __WEBBROWSER_URLLINEEDIT_H__
#define __WEBBROWSER_URLLINEEDIT_H__

#include <QtCore/QUrl>
#include <QtGui/QWidget>
#include <QtGui/QAbstractButton>
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

class ReaderButton : public QAbstractButton
{
    Q_OBJECT
public:
    ReaderButton(QWidget* parent = 0);

    void setBackGroundImagePaths(const QString& focusInPath, const QString& focusOutPath);
    void setPressed(bool press)
    {
        pressed_ = press;
        update();
    }

    void setReaderMode(bool is_reader_mode);
    bool isReaderMode() { return is_reader_mode_; }

protected:
    virtual void paintEvent(QPaintEvent*);
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);

private:
    void setMyStyleSheet();
    bool pressed_;
    bool is_reader_mode_;
};//ReaderButton

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

Q_SIGNALS:
    void enterReaderMode(bool enter);

public Q_SLOTS:
    void onReaderButtonClicked();
    void displayReaderButton(bool display);

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
    ReaderButton *reader_button_;
    ClearButton *clear_button_;
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

private:
    QLinearGradient generateGradient(const QColor &color) const;
    BrowserView *web_view_;
    UrlIconLabel *icon_label_;
    QColor default_base_color_;

};

}

#endif // __WEBBROWSER_URLLINEEDIT_H__

