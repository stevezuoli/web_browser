
#ifndef WEB_BROWSER_FRAME_H_
#define WEB_BROWSER_FRAME_H_

#include <QtGui/QtGui>
#include "NetworkService/xiaomi_account_manager.h"

#include "view.h"
//#include "keyboard_dialog.h"
//#include "bookmark_model.h"
#include "url_lineedit.h"
#include "ui/DKToolBar.h"

using namespace ui;

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

class BrowserMainWindow : public QWidget 
{
    Q_OBJECT

public:
    BrowserMainWindow(QWidget *parent = 0);
    ~BrowserMainWindow();

public Q_SLOTS:
    void load(const QString & url_str);
    void onScreenSizeChanged(int);

protected:
    void keyPressEvent(QKeyEvent * ke);
    void keyReleaseEvent(QKeyEvent *ke);
    bool event(QEvent *e);
    void closeEvent(QCloseEvent *e);

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
    void openUrlInAddress();

    void showHomePage();
    void showBackHistoryPage();
    void showForwardHistoryPage();
    void showMenu();
    void showSoftKeyboardIME(bool show);

    void setHomePageUrl(const QString& url)
    {
        m_homePageUrl = url;
    }
private:
    void loadThumbnails();
    void thumbnailModel(QStandardItemModel & model);
    void setupToolBar();
    void InitLayout();

private:
    QAction                homepage_action_;
    QAction                history_back_action_;
    QAction                history_forward_action_;
    QAction                menu_action_;
    UrlLineEdit            address_lineedit_;
    DKToolBar              navigation_toolbar_;
    BrowserView            view_;
    QVBoxLayout            main_layout_;
    //KeyboardDialog         keyboard_;
    QStandardItemModel     model_;
    //BrowserKeyboardPrivate keyboard_priv_;
    //BrowserKeyboardStatus  keyboard_status_;

    XiaomiAccountManager   xiaomi_account_manager_;
    QString                m_homePageUrl;
};

}   // namespace webbrowser

#endif  // WEB_BROWSER_FRAME_H_
