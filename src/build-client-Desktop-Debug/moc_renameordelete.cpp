/****************************************************************************
** Meta object code from reading C++ file 'renameordelete.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../client/view/renameordelete.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'renameordelete.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_renameOrDelete_t {
    QByteArrayData data[11];
    char stringdata0[147];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_renameOrDelete_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_renameOrDelete_t qt_meta_stringdata_renameOrDelete = {
    {
QT_MOC_LITERAL(0, 0, 14), // "renameOrDelete"
QT_MOC_LITERAL(1, 15, 10), // "sendData_2"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 12), // "ClientStuff*"
QT_MOC_LITERAL(4, 40, 11), // "Want2Close2"
QT_MOC_LITERAL(5, 52, 13), // "keyPressEvent"
QT_MOC_LITERAL(6, 66, 10), // "QKeyEvent*"
QT_MOC_LITERAL(7, 77, 13), // "receiveData_2"
QT_MOC_LITERAL(8, 91, 19), // "on_delete_2_clicked"
QT_MOC_LITERAL(9, 111, 17), // "on_rename_clicked"
QT_MOC_LITERAL(10, 129, 17) // "on_cancel_clicked"

    },
    "renameOrDelete\0sendData_2\0\0ClientStuff*\0"
    "Want2Close2\0keyPressEvent\0QKeyEvent*\0"
    "receiveData_2\0on_delete_2_clicked\0"
    "on_rename_clicked\0on_cancel_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_renameOrDelete[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   49,    2, 0x06 /* Public */,
       4,    0,   56,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   57,    2, 0x08 /* Private */,
       7,    3,   60,    2, 0x08 /* Private */,
       8,    0,   67,    2, 0x08 /* Private */,
       9,    0,   68,    2, 0x08 /* Private */,
      10,    0,   69,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString, QMetaType::QString,    2,    2,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString, QMetaType::QString,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void renameOrDelete::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        renameOrDelete *_t = static_cast<renameOrDelete *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendData_2((*reinterpret_cast< ClientStuff*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 1: _t->Want2Close2(); break;
        case 2: _t->keyPressEvent((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 3: _t->receiveData_2((*reinterpret_cast< ClientStuff*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 4: _t->on_delete_2_clicked(); break;
        case 5: _t->on_rename_clicked(); break;
        case 6: _t->on_cancel_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ClientStuff* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ClientStuff* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (renameOrDelete::*_t)(ClientStuff * , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&renameOrDelete::sendData_2)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (renameOrDelete::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&renameOrDelete::Want2Close2)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject renameOrDelete::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_renameOrDelete.data,
      qt_meta_data_renameOrDelete,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *renameOrDelete::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *renameOrDelete::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_renameOrDelete.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int renameOrDelete::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void renameOrDelete::sendData_2(ClientStuff * _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void renameOrDelete::Want2Close2()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
