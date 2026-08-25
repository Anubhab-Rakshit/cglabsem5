/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mainwindow.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "showmouseposition",
        "",
        "QPoint&",
        "pos",
        "mouse_pressed",
        "mouse_dragged",
        "mouse_released",
        "mouse_wheel",
        "delta",
        "on_clear_clicked",
        "on_draw_line_clicked",
        "on_btnAnimateLine_clicked",
        "animateLineStep",
        "on_spinBox_textChanged",
        "arg1",
        "on_spinBox_valueChanged",
        "on_sliderSpeed_valueChanged",
        "value",
        "on_btnAnimateCircle_clicked",
        "animateCircleStep",
        "on_comboLineAlgo_currentIndexChanged",
        "index",
        "on_comboLinePoint_currentIndexChanged",
        "on_comboCircleAlgo_currentIndexChanged",
        "on_comboCirclePoint_currentIndexChanged",
        "on_spinBoxRadius_valueChanged",
        "on_btnClearCircle_clicked",
        "on_btnDrawEllipse_clicked",
        "on_btnAnimateEllipse_clicked",
        "animateEllipseStep",
        "on_btnClearEllipse_clicked",
        "on_comboEllipseAlgo_currentIndexChanged",
        "on_comboEllipsePoint_currentIndexChanged",
        "on_spinBoxRx_valueChanged",
        "on_spinBoxRy_valueChanged",
        "on_chkQuadColors_stateChanged",
        "on_chkRegionSplit_stateChanged",
        "on_btnCommitEllipse_clicked",
        "on_btnClearCanvas_clicked",
        "on_sliderRotation_valueChanged",
        "on_sliderThickness_valueChanged"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'showmouseposition'
        QtMocHelpers::SlotData<void(QPoint &)>(1, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'mouse_pressed'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'mouse_dragged'
        QtMocHelpers::SlotData<void(QPoint &)>(6, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'mouse_released'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'mouse_wheel'
        QtMocHelpers::SlotData<void(int)>(8, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'on_clear_clicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_draw_line_clicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnAnimateLine_clicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'animateLineStep'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_spinBox_textChanged'
        QtMocHelpers::SlotData<void(const QString &)>(14, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 15 },
        }}),
        // Slot 'on_spinBox_valueChanged'
        QtMocHelpers::SlotData<void(int)>(16, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 15 },
        }}),
        // Slot 'on_sliderSpeed_valueChanged'
        QtMocHelpers::SlotData<void(int)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Slot 'on_btnAnimateCircle_clicked'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'animateCircleStep'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_comboLineAlgo_currentIndexChanged'
        QtMocHelpers::SlotData<void(int)>(21, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
        // Slot 'on_comboLinePoint_currentIndexChanged'
        QtMocHelpers::SlotData<void(int)>(23, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
        // Slot 'on_comboCircleAlgo_currentIndexChanged'
        QtMocHelpers::SlotData<void(int)>(24, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
        // Slot 'on_comboCirclePoint_currentIndexChanged'
        QtMocHelpers::SlotData<void(int)>(25, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
        // Slot 'on_spinBoxRadius_valueChanged'
        QtMocHelpers::SlotData<void(int)>(26, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 15 },
        }}),
        // Slot 'on_btnClearCircle_clicked'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnDrawEllipse_clicked'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnAnimateEllipse_clicked'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'animateEllipseStep'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnClearEllipse_clicked'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_comboEllipseAlgo_currentIndexChanged'
        QtMocHelpers::SlotData<void(int)>(32, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
        // Slot 'on_comboEllipsePoint_currentIndexChanged'
        QtMocHelpers::SlotData<void(int)>(33, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
        // Slot 'on_spinBoxRx_valueChanged'
        QtMocHelpers::SlotData<void(int)>(34, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 15 },
        }}),
        // Slot 'on_spinBoxRy_valueChanged'
        QtMocHelpers::SlotData<void(int)>(35, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 15 },
        }}),
        // Slot 'on_chkQuadColors_stateChanged'
        QtMocHelpers::SlotData<void(int)>(36, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 15 },
        }}),
        // Slot 'on_chkRegionSplit_stateChanged'
        QtMocHelpers::SlotData<void(int)>(37, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 15 },
        }}),
        // Slot 'on_btnCommitEllipse_clicked'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnClearCanvas_clicked'
        QtMocHelpers::SlotData<void()>(39, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_sliderRotation_valueChanged'
        QtMocHelpers::SlotData<void(int)>(40, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Slot 'on_sliderThickness_valueChanged'
        QtMocHelpers::SlotData<void(int)>(41, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->showmouseposition((*reinterpret_cast<std::add_pointer_t<QPoint&>>(_a[1]))); break;
        case 1: _t->mouse_pressed(); break;
        case 2: _t->mouse_dragged((*reinterpret_cast<std::add_pointer_t<QPoint&>>(_a[1]))); break;
        case 3: _t->mouse_released(); break;
        case 4: _t->mouse_wheel((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->on_clear_clicked(); break;
        case 6: _t->on_draw_line_clicked(); break;
        case 7: _t->on_btnAnimateLine_clicked(); break;
        case 8: _t->animateLineStep(); break;
        case 9: _t->on_spinBox_textChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->on_spinBox_valueChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 11: _t->on_sliderSpeed_valueChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->on_btnAnimateCircle_clicked(); break;
        case 13: _t->animateCircleStep(); break;
        case 14: _t->on_comboLineAlgo_currentIndexChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->on_comboLinePoint_currentIndexChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->on_comboCircleAlgo_currentIndexChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 17: _t->on_comboCirclePoint_currentIndexChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 18: _t->on_spinBoxRadius_valueChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 19: _t->on_btnClearCircle_clicked(); break;
        case 20: _t->on_btnDrawEllipse_clicked(); break;
        case 21: _t->on_btnAnimateEllipse_clicked(); break;
        case 22: _t->animateEllipseStep(); break;
        case 23: _t->on_btnClearEllipse_clicked(); break;
        case 24: _t->on_comboEllipseAlgo_currentIndexChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 25: _t->on_comboEllipsePoint_currentIndexChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 26: _t->on_spinBoxRx_valueChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 27: _t->on_spinBoxRy_valueChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 28: _t->on_chkQuadColors_stateChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 29: _t->on_chkRegionSplit_stateChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 30: _t->on_btnCommitEllipse_clicked(); break;
        case 31: _t->on_btnClearCanvas_clicked(); break;
        case 32: _t->on_sliderRotation_valueChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 33: _t->on_sliderThickness_valueChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 34)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 34;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 34)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 34;
    }
    return _id;
}
QT_WARNING_POP
