/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <my_label.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    my_label *frame;
    QVBoxLayout *verticalLayoutControls;
    QGroupBox *groupBoxGlobal;
    QHBoxLayout *horizontalLayoutGlobal;
    QLabel *labelGrid;
    QSpinBox *spinBox;
    QLabel *lblSpeed;
    QSlider *sliderSpeed;
    QTabWidget *tabWidget;
    QWidget *tabLine;
    QVBoxLayout *verticalLayoutLine;
    QGroupBox *groupBoxLineAlgo;
    QVBoxLayout *vboxLineAlgo;
    QComboBox *comboLineAlgo;
    QGroupBox *groupBoxLineControls;
    QHBoxLayout *hboxLineControls;
    QComboBox *comboLinePoint;
    QGroupBox *groupBoxLinePerf;
    QVBoxLayout *vboxLinePerfOuter;
    QScrollArea *scrollAreaLinePerf;
    QWidget *scrollAreaWidgetContentsLine;
    QVBoxLayout *vboxLinePerf;
    QLabel *dda_time_label;
    QLabel *lblDdaCount;
    QLabel *bresenham_time_label;
    QLabel *lblBresenhamCount;
    QGroupBox *groupBoxLineActions;
    QGridLayout *gridLineActions;
    QPushButton *clear;
    QPushButton *btnAnimateLine;
    QPushButton *draw_line;
    QSpacerItem *spacerLine;
    QWidget *tabCircle;
    QVBoxLayout *verticalLayoutCircle;
    QGroupBox *groupBoxCircleAlgo;
    QVBoxLayout *vboxCircleAlgo;
    QComboBox *comboCircleAlgo;
    QGroupBox *groupBoxCircleControls;
    QGridLayout *gridCircleControls;
    QComboBox *comboCirclePoint;
    QLabel *labelRadius;
    QSpinBox *spinBoxRadius;
    QGroupBox *groupBoxCirclePerf;
    QVBoxLayout *vboxCirclePerfOuter;
    QScrollArea *scrollAreaCirclePerf;
    QWidget *scrollAreaWidgetContentsCircle;
    QVBoxLayout *vboxCirclePerf;
    QLabel *lblPolarTime;
    QLabel *lblPolarCount;
    QLabel *lblMidpointTime;
    QLabel *lblMidpointCount;
    QLabel *lblCartesianTime;
    QLabel *lblCartesianCount;
    QGroupBox *groupBoxCircleActions;
    QHBoxLayout *hboxCircleActions;
    QPushButton *btnClearCircle;
    QPushButton *btnAnimateCircle;
    QSpacerItem *spacerCircle;
    QGroupBox *groupBoxDebugger;
    QVBoxLayout *vboxDebugger;
    QTextBrowser *textDebugger;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1100, 750);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(20);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(20, 20, 20, 20);
        frame = new my_label(centralwidget);
        frame->setObjectName("frame");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setMinimumSize(QSize(600, 600));
        frame->setFrameShape(QFrame::Box);

        horizontalLayout->addWidget(frame);

        verticalLayoutControls = new QVBoxLayout();
        verticalLayoutControls->setSpacing(15);
        verticalLayoutControls->setObjectName("verticalLayoutControls");
        verticalLayoutControls->setSizeConstraint(QLayout::SetFixedSize);
        groupBoxGlobal = new QGroupBox(centralwidget);
        groupBoxGlobal->setObjectName("groupBoxGlobal");
        horizontalLayoutGlobal = new QHBoxLayout(groupBoxGlobal);
        horizontalLayoutGlobal->setSpacing(15);
        horizontalLayoutGlobal->setObjectName("horizontalLayoutGlobal");
        horizontalLayoutGlobal->setContentsMargins(15, 15, 15, 15);
        labelGrid = new QLabel(groupBoxGlobal);
        labelGrid->setObjectName("labelGrid");

        horizontalLayoutGlobal->addWidget(labelGrid);

        spinBox = new QSpinBox(groupBoxGlobal);
        spinBox->setObjectName("spinBox");
        spinBox->setMinimum(5);
        spinBox->setValue(15);

        horizontalLayoutGlobal->addWidget(spinBox);

        lblSpeed = new QLabel(groupBoxGlobal);
        lblSpeed->setObjectName("lblSpeed");

        horizontalLayoutGlobal->addWidget(lblSpeed);

        sliderSpeed = new QSlider(groupBoxGlobal);
        sliderSpeed->setObjectName("sliderSpeed");
        sliderSpeed->setOrientation(Qt::Horizontal);
        sliderSpeed->setMinimum(1);
        sliderSpeed->setMaximum(20);
        sliderSpeed->setValue(10);

        horizontalLayoutGlobal->addWidget(sliderSpeed);


        verticalLayoutControls->addWidget(groupBoxGlobal);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setMinimumSize(QSize(350, 500));
        tabLine = new QWidget();
        tabLine->setObjectName("tabLine");
        verticalLayoutLine = new QVBoxLayout(tabLine);
        verticalLayoutLine->setSpacing(15);
        verticalLayoutLine->setObjectName("verticalLayoutLine");
        verticalLayoutLine->setContentsMargins(15, 15, 15, 15);
        groupBoxLineAlgo = new QGroupBox(tabLine);
        groupBoxLineAlgo->setObjectName("groupBoxLineAlgo");
        vboxLineAlgo = new QVBoxLayout(groupBoxLineAlgo);
        vboxLineAlgo->setObjectName("vboxLineAlgo");
        vboxLineAlgo->setContentsMargins(15, 20, 15, 15);
        comboLineAlgo = new QComboBox(groupBoxLineAlgo);
        comboLineAlgo->addItem(QString());
        comboLineAlgo->addItem(QString());
        comboLineAlgo->addItem(QString());
        comboLineAlgo->setObjectName("comboLineAlgo");

        vboxLineAlgo->addWidget(comboLineAlgo);


        verticalLayoutLine->addWidget(groupBoxLineAlgo);

        groupBoxLineControls = new QGroupBox(tabLine);
        groupBoxLineControls->setObjectName("groupBoxLineControls");
        hboxLineControls = new QHBoxLayout(groupBoxLineControls);
        hboxLineControls->setObjectName("hboxLineControls");
        hboxLineControls->setContentsMargins(15, 20, 15, 15);
        comboLinePoint = new QComboBox(groupBoxLineControls);
        comboLinePoint->addItem(QString());
        comboLinePoint->addItem(QString());
        comboLinePoint->setObjectName("comboLinePoint");

        hboxLineControls->addWidget(comboLinePoint);


        verticalLayoutLine->addWidget(groupBoxLineControls);

        groupBoxLinePerf = new QGroupBox(tabLine);
        groupBoxLinePerf->setObjectName("groupBoxLinePerf");
        vboxLinePerfOuter = new QVBoxLayout(groupBoxLinePerf);
        vboxLinePerfOuter->setObjectName("vboxLinePerfOuter");
        vboxLinePerfOuter->setContentsMargins(5, 10, 5, 5);
        scrollAreaLinePerf = new QScrollArea(groupBoxLinePerf);
        scrollAreaLinePerf->setObjectName("scrollAreaLinePerf");
        scrollAreaLinePerf->setWidgetResizable(true);
        scrollAreaLinePerf->setFrameShape(QFrame::NoFrame);
        scrollAreaWidgetContentsLine = new QWidget();
        scrollAreaWidgetContentsLine->setObjectName("scrollAreaWidgetContentsLine");
        vboxLinePerf = new QVBoxLayout(scrollAreaWidgetContentsLine);
        vboxLinePerf->setSpacing(10);
        vboxLinePerf->setObjectName("vboxLinePerf");
        dda_time_label = new QLabel(scrollAreaWidgetContentsLine);
        dda_time_label->setObjectName("dda_time_label");
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        dda_time_label->setFont(font);

        vboxLinePerf->addWidget(dda_time_label);

        lblDdaCount = new QLabel(scrollAreaWidgetContentsLine);
        lblDdaCount->setObjectName("lblDdaCount");
        QFont font1;
        font1.setPointSize(10);
        lblDdaCount->setFont(font1);

        vboxLinePerf->addWidget(lblDdaCount);

        bresenham_time_label = new QLabel(scrollAreaWidgetContentsLine);
        bresenham_time_label->setObjectName("bresenham_time_label");
        bresenham_time_label->setFont(font);

        vboxLinePerf->addWidget(bresenham_time_label);

        lblBresenhamCount = new QLabel(scrollAreaWidgetContentsLine);
        lblBresenhamCount->setObjectName("lblBresenhamCount");
        lblBresenhamCount->setFont(font1);

        vboxLinePerf->addWidget(lblBresenhamCount);

        scrollAreaLinePerf->setWidget(scrollAreaWidgetContentsLine);

        vboxLinePerfOuter->addWidget(scrollAreaLinePerf);


        verticalLayoutLine->addWidget(groupBoxLinePerf);

        groupBoxLineActions = new QGroupBox(tabLine);
        groupBoxLineActions->setObjectName("groupBoxLineActions");
        gridLineActions = new QGridLayout(groupBoxLineActions);
        gridLineActions->setSpacing(10);
        gridLineActions->setObjectName("gridLineActions");
        gridLineActions->setContentsMargins(15, 20, 15, 15);
        clear = new QPushButton(groupBoxLineActions);
        clear->setObjectName("clear");

        gridLineActions->addWidget(clear, 0, 0, 1, 1);

        btnAnimateLine = new QPushButton(groupBoxLineActions);
        btnAnimateLine->setObjectName("btnAnimateLine");

        gridLineActions->addWidget(btnAnimateLine, 0, 1, 1, 1);

        draw_line = new QPushButton(groupBoxLineActions);
        draw_line->setObjectName("draw_line");

        gridLineActions->addWidget(draw_line, 1, 0, 1, 2);


        verticalLayoutLine->addWidget(groupBoxLineActions);

        spacerLine = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayoutLine->addItem(spacerLine);

        tabWidget->addTab(tabLine, QString());
        tabCircle = new QWidget();
        tabCircle->setObjectName("tabCircle");
        verticalLayoutCircle = new QVBoxLayout(tabCircle);
        verticalLayoutCircle->setSpacing(15);
        verticalLayoutCircle->setObjectName("verticalLayoutCircle");
        verticalLayoutCircle->setContentsMargins(15, 15, 15, 15);
        groupBoxCircleAlgo = new QGroupBox(tabCircle);
        groupBoxCircleAlgo->setObjectName("groupBoxCircleAlgo");
        vboxCircleAlgo = new QVBoxLayout(groupBoxCircleAlgo);
        vboxCircleAlgo->setObjectName("vboxCircleAlgo");
        vboxCircleAlgo->setContentsMargins(15, 20, 15, 15);
        comboCircleAlgo = new QComboBox(groupBoxCircleAlgo);
        comboCircleAlgo->addItem(QString());
        comboCircleAlgo->addItem(QString());
        comboCircleAlgo->addItem(QString());
        comboCircleAlgo->addItem(QString());
        comboCircleAlgo->setObjectName("comboCircleAlgo");

        vboxCircleAlgo->addWidget(comboCircleAlgo);


        verticalLayoutCircle->addWidget(groupBoxCircleAlgo);

        groupBoxCircleControls = new QGroupBox(tabCircle);
        groupBoxCircleControls->setObjectName("groupBoxCircleControls");
        gridCircleControls = new QGridLayout(groupBoxCircleControls);
        gridCircleControls->setObjectName("gridCircleControls");
        gridCircleControls->setHorizontalSpacing(15);
        gridCircleControls->setVerticalSpacing(15);
        gridCircleControls->setContentsMargins(15, 20, 15, 15);
        comboCirclePoint = new QComboBox(groupBoxCircleControls);
        comboCirclePoint->addItem(QString());
        comboCirclePoint->addItem(QString());
        comboCirclePoint->setObjectName("comboCirclePoint");

        gridCircleControls->addWidget(comboCirclePoint, 0, 0, 1, 2);

        labelRadius = new QLabel(groupBoxCircleControls);
        labelRadius->setObjectName("labelRadius");

        gridCircleControls->addWidget(labelRadius, 1, 0, 1, 1);

        spinBoxRadius = new QSpinBox(groupBoxCircleControls);
        spinBoxRadius->setObjectName("spinBoxRadius");
        spinBoxRadius->setMaximum(1000);

        gridCircleControls->addWidget(spinBoxRadius, 1, 1, 1, 1);


        verticalLayoutCircle->addWidget(groupBoxCircleControls);

        groupBoxCirclePerf = new QGroupBox(tabCircle);
        groupBoxCirclePerf->setObjectName("groupBoxCirclePerf");
        vboxCirclePerfOuter = new QVBoxLayout(groupBoxCirclePerf);
        vboxCirclePerfOuter->setObjectName("vboxCirclePerfOuter");
        vboxCirclePerfOuter->setContentsMargins(5, 10, 5, 5);
        scrollAreaCirclePerf = new QScrollArea(groupBoxCirclePerf);
        scrollAreaCirclePerf->setObjectName("scrollAreaCirclePerf");
        scrollAreaCirclePerf->setWidgetResizable(true);
        scrollAreaCirclePerf->setFrameShape(QFrame::NoFrame);
        scrollAreaWidgetContentsCircle = new QWidget();
        scrollAreaWidgetContentsCircle->setObjectName("scrollAreaWidgetContentsCircle");
        vboxCirclePerf = new QVBoxLayout(scrollAreaWidgetContentsCircle);
        vboxCirclePerf->setSpacing(10);
        vboxCirclePerf->setObjectName("vboxCirclePerf");
        lblPolarTime = new QLabel(scrollAreaWidgetContentsCircle);
        lblPolarTime->setObjectName("lblPolarTime");
        QFont font2;
        font2.setBold(true);
        lblPolarTime->setFont(font2);

        vboxCirclePerf->addWidget(lblPolarTime);

        lblPolarCount = new QLabel(scrollAreaWidgetContentsCircle);
        lblPolarCount->setObjectName("lblPolarCount");

        vboxCirclePerf->addWidget(lblPolarCount);

        lblMidpointTime = new QLabel(scrollAreaWidgetContentsCircle);
        lblMidpointTime->setObjectName("lblMidpointTime");
        lblMidpointTime->setFont(font2);

        vboxCirclePerf->addWidget(lblMidpointTime);

        lblMidpointCount = new QLabel(scrollAreaWidgetContentsCircle);
        lblMidpointCount->setObjectName("lblMidpointCount");

        vboxCirclePerf->addWidget(lblMidpointCount);

        lblCartesianTime = new QLabel(scrollAreaWidgetContentsCircle);
        lblCartesianTime->setObjectName("lblCartesianTime");
        lblCartesianTime->setFont(font2);

        vboxCirclePerf->addWidget(lblCartesianTime);

        lblCartesianCount = new QLabel(scrollAreaWidgetContentsCircle);
        lblCartesianCount->setObjectName("lblCartesianCount");

        vboxCirclePerf->addWidget(lblCartesianCount);

        scrollAreaCirclePerf->setWidget(scrollAreaWidgetContentsCircle);

        vboxCirclePerfOuter->addWidget(scrollAreaCirclePerf);


        verticalLayoutCircle->addWidget(groupBoxCirclePerf);

        groupBoxCircleActions = new QGroupBox(tabCircle);
        groupBoxCircleActions->setObjectName("groupBoxCircleActions");
        hboxCircleActions = new QHBoxLayout(groupBoxCircleActions);
        hboxCircleActions->setSpacing(15);
        hboxCircleActions->setObjectName("hboxCircleActions");
        hboxCircleActions->setContentsMargins(15, 20, 15, 15);
        btnClearCircle = new QPushButton(groupBoxCircleActions);
        btnClearCircle->setObjectName("btnClearCircle");

        hboxCircleActions->addWidget(btnClearCircle);

        btnAnimateCircle = new QPushButton(groupBoxCircleActions);
        btnAnimateCircle->setObjectName("btnAnimateCircle");

        hboxCircleActions->addWidget(btnAnimateCircle);


        verticalLayoutCircle->addWidget(groupBoxCircleActions);

        spacerCircle = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayoutCircle->addItem(spacerCircle);

        tabWidget->addTab(tabCircle, QString());

        verticalLayoutControls->addWidget(tabWidget);

        groupBoxDebugger = new QGroupBox(centralwidget);
        groupBoxDebugger->setObjectName("groupBoxDebugger");
        vboxDebugger = new QVBoxLayout(groupBoxDebugger);
        vboxDebugger->setObjectName("vboxDebugger");
        vboxDebugger->setContentsMargins(10, 15, 10, 10);
        textDebugger = new QTextBrowser(groupBoxDebugger);
        textDebugger->setObjectName("textDebugger");
        textDebugger->setMaximumHeight(150);

        vboxDebugger->addWidget(textDebugger);


        verticalLayoutControls->addWidget(groupBoxDebugger);


        horizontalLayout->addLayout(verticalLayoutControls);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1100, 24));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Computer Graphics Lab", nullptr));
        MainWindow->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"QMainWindow {\n"
"    background-color: #1e1e2e;\n"
"    color: #ffffff;\n"
"    font-size: 14px;\n"
"}\n"
"* {\n"
"    color: #ffffff;\n"
"    font-size: 14px;\n"
"}\n"
"QGroupBox {\n"
"    border: 1px solid #45475a;\n"
"    border-radius: 8px;\n"
"    margin-top: 2ex;\n"
"    font-weight: bold;\n"
"    color: #ffffff;\n"
"}\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top left;\n"
"    padding: 0 5px;\n"
"    left: 10px;\n"
"    color: #89b4fa;\n"
"}\n"
"QPushButton {\n"
"    background-color: #313244;\n"
"    border: 1px solid #45475a;\n"
"    border-radius: 6px;\n"
"    padding: 8px 16px;\n"
"    color: #ffffff;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #45475a;\n"
"    border: 1px solid #89b4fa;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #585b70;\n"
"}\n"
"QComboBox, QSpinBox {\n"
"    background-color: #313244;\n"
"    border: 1px solid #45475a;\n"
"    border-radius: 6px;\n"
"    padding: 8px;\n"
"    "
                        "color: #ffffff;\n"
"}\n"
"QLabel {\n"
"    color: #ffffff;\n"
"}\n"
"QTabWidget::pane {\n"
"    border: 1px solid #45475a;\n"
"    border-radius: 8px;\n"
"    background-color: #1e1e2e;\n"
"}\n"
"QTabBar::tab {\n"
"    background-color: #313244;\n"
"    border: 1px solid #45475a;\n"
"    padding: 10px 25px;\n"
"    margin-right: 4px;\n"
"    border-top-left-radius: 6px;\n"
"    border-top-right-radius: 6px;\n"
"    color: #ffffff;\n"
"}\n"
"QTabBar::tab:selected {\n"
"    background-color: #89b4fa;\n"
"    color: #11111b;\n"
"    font-weight: bold;\n"
"}\n"
"QTextBrowser {\n"
"    background-color: #11111b;\n"
"    border: 1px solid #45475a;\n"
"    border-radius: 6px;\n"
"    padding: 8px;\n"
"    color: #a6e3a1;\n"
"}\n"
"   ", nullptr));
        frame->setText(QString());
        groupBoxGlobal->setTitle(QCoreApplication::translate("MainWindow", "Global Settings", nullptr));
        labelGrid->setText(QCoreApplication::translate("MainWindow", "Grid:", nullptr));
        lblSpeed->setText(QCoreApplication::translate("MainWindow", "Speed: 1.0x", nullptr));
        groupBoxLineAlgo->setTitle(QCoreApplication::translate("MainWindow", "Algorithm Selection", nullptr));
        comboLineAlgo->setItemText(0, QCoreApplication::translate("MainWindow", "DDA Algorithm", nullptr));
        comboLineAlgo->setItemText(1, QCoreApplication::translate("MainWindow", "Bresenham Algorithm", nullptr));
        comboLineAlgo->setItemText(2, QCoreApplication::translate("MainWindow", "All (Overlap)", nullptr));

        groupBoxLineControls->setTitle(QCoreApplication::translate("MainWindow", "Controls", nullptr));
        comboLinePoint->setItemText(0, QCoreApplication::translate("MainWindow", "Select Point 1", nullptr));
        comboLinePoint->setItemText(1, QCoreApplication::translate("MainWindow", "Select Point 2", nullptr));

        groupBoxLinePerf->setTitle(QCoreApplication::translate("MainWindow", "Performance", nullptr));
        dda_time_label->setText(QCoreApplication::translate("MainWindow", "DDA Time: -", nullptr));
        lblDdaCount->setText(QCoreApplication::translate("MainWindow", "DDA Pixels: -", nullptr));
        bresenham_time_label->setText(QCoreApplication::translate("MainWindow", "Bresenham Time: -", nullptr));
        lblBresenhamCount->setText(QCoreApplication::translate("MainWindow", "Bresenham Pixels: -", nullptr));
        groupBoxLineActions->setTitle(QCoreApplication::translate("MainWindow", "Actions", nullptr));
        clear->setText(QCoreApplication::translate("MainWindow", "Clear Line", nullptr));
        btnAnimateLine->setText(QCoreApplication::translate("MainWindow", "Animate Line", nullptr));
        draw_line->setText(QCoreApplication::translate("MainWindow", "Draw Instantly", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabLine), QCoreApplication::translate("MainWindow", "Line Drawing", nullptr));
        groupBoxCircleAlgo->setTitle(QCoreApplication::translate("MainWindow", "Algorithm Selection", nullptr));
        comboCircleAlgo->setItemText(0, QCoreApplication::translate("MainWindow", "Polar Algorithm", nullptr));
        comboCircleAlgo->setItemText(1, QCoreApplication::translate("MainWindow", "Midpoint (Bresenham) Algorithm", nullptr));
        comboCircleAlgo->setItemText(2, QCoreApplication::translate("MainWindow", "Cartesian Algorithm", nullptr));
        comboCircleAlgo->setItemText(3, QCoreApplication::translate("MainWindow", "All (Overlap)", nullptr));

        groupBoxCircleControls->setTitle(QCoreApplication::translate("MainWindow", "Controls", nullptr));
        comboCirclePoint->setItemText(0, QCoreApplication::translate("MainWindow", "Select Center (Pt 1)", nullptr));
        comboCirclePoint->setItemText(1, QCoreApplication::translate("MainWindow", "Select Radius (Pt 2)", nullptr));

        labelRadius->setText(QCoreApplication::translate("MainWindow", "Radius:", nullptr));
        groupBoxCirclePerf->setTitle(QCoreApplication::translate("MainWindow", "Performance", nullptr));
        lblPolarTime->setText(QCoreApplication::translate("MainWindow", "Polar Time: -", nullptr));
        lblPolarCount->setText(QCoreApplication::translate("MainWindow", "Polar Pixels: -", nullptr));
        lblMidpointTime->setText(QCoreApplication::translate("MainWindow", "Midpoint Time: -", nullptr));
        lblMidpointCount->setText(QCoreApplication::translate("MainWindow", "Midpoint Pixels: -", nullptr));
        lblCartesianTime->setText(QCoreApplication::translate("MainWindow", "Cartesian Time: -", nullptr));
        lblCartesianCount->setText(QCoreApplication::translate("MainWindow", "Cartesian Pixels: -", nullptr));
        groupBoxCircleActions->setTitle(QCoreApplication::translate("MainWindow", "Actions", nullptr));
        btnClearCircle->setText(QCoreApplication::translate("MainWindow", "Clear Circle", nullptr));
        btnAnimateCircle->setText(QCoreApplication::translate("MainWindow", "Animate Symmetry", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabCircle), QCoreApplication::translate("MainWindow", "Circle Drawing", nullptr));
        groupBoxDebugger->setTitle(QCoreApplication::translate("MainWindow", "Algorithm State Debugger", nullptr));
        textDebugger->setPlaceholderText(QCoreApplication::translate("MainWindow", "Awaiting animation to trace algorithm state...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
