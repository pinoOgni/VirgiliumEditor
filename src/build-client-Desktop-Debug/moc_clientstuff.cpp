/****************************************************************************
** Meta object code from reading C++ file 'clientstuff.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../client/clientstuff.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clientstuff.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ClientStuff_t {
    QByteArrayData data[41];
    char stringdata0[555];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ClientStuff_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ClientStuff_t qt_meta_stringdata_ClientStuff = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ClientStuff"
QT_MOC_LITERAL(1, 12, 11), // "hasReadSome"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 3), // "msg"
QT_MOC_LITERAL(4, 29, 10), // "tryToLogin"
QT_MOC_LITERAL(5, 40, 6), // "logged"
QT_MOC_LITERAL(6, 47, 11), // "tryToSignup"
QT_MOC_LITERAL(7, 59, 10), // "registered"
QT_MOC_LITERAL(8, 70, 13), // "getFilesOwner"
QT_MOC_LITERAL(9, 84, 3), // "row"
QT_MOC_LITERAL(10, 88, 26), // "std::vector<FilesMessage>&"
QT_MOC_LITERAL(11, 115, 12), // "filesMessage"
QT_MOC_LITERAL(12, 128, 11), // "getInfoUser"
QT_MOC_LITERAL(13, 140, 12), // "UserMessage&"
QT_MOC_LITERAL(14, 153, 1), // "u"
QT_MOC_LITERAL(15, 155, 12), // "getUserFiles"
QT_MOC_LITERAL(16, 168, 13), // "isFileRenamed"
QT_MOC_LITERAL(17, 182, 13), // "isFileDeleted"
QT_MOC_LITERAL(18, 196, 12), // "isPswChanged"
QT_MOC_LITERAL(19, 209, 13), // "isFileCreated"
QT_MOC_LITERAL(20, 223, 19), // "isCollaboratorAdded"
QT_MOC_LITERAL(21, 243, 21), // "isCollaboratorRemoved"
QT_MOC_LITERAL(22, 265, 14), // "isUnsubscribed"
QT_MOC_LITERAL(23, 280, 10), // "getAllData"
QT_MOC_LITERAL(24, 291, 19), // "processBasicMessage"
QT_MOC_LITERAL(25, 311, 12), // "BasicMessage"
QT_MOC_LITERAL(26, 324, 21), // "processLoginAndSignup"
QT_MOC_LITERAL(27, 346, 18), // "processUserMessage"
QT_MOC_LITERAL(28, 365, 11), // "UserMessage"
QT_MOC_LITERAL(29, 377, 19), // "processFilesMessage"
QT_MOC_LITERAL(30, 397, 25), // "std::vector<FilesMessage>"
QT_MOC_LITERAL(31, 423, 14), // "processAllData"
QT_MOC_LITERAL(32, 438, 4), // "code"
QT_MOC_LITERAL(33, 443, 11), // "userMessage"
QT_MOC_LITERAL(34, 455, 4), // "row1"
QT_MOC_LITERAL(35, 460, 10), // "filesOwner"
QT_MOC_LITERAL(36, 471, 4), // "row2"
QT_MOC_LITERAL(37, 476, 12), // "filesCollabs"
QT_MOC_LITERAL(38, 489, 21), // "processFileManagement"
QT_MOC_LITERAL(39, 511, 21), // "processChangePassword"
QT_MOC_LITERAL(40, 533, 21) // "processUserManagement"

    },
    "ClientStuff\0hasReadSome\0\0msg\0tryToLogin\0"
    "logged\0tryToSignup\0registered\0"
    "getFilesOwner\0row\0std::vector<FilesMessage>&\0"
    "filesMessage\0getInfoUser\0UserMessage&\0"
    "u\0getUserFiles\0isFileRenamed\0isFileDeleted\0"
    "isPswChanged\0isFileCreated\0"
    "isCollaboratorAdded\0isCollaboratorRemoved\0"
    "isUnsubscribed\0getAllData\0processBasicMessage\0"
    "BasicMessage\0processLoginAndSignup\0"
    "processUserMessage\0UserMessage\0"
    "processFilesMessage\0std::vector<FilesMessage>\0"
    "processAllData\0code\0userMessage\0row1\0"
    "filesOwner\0row2\0filesCollabs\0"
    "processFileManagement\0processChangePassword\0"
    "processUserManagement"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ClientStuff[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      14,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  124,    2, 0x06 /* Public */,
       4,    1,  127,    2, 0x06 /* Public */,
       6,    1,  130,    2, 0x06 /* Public */,
       8,    2,  133,    2, 0x06 /* Public */,
      12,    1,  138,    2, 0x06 /* Public */,
      15,    2,  141,    2, 0x06 /* Public */,
      16,    1,  146,    2, 0x06 /* Public */,
      17,    1,  149,    2, 0x06 /* Public */,
      18,    1,  152,    2, 0x06 /* Public */,
      19,    1,  155,    2, 0x06 /* Public */,
      20,    1,  158,    2, 0x06 /* Public */,
      21,    1,  161,    2, 0x06 /* Public */,
      22,    1,  164,    2, 0x06 /* Public */,
      23,    5,  167,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      24,    2,  178,    2, 0x0a /* Public */,
      26,    1,  183,    2, 0x0a /* Public */,
      27,    2,  186,    2, 0x0a /* Public */,
      29,    2,  191,    2, 0x0a /* Public */,
      31,    6,  196,    2, 0x0a /* Public */,
      38,    1,  209,    2, 0x0a /* Public */,
      39,    1,  212,    2, 0x0a /* Public */,
      40,    1,  215,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 10,    9,   11,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 10,    9,   11,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, 0x80000000 | 13, QMetaType::LongLong, 0x80000000 | 10, QMetaType::LongLong, 0x80000000 | 10,    2,    2,    2,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::LongLong, 0x80000000 | 25,    2,    2,
    QMetaType::Void, QMetaType::LongLong,    2,
    QMetaType::Void, QMetaType::LongLong, 0x80000000 | 28,    2,    2,
    QMetaType::Void, QMetaType::LongLong, 0x80000000 | 30,    2,    2,
    QMetaType::Void, QMetaType::LongLong, 0x80000000 | 28, QMetaType::LongLong, 0x80000000 | 30, QMetaType::LongLong, 0x80000000 | 30,   32,   33,   34,   35,   36,   37,
    QMetaType::Void, QMetaType::LongLong,    2,
    QMetaType::Void, QMetaType::LongLong,    2,
    QMetaType::Void, QMetaType::LongLong,    2,

       0        // eod
};

void ClientStuff::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ClientStuff *_t = static_cast<ClientStuff *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->hasReadSome((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->tryToLogin((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->tryToSignup((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->getFilesOwner((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< std::vector<FilesMessage>(*)>(_a[2]))); break;
        case 4: _t->getInfoUser((*reinterpret_cast< UserMessage(*)>(_a[1]))); break;
        case 5: _t->getUserFiles((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< std::vector<FilesMessage>(*)>(_a[2]))); break;
        case 6: _t->isFileRenamed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->isFileDeleted((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->isPswChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->isFileCreated((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->isCollaboratorAdded((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->isCollaboratorRemoved((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->isUnsubscribed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->getAllData((*reinterpret_cast< UserMessage(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2])),(*reinterpret_cast< std::vector<FilesMessage>(*)>(_a[3])),(*reinterpret_cast< qint64(*)>(_a[4])),(*reinterpret_cast< std::vector<FilesMessage>(*)>(_a[5]))); break;
        case 14: _t->processBasicMessage((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< BasicMessage(*)>(_a[2]))); break;
        case 15: _t->processLoginAndSignup((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 16: _t->processUserMessage((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< UserMessage(*)>(_a[2]))); break;
        case 17: _t->processFilesMessage((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< std::vector<FilesMessage>(*)>(_a[2]))); break;
        case 18: _t->processAllData((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< UserMessage(*)>(_a[2])),(*reinterpret_cast< qint64(*)>(_a[3])),(*reinterpret_cast< std::vector<FilesMessage>(*)>(_a[4])),(*reinterpret_cast< qint64(*)>(_a[5])),(*reinterpret_cast< std::vector<FilesMessage>(*)>(_a[6]))); break;
        case 19: _t->processFileManagement((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 20: _t->processChangePassword((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 21: _t->processUserManagement((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (ClientStuff::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientStuff::hasReadSome)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ClientStuff::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientStuff::tryToLogin)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (ClientStuff::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientStuff::tryToSignup)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (ClientStuff::*_t)(int , std::vector<FilesMessage> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientStuff::getFilesOwner)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (ClientStuff::*_t)(UserMessage & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientStuff::getInfoUser)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (ClientStuff::*_t)(int , std::vector<FilesMessage> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientStuff::getUserFiles)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (ClientStuff::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientStuff::isFileRenamed)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (ClientStuff::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientStuff::isFileDeleted)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (ClientStuff::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientStuff::isPswChanged)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (ClientStuff::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientStuff::isFileCreated)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (ClientStuff::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientStuff::isCollaboratorAdded)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (ClientStuff::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientStuff::isCollaboratorRemoved)) {
                *result = 11;
                return;
            }
        }
        {
            typedef void (ClientStuff::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientStuff::isUnsubscribed)) {
                *result = 12;
                return;
            }
        }
        {
            typedef void (ClientStuff::*_t)(UserMessage & , qint64 , std::vector<FilesMessage> & , qint64 , std::vector<FilesMessage> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientStuff::getAllData)) {
                *result = 13;
                return;
            }
        }
    }
}

const QMetaObject ClientStuff::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ClientStuff.data,
      qt_meta_data_ClientStuff,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ClientStuff::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClientStuff::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ClientStuff.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ClientStuff::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void ClientStuff::hasReadSome(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ClientStuff::tryToLogin(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ClientStuff::tryToSignup(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ClientStuff::getFilesOwner(int _t1, std::vector<FilesMessage> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ClientStuff::getInfoUser(UserMessage & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ClientStuff::getUserFiles(int _t1, std::vector<FilesMessage> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ClientStuff::isFileRenamed(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void ClientStuff::isFileDeleted(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void ClientStuff::isPswChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void ClientStuff::isFileCreated(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void ClientStuff::isCollaboratorAdded(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void ClientStuff::isCollaboratorRemoved(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void ClientStuff::isUnsubscribed(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void ClientStuff::getAllData(UserMessage & _t1, qint64 _t2, std::vector<FilesMessage> & _t3, qint64 _t4, std::vector<FilesMessage> & _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
