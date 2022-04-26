/****************************************************************************
** Meta object code from reading C++ file 'tactrola.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../tactrola.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tactrola.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MidiOut_t {
    QByteArrayData data[1];
    char stringdata0[8];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MidiOut_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MidiOut_t qt_meta_stringdata_MidiOut = {
    {
QT_MOC_LITERAL(0, 0, 7) // "MidiOut"

    },
    "MidiOut"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MidiOut[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void MidiOut::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject MidiOut::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MidiOut.data,
      qt_meta_data_MidiOut,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MidiOut::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MidiOut::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MidiOut.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MidiOut::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_TouchWidget_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TouchWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TouchWidget_t qt_meta_stringdata_TouchWidget = {
    {
QT_MOC_LITERAL(0, 0, 11) // "TouchWidget"

    },
    "TouchWidget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TouchWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void TouchWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject TouchWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TouchWidget.data,
      qt_meta_data_TouchWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TouchWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TouchWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TouchWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TouchWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_PianoWidget_t {
    QByteArrayData data[6];
    char stringdata0[56];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PianoWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PianoWidget_t qt_meta_stringdata_PianoWidget = {
    {
QT_MOC_LITERAL(0, 0, 11), // "PianoWidget"
QT_MOC_LITERAL(1, 12, 14), // "setAdjustRange"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 12), // "adjust_range"
QT_MOC_LITERAL(4, 41, 8), // "setGlide"
QT_MOC_LITERAL(5, 50, 5) // "glide"

    },
    "PianoWidget\0setAdjustRange\0\0adjust_range\0"
    "setGlide\0glide"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PianoWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x0a /* Public */,
       4,    1,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    5,

       0        // eod
};

void PianoWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PianoWidget *_t = static_cast<PianoWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setAdjustRange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->setGlide((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject PianoWidget::staticMetaObject = {
    { &TouchWidget::staticMetaObject, qt_meta_stringdata_PianoWidget.data,
      qt_meta_data_PianoWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PianoWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PianoWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PianoWidget.stringdata0))
        return static_cast<void*>(this);
    return TouchWidget::qt_metacast(_clname);
}

int PianoWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TouchWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_SliderWidget_t {
    QByteArrayData data[4];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SliderWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SliderWidget_t qt_meta_stringdata_SliderWidget = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SliderWidget"
QT_MOC_LITERAL(1, 13, 19), // "setControllerNumber"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 17) // "controller_number"

    },
    "SliderWidget\0setControllerNumber\0\0"
    "controller_number"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SliderWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void SliderWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SliderWidget *_t = static_cast<SliderWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setControllerNumber((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject SliderWidget::staticMetaObject = {
    { &TouchWidget::staticMetaObject, qt_meta_stringdata_SliderWidget.data,
      qt_meta_data_SliderWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SliderWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SliderWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SliderWidget.stringdata0))
        return static_cast<void*>(this);
    return TouchWidget::qt_metacast(_clname);
}

int SliderWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TouchWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_Button_t {
    QByteArrayData data[3];
    char stringdata0[18];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Button_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Button_t qt_meta_stringdata_Button = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Button"
QT_MOC_LITERAL(1, 7, 9), // "triggered"
QT_MOC_LITERAL(2, 17, 0) // ""

    },
    "Button\0triggered\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Button[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void Button::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Button *_t = static_cast<Button *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->triggered(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (Button::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Button::triggered)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Button::staticMetaObject = {
    { &TouchWidget::staticMetaObject, qt_meta_stringdata_Button.data,
      qt_meta_data_Button,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Button::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Button::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Button.stringdata0))
        return static_cast<void*>(this);
    return TouchWidget::qt_metacast(_clname);
}

int Button::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TouchWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void Button::triggered()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_ShiftButton_t {
    QByteArrayData data[4];
    char stringdata0[37];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ShiftButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ShiftButton_t qt_meta_stringdata_ShiftButton = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ShiftButton"
QT_MOC_LITERAL(1, 12, 12), // "stateChanged"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 10) // "is_pressed"

    },
    "ShiftButton\0stateChanged\0\0is_pressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ShiftButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

void ShiftButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ShiftButton *_t = static_cast<ShiftButton *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (ShiftButton::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ShiftButton::stateChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ShiftButton::staticMetaObject = {
    { &TouchWidget::staticMetaObject, qt_meta_stringdata_ShiftButton.data,
      qt_meta_data_ShiftButton,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ShiftButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ShiftButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ShiftButton.stringdata0))
        return static_cast<void*>(this);
    return TouchWidget::qt_metacast(_clname);
}

int ShiftButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TouchWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ShiftButton::stateChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_LatchButton_t {
    QByteArrayData data[4];
    char stringdata0[37];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LatchButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LatchButton_t qt_meta_stringdata_LatchButton = {
    {
QT_MOC_LITERAL(0, 0, 11), // "LatchButton"
QT_MOC_LITERAL(1, 12, 12), // "stateChanged"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 10) // "is_pressed"

    },
    "LatchButton\0stateChanged\0\0is_pressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LatchButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

void LatchButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LatchButton *_t = static_cast<LatchButton *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (LatchButton::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LatchButton::stateChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject LatchButton::staticMetaObject = {
    { &TouchWidget::staticMetaObject, qt_meta_stringdata_LatchButton.data,
      qt_meta_data_LatchButton,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *LatchButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LatchButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LatchButton.stringdata0))
        return static_cast<void*>(this);
    return TouchWidget::qt_metacast(_clname);
}

int LatchButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TouchWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void LatchButton::stateChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_Window_t {
    QByteArrayData data[4];
    char stringdata0[41];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Window_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Window_t qt_meta_stringdata_Window = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Window"
QT_MOC_LITERAL(1, 7, 15), // "showSetupDialog"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 16) // "toggleFullscreen"

    },
    "Window\0showSetupDialog\0\0toggleFullscreen"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Window[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x0a /* Public */,
       3,    0,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Window::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Window *_t = static_cast<Window *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showSetupDialog(); break;
        case 1: _t->toggleFullscreen(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Window::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Window.data,
      qt_meta_data_Window,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Window::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Window.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_SetupDialog_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SetupDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SetupDialog_t qt_meta_stringdata_SetupDialog = {
    {
QT_MOC_LITERAL(0, 0, 11) // "SetupDialog"

    },
    "SetupDialog"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SetupDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void SetupDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject SetupDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SetupDialog.data,
      qt_meta_data_SetupDialog,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SetupDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SetupDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SetupDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SetupDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
