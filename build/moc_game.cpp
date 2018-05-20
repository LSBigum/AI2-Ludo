/****************************************************************************
** Meta object code from reading C++ file 'game.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../ludo/game.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'game.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_game[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x05,
      40,    5,    5,    5, 0x05,
      74,    5,    5,    5, 0x05,
     108,    5,    5,    5, 0x05,
     142,    5,    5,    5, 0x05,
     172,    5,    5,    5, 0x05,
     202,    5,    5,    5, 0x05,
     232,    5,    5,    5, 0x05,
     262,    5,    5,    5, 0x05,
     296,    5,    5,    5, 0x05,
     311,    5,    5,    5, 0x05,
     332,    5,    5,    5, 0x05,
     352,    5,    5,    5, 0x05,

 // slots: signature, parameters, type, tag, flags
     364,  360,    5,    5, 0x0a,
     398,  383,    5,    5, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_game[] = {
    "game\0\0player1_start(positions_and_dice)\0"
    "player2_start(positions_and_dice)\0"
    "player3_start(positions_and_dice)\0"
    "player4_start(positions_and_dice)\0"
    "player1_end(std::vector<int>)\0"
    "player2_end(std::vector<int>)\0"
    "player3_end(std::vector<int>)\0"
    "player4_end(std::vector<int>)\0"
    "update_graphics(std::vector<int>)\0"
    "set_color(int)\0set_dice_result(int)\0"
    "declare_winner(int)\0close()\0win\0"
    "turnComplete(bool)\0relative_piece\0"
    "movePiece(int)\0"
};

void game::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        game *_t = static_cast<game *>(_o);
        switch (_id) {
        case 0: _t->player1_start((*reinterpret_cast< positions_and_dice(*)>(_a[1]))); break;
        case 1: _t->player2_start((*reinterpret_cast< positions_and_dice(*)>(_a[1]))); break;
        case 2: _t->player3_start((*reinterpret_cast< positions_and_dice(*)>(_a[1]))); break;
        case 3: _t->player4_start((*reinterpret_cast< positions_and_dice(*)>(_a[1]))); break;
        case 4: _t->player1_end((*reinterpret_cast< std::vector<int>(*)>(_a[1]))); break;
        case 5: _t->player2_end((*reinterpret_cast< std::vector<int>(*)>(_a[1]))); break;
        case 6: _t->player3_end((*reinterpret_cast< std::vector<int>(*)>(_a[1]))); break;
        case 7: _t->player4_end((*reinterpret_cast< std::vector<int>(*)>(_a[1]))); break;
        case 8: _t->update_graphics((*reinterpret_cast< std::vector<int>(*)>(_a[1]))); break;
        case 9: _t->set_color((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->set_dice_result((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->declare_winner((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->close(); break;
        case 13: _t->turnComplete((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->movePiece((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData game::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject game::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_game,
      qt_meta_data_game, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &game::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *game::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *game::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_game))
        return static_cast<void*>(const_cast< game*>(this));
    return QThread::qt_metacast(_clname);
}

int game::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void game::player1_start(positions_and_dice _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void game::player2_start(positions_and_dice _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void game::player3_start(positions_and_dice _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void game::player4_start(positions_and_dice _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void game::player1_end(std::vector<int> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void game::player2_end(std::vector<int> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void game::player3_end(std::vector<int> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void game::player4_end(std::vector<int> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void game::update_graphics(std::vector<int> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void game::set_color(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void game::set_dice_result(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void game::declare_winner(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void game::close()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}
QT_END_MOC_NAMESPACE
