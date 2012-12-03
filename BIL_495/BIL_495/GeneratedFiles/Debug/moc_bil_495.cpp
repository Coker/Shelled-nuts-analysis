/****************************************************************************
** Meta object code from reading C++ file 'bil_495.h'
**
** Created: Sun 2. Dec 22:38:25 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../bil_495.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bil_495.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BIL_495[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      18,    8,    8,    8, 0x0a,
      28,    8,    8,    8, 0x0a,
      38,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_BIL_495[] = {
    "BIL_495\0\0slottt()\0slottt2()\0slottt3()\0"
    "slottt4()\0"
};

void BIL_495::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BIL_495 *_t = static_cast<BIL_495 *>(_o);
        switch (_id) {
        case 0: _t->slottt(); break;
        case 1: _t->slottt2(); break;
        case 2: _t->slottt3(); break;
        case 3: _t->slottt4(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData BIL_495::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BIL_495::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_BIL_495,
      qt_meta_data_BIL_495, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BIL_495::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BIL_495::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BIL_495::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BIL_495))
        return static_cast<void*>(const_cast< BIL_495*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int BIL_495::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
