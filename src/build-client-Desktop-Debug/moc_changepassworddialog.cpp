/****************************************************************************
** Meta object code from reading C++ file 'changepassworddialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../client/view/changepassworddialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'changepassworddialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ChangePasswordDialog_t {
    QByteArrayData data[9];
    char stringdata0[117];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChangePasswordDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChangePasswordDialog_t qt_meta_stringdata_ChangePasswordDialog = {
    {
QT_MOC_LITERAL(0, 0, 20), // "ChangePasswordDialog"
QT_MOC_LITERAL(1, 21, 13), // "on_ok_clicked"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 17), // "on_cancel_clicked"
QT_MOC_LITERAL(4, 54, 11), // "receiveData"
QT_MOC_LITERAL(5, 66, 12), // "ClientStuff*"
QT_MOC_LITERAL(6, 79, 13), // "keyPressEvent"
QT_MOC_LITERAL(7, 93, 10), // "QKeyEvent*"
QT_MOC_LITERAL(8, 104, 12) // "isPswChanged"

    },
    "ChangePasswordDialog\0on_ok_clicked\0\0"
    "on_cancel_clicked\0receiveData\0"
    "ClientStuff*\0keyPressEvent\0QKeyEvent*\0"
    "isPswChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChangePasswordDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08 /* Private */,
       3,    0,   40,    2, 0x08 /* Private */,
       4,    2,   41,    2, 0x08 /* Private */,
       6,    1,   46,    2, 0x08 /* Private */,
       8,    1,   49,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5, QMetaType::QString,    2,    2,
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void ChangePasswordDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ChangePasswordDialog *_t = static_cast<ChangePasswordDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_ok_clicked(); break;
        case 1: _t->on_cancel_clicked(); break;
        case 2: _t->receiveData((*reinterpret_cast< ClientStuff*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->keyPressEvent((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 4: _t->isPswChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ClientStuff* >(); break;
            }
            break;
        }
    }
}

const QMetaObject ChangePasswordDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ChangePasswordDialog.data,
      qt_meta_data_ChangePasswordDialog,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ChangePasswordDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChangePasswordDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ChangePasswordDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ChangePasswordDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
