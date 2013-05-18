/****************************************************************************
** Meta object code from reading C++ file 'view.h'
**
** Created: Thu May 16 19:57:32 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "view.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'view.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_webbrowser__BrowserView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      56,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      29,   25,   24,   24, 0x05,
      46,   24,   24,   24, 0x05,
      59,   57,   24,   24, 0x05,
      93,   90,   24,   24, 0x05,
     133,   24,   24,   24, 0x05,
     205,  144,   24,   24, 0x05,
     271,   24,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
     292,   24,   24,   24, 0x0a,
     308,  144,   24,   24, 0x0a,
     436,  369,   24,   24, 0x0a,
     513,   24,   24,   24, 0x0a,
     529,   24,   24,   24, 0x0a,
     549,   24,   24,   24, 0x0a,
     563,   24,   24,   24, 0x0a,
     576,   24,   24,   24, 0x0a,
     592,   24,   24,   24, 0x08,
     608,   24,   24,   24, 0x08,
     620,   24,   24,   24, 0x08,
     651,  646,   24,   24, 0x08,
     673,   24,   24,   24, 0x08,
     693,   24,   24,   24, 0x08,
     712,   24,   24,   24, 0x08,
     728,   24,   24,   24, 0x08,
     748,   24,   24,   24, 0x08,
     769,   24,   24,   24, 0x08,
     795,   24,   24,   24, 0x08,
     824,  816,   24,   24, 0x08,
     866,  861,   24,   24, 0x08,
     898,  893,   24,   24, 0x08,
     932,  928,  923,   24, 0x08,
     958,  928,   24,   24, 0x08,
     985,   24,   24,   24, 0x08,
    1003,   24,   24,   24, 0x08,
    1020,   24,   24,   24, 0x08,
    1039,   24,   24,   24, 0x08,
    1056,   24,  923,   24, 0x08,
    1074, 1067,   24,   24, 0x08,
    1100,   24,   24,   24, 0x28,
    1122,   24,  923,   24, 0x08,
    1147,   24,   24,   24, 0x08,
    1164,   24,   24,   24, 0x08,
    1175,   24,   24,   24, 0x08,
    1182,   24,   24,   24, 0x08,
    1206,  646, 1199,   24, 0x08,
    1229, 1223,   24,   24, 0x08,
    1249, 1247,   24,   24, 0x08,
    1268,   24,   24,   24, 0x08,
    1298,   24, 1290,   24, 0x08,
    1314, 1223,   24,   24, 0x08,
    1338,   24,   24,   24, 0x08,
    1371,   24,   24,   24, 0x08,
    1381,   24,   24,   24, 0x08,
    1400,   24,   24,   24, 0x08,
    1421,   24,   24,   24, 0x08,
    1444,   24,   24,   24, 0x08,
    1460,   24,   24,   24, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_webbrowser__BrowserView[] = {
    "webbrowser::BrowserView\0\0url\0"
    "requestOTA(QUrl)\0focusOut()\0,\0"
    "progressChangedSignal(int,int)\0,,\0"
    "viewportRangeChangedSignal(int,int,int)\0"
    "showHome()\0"
    "form_id,form_name,form_action,input_type,input_id,input_name\0"
    "inputFormFocused(QString,QString,QString,QString,QString,QString)\0"
    "inputFormLostFocus()\0formLostFocus()\0"
    "formFocused(QString,QString,QString,QString,QString,QString)\0"
    "form_id,form_name,form_action,input_type,input_id,input_name,value\0"
    "formFocusedAddValue(QString,QString,QString,QString,QString,QString,QS"
    "tring)\0"
    "selectMouseUp()\0handleSelectPopup()\0"
    "selectFocus()\0selectBlur()\0selectChanged()\0"
    "updateActions()\0popupMenu()\0"
    "changeFontFamily(QString)\0size\0"
    "changeFontSize(qreal)\0onLinkClicked(QUrl)\0"
    "reloadCurrentUrl()\0onLoadStarted()\0"
    "onLoadProgress(int)\0onLoadFinished(bool)\0"
    "onRepaintRequested(QRect)\0"
    "onSelectionChanged()\0request\0"
    "onDownloadRequested(QNetworkRequest)\0"
    "data\0onSavePassword(QByteArray)\0rect\0"
    "fastUpdateWidget(QRect&)\0bool\0wnd\0"
    "isWidgetVisible(QWidget*)\0"
    "hideHelperWidget(QWidget*)\0startDictLookup()\0"
    "stopDictLookup()\0showSearchWidget()\0"
    "onSearchClosed()\0doSearch()\0enable\0"
    "enableTextSelection(bool)\0"
    "enableTextSelection()\0isTextSelectionEnabled()\0"
    "clearSelection()\0storeUrl()\0sync()\0"
    "saveThumbnails()\0QImage\0thumbnail(QSize)\0"
    "dx,dy\0myScroll(int,int)\0p\0myScrollTo(QPoint)\0"
    "updateViewportRange()\0QPointF\0"
    "currentOffset()\0accurateScroll(int,int)\0"
    "populateJavaScriptWindowObject()\0"
    "hideGif()\0hideInlineFrames()\0"
    "hideObjectElements()\0hideEmbeddedElements()\0"
    "hideScrollbar()\0clearHistory()\0"
};

void webbrowser::BrowserView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BrowserView *_t = static_cast<BrowserView *>(_o);
        switch (_id) {
        case 0: _t->requestOTA((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 1: _t->focusOut(); break;
        case 2: _t->progressChangedSignal((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 3: _t->viewportRangeChangedSignal((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 4: _t->showHome(); break;
        case 5: _t->inputFormFocused((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6]))); break;
        case 6: _t->inputFormLostFocus(); break;
        case 7: _t->formLostFocus(); break;
        case 8: _t->formFocused((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6]))); break;
        case 9: _t->formFocusedAddValue((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6])),(*reinterpret_cast< const QString(*)>(_a[7]))); break;
        case 10: _t->selectMouseUp(); break;
        case 11: _t->handleSelectPopup(); break;
        case 12: _t->selectFocus(); break;
        case 13: _t->selectBlur(); break;
        case 14: _t->selectChanged(); break;
        case 15: _t->updateActions(); break;
        case 16: _t->popupMenu(); break;
        case 17: _t->changeFontFamily((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: _t->changeFontSize((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 19: _t->onLinkClicked((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 20: _t->reloadCurrentUrl(); break;
        case 21: _t->onLoadStarted(); break;
        case 22: _t->onLoadProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->onLoadFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 24: _t->onRepaintRequested((*reinterpret_cast< const QRect(*)>(_a[1]))); break;
        case 25: _t->onSelectionChanged(); break;
        case 26: _t->onDownloadRequested((*reinterpret_cast< const QNetworkRequest(*)>(_a[1]))); break;
        case 27: _t->onSavePassword((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 28: _t->fastUpdateWidget((*reinterpret_cast< QRect(*)>(_a[1]))); break;
        case 29: { bool _r = _t->isWidgetVisible((*reinterpret_cast< QWidget*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 30: _t->hideHelperWidget((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 31: _t->startDictLookup(); break;
        case 32: _t->stopDictLookup(); break;
        case 33: _t->showSearchWidget(); break;
        case 34: _t->onSearchClosed(); break;
        case 35: { bool _r = _t->doSearch();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 36: _t->enableTextSelection((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 37: _t->enableTextSelection(); break;
        case 38: { bool _r = _t->isTextSelectionEnabled();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 39: _t->clearSelection(); break;
        case 40: _t->storeUrl(); break;
        case 41: _t->sync(); break;
        case 42: _t->saveThumbnails(); break;
        case 43: { QImage _r = _t->thumbnail((*reinterpret_cast< const QSize(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = _r; }  break;
        case 44: _t->myScroll((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 45: _t->myScrollTo((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 46: _t->updateViewportRange(); break;
        case 47: { QPointF _r = _t->currentOffset();
            if (_a[0]) *reinterpret_cast< QPointF*>(_a[0]) = _r; }  break;
        case 48: _t->accurateScroll((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 49: _t->populateJavaScriptWindowObject(); break;
        case 50: _t->hideGif(); break;
        case 51: _t->hideInlineFrames(); break;
        case 52: _t->hideObjectElements(); break;
        case 53: _t->hideEmbeddedElements(); break;
        case 54: _t->hideScrollbar(); break;
        case 55: _t->clearHistory(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData webbrowser::BrowserView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject webbrowser::BrowserView::staticMetaObject = {
    { &QWebView::staticMetaObject, qt_meta_stringdata_webbrowser__BrowserView,
      qt_meta_data_webbrowser__BrowserView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &webbrowser::BrowserView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *webbrowser::BrowserView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *webbrowser::BrowserView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_webbrowser__BrowserView))
        return static_cast<void*>(const_cast< BrowserView*>(this));
    return QWebView::qt_metacast(_clname);
}

int webbrowser::BrowserView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 56)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 56;
    }
    return _id;
}

// SIGNAL 0
void webbrowser::BrowserView::requestOTA(const QUrl & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void webbrowser::BrowserView::focusOut()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void webbrowser::BrowserView::progressChangedSignal(const int _t1, const int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void webbrowser::BrowserView::viewportRangeChangedSignal(const int _t1, const int _t2, const int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void webbrowser::BrowserView::showHome()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void webbrowser::BrowserView::inputFormFocused(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4, const QString & _t5, const QString & _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void webbrowser::BrowserView::inputFormLostFocus()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}
QT_END_MOC_NAMESPACE
