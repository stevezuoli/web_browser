/****************************************************************************
** Meta object code from reading C++ file 'web_application.h'
**
** Created: Thu May 16 19:57:31 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "web_application.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'web_application.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_webbrowser__WebApplication[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      43,   33,   28,   27, 0x0a,
      57,   33,   28,   27, 0x0a,
      72,   27,   27,   27, 0x0a,
      82,   27,   27,   27, 0x0a,
      98,   27,   27,   27, 0x0a,
     114,  105,   27,   27, 0x0a,
     141,   27,   27,   27, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_webbrowser__WebApplication[] = {
    "webbrowser::WebApplication\0\0bool\0"
    "path_name\0open(QString)\0close(QString)\0"
    "suspend()\0configNetwork()\0scan()\0"
    "ssid,psk\0connectTo(QString,QString)\0"
    "onWakeUp()\0"
};

void webbrowser::WebApplication::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WebApplication *_t = static_cast<WebApplication *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->open((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { bool _r = _t->close((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: _t->suspend(); break;
        case 3: _t->configNetwork(); break;
        case 4: _t->scan(); break;
        case 5: _t->connectTo((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: _t->onWakeUp(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData webbrowser::WebApplication::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject webbrowser::WebApplication::staticMetaObject = {
    { &QApplication::staticMetaObject, qt_meta_stringdata_webbrowser__WebApplication,
      qt_meta_data_webbrowser__WebApplication, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &webbrowser::WebApplication::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *webbrowser::WebApplication::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *webbrowser::WebApplication::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_webbrowser__WebApplication))
        return static_cast<void*>(const_cast< WebApplication*>(this));
    return QApplication::qt_metacast(_clname);
}

int webbrowser::WebApplication::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QApplication::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
