/****************************************************************************
** Meta object code from reading C++ file 'CMainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CMainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CMainWindow_t {
    QByteArrayData data[18];
    char stringdata[370];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CMainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CMainWindow_t qt_meta_stringdata_CMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 31),
QT_MOC_LITERAL(2, 44, 0),
QT_MOC_LITERAL(3, 45, 16),
QT_MOC_LITERAL(4, 62, 4),
QT_MOC_LITERAL(5, 67, 6),
QT_MOC_LITERAL(6, 74, 22),
QT_MOC_LITERAL(7, 97, 4),
QT_MOC_LITERAL(8, 102, 26),
QT_MOC_LITERAL(9, 129, 26),
QT_MOC_LITERAL(10, 156, 28),
QT_MOC_LITERAL(11, 185, 7),
QT_MOC_LITERAL(12, 193, 24),
QT_MOC_LITERAL(13, 218, 30),
QT_MOC_LITERAL(14, 249, 27),
QT_MOC_LITERAL(15, 277, 29),
QT_MOC_LITERAL(16, 307, 27),
QT_MOC_LITERAL(17, 335, 33)
    },
    "CMainWindow\0on_SampleTree_itemDoubleClicked\0"
    "\0QTreeWidgetItem*\0item\0column\0"
    "on_ActionSolid_toggled\0arg1\0"
    "on_ActionWireframe_toggled\0"
    "on_ActionWireframe_changed\0"
    "on_ActionWireframe_triggered\0checked\0"
    "on_ActionSolid_triggered\0"
    "on_ActionDebugWindow_triggered\0"
    "on_ActionCullNone_triggered\0"
    "on_ActionCullSphere_triggered\0"
    "on_ActionCullAABB_triggered\0"
    "on_ActionCullSphereAABB_triggered\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   69,    2, 0x08,
       6,    1,   74,    2, 0x08,
       8,    1,   77,    2, 0x08,
       9,    0,   80,    2, 0x08,
      10,    1,   81,    2, 0x08,
      12,    1,   84,    2, 0x08,
      13,    0,   87,    2, 0x08,
      14,    1,   88,    2, 0x08,
      15,    1,   91,    2, 0x08,
      16,    1,   94,    2, 0x08,
      17,    1,   97,    2, 0x08,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,

       0        // eod
};

void CMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CMainWindow *_t = static_cast<CMainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_SampleTree_itemDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->on_ActionSolid_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_ActionWireframe_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_ActionWireframe_changed(); break;
        case 4: _t->on_ActionWireframe_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->on_ActionSolid_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->on_ActionDebugWindow_triggered(); break;
        case 7: _t->on_ActionCullNone_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->on_ActionCullSphere_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->on_ActionCullAABB_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->on_ActionCullSphereAABB_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CMainWindow.data,
      qt_meta_data_CMainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *CMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CMainWindow.stringdata))
        return static_cast<void*>(const_cast< CMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
