#ifndef WEB_BROWSER_FRAME_H_
#define WEB_BROWSER_FRAME_H_

#include <QtGui/QtGui>
#include "Base/base.h"
#include "NetworkService/xiaomi_account_manager.h"

//#include "keyboard_dialog.h"
//#include "bookmark_model.h"
#include "ui/DKToolBar.h"
#include "ui/DKSoftKeyboardIME.h"

#include "view.h"

using namespace ui;
using namespace base;
using namespace network_service;

namespace webbrowser
{

struct BrowserKeyboardPrivate
{
    QString form_id;
    QString form_name;
    QString form_action;
    QString input_type;
    QString input_id;
    QString input_name;
};

enum BrowserKeyboardStatus
{
    KEYBOARD_FREE = -1,
    FORM_FOCUSED = 0,
    URL_INPUTTING
};

class UrlLineEdit;
class BrowserMainWindow : public QWidget 
{
    Q_OBJECT
public:
    BrowserMainWindow(QWidget *parent = 0);
    ~BrowserMainWindow();

public Q_SLOTS:
    void load(const QString & url_str, const QString & option = QString());
    void onScreenSizeChanged(int);

protected:
    virtual void keyPressEvent(QKeyEvent * ke);
    virtual void keyReleaseEvent(QKeyEvent *ke);
    virtual bool event(QEvent *e);
    virtual void closeEvent(QCloseEvent *e);
    virtual void paintEvent(QPaintEvent *e);

private Q_SLOTS:
    void onInputFormFocused(const QString & form_id,
                            const QString & form_name,
                            const QString & form_action,
                            const QString & input_type,
                            const QString & input_id,
                            const QString & input_name);
    void onInputFormLostFocus();
    void onAddressInputFocus(bool);
    void onUrlChanged(const QUrl& url);
    void onLinkClicked(const QUrl& url);
    void onLoadFinished(bool ok);
    void openUrlInAddress();

    void showHomePage();
    void showBackHistoryPage();
    void showForwardHistoryPage();
    void showMenu();
    void showSoftKeyboardIME(bool show);
    void switchKeyboardVisible();
    void setHomePageUrl(const QString& url)
    {
        m_homePageUrl = url;
    }

    void onXiaomiAccountPageChanged(const QString& message);
    void onXiaomiAccountLoadFinished(bool ok);

private:
    void loadThumbnails();
    void thumbnailModel(QStandardItemModel & model);
    void setupToolBar();
    void InitLayout();

    void setupXiaomiAccountConnect();

private:
    QToolButton                exit_tool_button_;
    QToolButton                history_back_tool_button_;
    QToolButton                history_forward_tool_button_;
    QToolButton                menu_tool_button_;
    UrlLineEdit*           address_lineedit_;
    DKToolBar*             navigation_toolbar_;
    BrowserView*           view_;
    DKSoftKeyboardIME*     keyboard_;
    QVBoxLayout            main_layout_;
    QStandardItemModel     model_;

    shared_ptr<XiaomiAccountManager>  xiaomi_account_manager_;
    QString                m_homePageUrl;
};

};   // namespace webbrowser

#endif  // WEB_BROWSER_FRAME_H_
