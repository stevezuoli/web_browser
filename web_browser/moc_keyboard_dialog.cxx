/****************************************************************************
** Meta object code from reading C++ file 'keyboard_dialog.h'
**
** Created: Thu May 16 19:57:32 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "keyboard_dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'keyboard_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_webbrowser__KeyboardDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      33,   28,   27,   27, 0x05,

 // slots: signature, parameters, type, tag, flags
      55,   27,   27,   27, 0x08,
      69,   27,   27,   27, 0x08,
      81,   27,   27,   27, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_webbrowser__KeyboardDialog[] = {
    "webbrowser::KeyboardDialog\0\0text\0"
    "textFinsihed(QString)\0onOKClicked()\0"
    "onTimeout()\0onClose()\0"
};

void webbrowser::KeyboardDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        KeyboardDialog *_t = static_cast<KeyboardDialog *>(_o);
        switch (_id) {
        case 0: _t->textFinsihed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->onOKClicked(); break;
        case 2: _t->onTimeout(); break;
        case 3: _t->onClose(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData webbrowser::KeyboardDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject webbrowser::KeyboardDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_webbrowser__KeyboardDialog,
      qt_meta_data_webbrowser__KeyboardDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &webbrowser::KeyboardDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *webbrowser::KeyboardDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *webbrowser::KeyboardDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_webbrowser__KeyboardDialog))
        return static_cast<void*>(const_cast< KeyboardDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int webbrowser::KeyboardDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void webbrowser::KeyboardDialog::textFinsihed(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
