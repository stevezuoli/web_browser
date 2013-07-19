#ifndef WEB_BROWSER_FRAME_H_
#define WEB_BROWSER_FRAME_H_

#include <QtGui/QtGui>
#include "Base/base.h"
#include "NetworkService/xiaomi_account_manager.h"
#include "NetworkService/evernote_account_manager.h"
#include "NetworkService/xiaomi_migration_manager.h"

#include "ui/DKMenu.h"
#include "ui/DKLabel.h"
#include "ui/DKToolBar.h"
#include "ui/DKSoftKeyboardIME.h"

#include "web_browser/view.h"
#include "web_browser/history_page.h"

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
    virtual bool eventFilter(QObject* watched, QEvent* event);

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
    void onBackKeyPressed();

    void showBackHistoryPage();
    void showForwardHistoryPage();
    void switchMenuVisibility();
    void showSoftKeyboardIME(bool show);
    void switchKeyboardVisibility();
    void setHomePageUrl(const QString& url)
    {
        home_page_url_ = url;
    }

    void onAccountPageChanged(const QString& message);
    void onXiaomiAccountLoadFinished(bool ok);
    void onXiaomiMigrationFinished();
    // Evernote account
    void onEvernoteAccountLoadFinished(bool ok);
    

    //set focus in address edit when keyboard key pressed
    void onKeyboardKeyPressed();

    // reader mode
    void onReaderModeToggled();

    // mouse mode
    void onMouseModeToggled();

    // text displayed on line edit
    void onLineEditTextChanged(const QString& message);

    void showHomePage();
    void showHistoryPage(bool show = true);
    void hideHistoryPage(const QUrl&);
    void bookmarkThisPage();
    void showBookmarkPage();
    void showSettingsPage();
    void printScreen();

    // exit
    void exitBrowser();

    //zoom text
    void zoomIn();
    void zoomOut();

    void onViewScaleBegin();
    void onViewScaleEnd();

    //mosue-key mode shift
    void shiftMouseMode(bool mouse = true);

    void onKeyboardVisibleChanged(bool);
private:
    void loadThumbnails();
    void thumbnailModel(QStandardItemModel & model);
    void setupToolBar();
    void showOnlyTitleOnToolBar(bool show);
    void setupMenu();
    void InitLayout();

    void setupXiaomiAccountConnect();
    void setupEvernoteAccountConnection();
    void setupXiaomiMigrationConnection();
    
    void updateBackForwardButtonStatus();
    void updateMenuStatusInSpecialMode(bool inSpecialMode);
    void updateMenuStatusInHistoryPage(bool inHistoryPage);
    void updateMenuStatusOnZoomFactorChanged();

private:
    enum MenuAction
    {
        MA_BookStore,
        MA_History,
        MA_Mouse,
        MA_Zoom_In,
        MA_Zoom_Out,
        MA_Reader_Mode,
        MA_Exit,
        MA_Count
    };

    enum ToolBarAction
    {
        TBA_Back,
        TBA_Forward,
        TBA_Edit,
        TBA_Keyboard,
        TBA_Menu,
        TBA_Title,
        TBA_Space,
        TBA_Count
    };
    QToolButton                history_back_tool_button_;
    QToolButton                history_forward_tool_button_;
    QToolButton                menu_tool_button_;
    QToolButton                keyboard_button_;
    DKLabel                    title_label_;
    
    UrlLineEdit*               address_lineedit_;
    DKToolBar*                 navigation_toolbar_;
    BrowserView*               view_;
    DKSoftKeyboardIME*         keyboard_;
    HistoryPage*               history_page_;
    DKMenu                     menu_;
    QVBoxLayout                main_layout_;
    QAction*                   menu_actions[MA_Count];
    QAction*                   toolbar_actions_[TBA_Count];

    shared_ptr<XiaomiAccountManager>  xiaomi_account_manager_;
    shared_ptr<EvernoteAccountManager> evernote_account_manager_;
    shared_ptr<XiaomiMigrationManager> xiaomi_migration_manager_;
    QString                    home_page_url_;
    bool                       reader_mode_;
    bool                       mouse_mode_;
};

};   // namespace webbrowser

#endif  // WEB_BROWSER_FRAME_H_
