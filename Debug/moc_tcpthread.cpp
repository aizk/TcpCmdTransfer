/****************************************************************************
** Meta object code from reading C++ file 'tcpthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../tcpthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TcpThread_t {
    QByteArrayData data[12];
    char stringdata0[152];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TcpThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TcpThread_t qt_meta_stringdata_TcpThread = {
    {
QT_MOC_LITERAL(0, 0, 9), // "TcpThread"
QT_MOC_LITERAL(1, 10, 25), // "handleNewSocketDescriptor"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 7), // "qintptr"
QT_MOC_LITERAL(4, 45, 16), // "socketDescriptor"
QT_MOC_LITERAL(5, 62, 14), // "handleCmdReady"
QT_MOC_LITERAL(6, 77, 5), // "devId"
QT_MOC_LITERAL(7, 83, 10), // "TcpSocket*"
QT_MOC_LITERAL(8, 94, 10), // "pTcpSocket"
QT_MOC_LITERAL(9, 105, 4), // "data"
QT_MOC_LITERAL(10, 110, 22), // "handleSocketDisconnect"
QT_MOC_LITERAL(11, 133, 18) // "handleToDisconnect"

    },
    "TcpThread\0handleNewSocketDescriptor\0"
    "\0qintptr\0socketDescriptor\0handleCmdReady\0"
    "devId\0TcpSocket*\0pTcpSocket\0data\0"
    "handleSocketDisconnect\0handleToDisconnect"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TcpThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x0a /* Public */,
       5,    3,   37,    2, 0x09 /* Protected */,
      10,    2,   44,    2, 0x09 /* Protected */,
      11,    1,   49,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::UInt, 0x80000000 | 7, QMetaType::QByteArray,    6,    8,    9,
    QMetaType::Void, QMetaType::UInt, 0x80000000 | 7,    6,    8,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void TcpThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TcpThread *_t = static_cast<TcpThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handleNewSocketDescriptor((*reinterpret_cast< qintptr(*)>(_a[1]))); break;
        case 1: _t->handleCmdReady((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< TcpSocket*(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 2: _t->handleSocketDisconnect((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< TcpSocket*(*)>(_a[2]))); break;
        case 3: _t->handleToDisconnect((*reinterpret_cast< TcpSocket*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TcpSocket* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TcpSocket* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TcpSocket* >(); break;
            }
            break;
        }
    }
}

const QMetaObject TcpThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_TcpThread.data,
      qt_meta_data_TcpThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TcpThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TcpThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TcpThread.stringdata0))
        return static_cast<void*>(const_cast< TcpThread*>(this));
    return QThread::qt_metacast(_clname);
}

int TcpThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
