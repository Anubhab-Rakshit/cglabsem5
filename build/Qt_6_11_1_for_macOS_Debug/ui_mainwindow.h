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
#include <QtWidgets/QCheckBox>
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
    QVBoxLayout *vboxCP;
    QScrollArea *scrollC;
    QWidget *sw_c;
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
    QWidget *tabEllipse;
    QVBoxLayout *verticalLayoutEllipse;
    QGroupBox *groupBoxEllipseAlgo;
    QVBoxLayout *vboxEllipseAlgo;
    QComboBox *comboEllipseAlgo;
    QGroupBox *groupBoxEllipseControls;
    QGridLayout *gridEllipseControls;
    QComboBox *comboEllipsePoint;
    QLabel *labelRx;
    QSpinBox *spinBoxRx;
    QLabel *labelRy;
    QSpinBox *spinBoxRy;
    QCheckBox *chkQuadColors;
    QCheckBox *chkRegionSplit;
    QLabel *labelRotation;
    QSlider *sliderRotation;
    QLabel *labelThickness;
    QSlider *sliderThickness;
    QGroupBox *groupBoxEllipsePerf;
    QVBoxLayout *vboxEP;
    QScrollArea *scrollE;
    QWidget *sw_e;
    QVBoxLayout *vboxEllipsePerf;
    QLabel *lblEllipsePolarTime;
    QLabel *lblEllipsePolarCount;
    QLabel *lblEllipseMidpointTime;
    QLabel *lblEllipseMidpointCount;
    QLabel *lblEllipseCartesianTime;
    QLabel *lblEllipseCartesianCount;
    QLabel *lblEllipseCompare;
    QLabel *lblEllipseArea;
    QLabel *lblEllipsePerimeter;
    QGroupBox *groupBoxEllipseActions;
    QGridLayout *gridEllipseActions;
    QPushButton *btnDrawEllipse;
    QPushButton *btnAnimateEllipse;
    QPushButton *btnClearEllipse;
    QPushButton *btnCommitEllipse;
    QPushButton *btnClearCanvas;
    QGroupBox *groupBoxDebugger;
    QVBoxLayout *vboxDebugger;
    QTextBrowser *textDebugger;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 800);
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
        frame->setFrameShape(QFrame::NoFrame);

        horizontalLayout->addWidget(frame);

        verticalLayoutControls = new QVBoxLayout();
        verticalLayoutControls->setSpacing(15);
        verticalLayoutControls->setObjectName("verticalLayoutControls");
        groupBoxGlobal = new QGroupBox(centralwidget);
        groupBoxGlobal->setObjectName("groupBoxGlobal");
        horizontalLayoutGlobal = new QHBoxLayout(groupBoxGlobal);
        horizontalLayoutGlobal->setObjectName("horizontalLayoutGlobal");
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
        sliderSpeed->setMaximum(50);
        sliderSpeed->setValue(10);

        horizontalLayoutGlobal->addWidget(sliderSpeed);


        verticalLayoutControls->addWidget(groupBoxGlobal);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabLine = new QWidget();
        tabLine->setObjectName("tabLine");
        verticalLayoutLine = new QVBoxLayout(tabLine);
        verticalLayoutLine->setObjectName("verticalLayoutLine");
        groupBoxLineAlgo = new QGroupBox(tabLine);
        groupBoxLineAlgo->setObjectName("groupBoxLineAlgo");
        vboxLineAlgo = new QVBoxLayout(groupBoxLineAlgo);
        vboxLineAlgo->setObjectName("vboxLineAlgo");
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
        comboLinePoint = new QComboBox(groupBoxLineControls);
        comboLinePoint->addItem(QString());
        comboLinePoint->addItem(QString());
        comboLinePoint->setObjectName("comboLinePoint");

        hboxLineControls->addWidget(comboLinePoint);


        verticalLayoutLine->addWidget(groupBoxLineControls);

        groupBoxLinePerf = new QGroupBox(tabLine);
        groupBoxLinePerf->setObjectName("groupBoxLinePerf");
        vboxLinePerf = new QVBoxLayout(groupBoxLinePerf);
        vboxLinePerf->setObjectName("vboxLinePerf");
        dda_time_label = new QLabel(groupBoxLinePerf);
        dda_time_label->setObjectName("dda_time_label");

        vboxLinePerf->addWidget(dda_time_label);

        lblDdaCount = new QLabel(groupBoxLinePerf);
        lblDdaCount->setObjectName("lblDdaCount");

        vboxLinePerf->addWidget(lblDdaCount);

        bresenham_time_label = new QLabel(groupBoxLinePerf);
        bresenham_time_label->setObjectName("bresenham_time_label");

        vboxLinePerf->addWidget(bresenham_time_label);

        lblBresenhamCount = new QLabel(groupBoxLinePerf);
        lblBresenhamCount->setObjectName("lblBresenhamCount");

        vboxLinePerf->addWidget(lblBresenhamCount);


        verticalLayoutLine->addWidget(groupBoxLinePerf);

        groupBoxLineActions = new QGroupBox(tabLine);
        groupBoxLineActions->setObjectName("groupBoxLineActions");
        gridLineActions = new QGridLayout(groupBoxLineActions);
        gridLineActions->setObjectName("gridLineActions");
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
        verticalLayoutCircle->setObjectName("verticalLayoutCircle");
        groupBoxCircleAlgo = new QGroupBox(tabCircle);
        groupBoxCircleAlgo->setObjectName("groupBoxCircleAlgo");
        vboxCircleAlgo = new QVBoxLayout(groupBoxCircleAlgo);
        vboxCircleAlgo->setObjectName("vboxCircleAlgo");
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
        vboxCP = new QVBoxLayout(groupBoxCirclePerf);
        vboxCP->setObjectName("vboxCP");
        scrollC = new QScrollArea(groupBoxCirclePerf);
        scrollC->setObjectName("scrollC");
        scrollC->setWidgetResizable(true);
        sw_c = new QWidget();
        sw_c->setObjectName("sw_c");
        vboxCirclePerf = new QVBoxLayout(sw_c);
        vboxCirclePerf->setObjectName("vboxCirclePerf");
        lblPolarTime = new QLabel(sw_c);
        lblPolarTime->setObjectName("lblPolarTime");

        vboxCirclePerf->addWidget(lblPolarTime);

        lblPolarCount = new QLabel(sw_c);
        lblPolarCount->setObjectName("lblPolarCount");

        vboxCirclePerf->addWidget(lblPolarCount);

        lblMidpointTime = new QLabel(sw_c);
        lblMidpointTime->setObjectName("lblMidpointTime");

        vboxCirclePerf->addWidget(lblMidpointTime);

        lblMidpointCount = new QLabel(sw_c);
        lblMidpointCount->setObjectName("lblMidpointCount");

        vboxCirclePerf->addWidget(lblMidpointCount);

        lblCartesianTime = new QLabel(sw_c);
        lblCartesianTime->setObjectName("lblCartesianTime");

        vboxCirclePerf->addWidget(lblCartesianTime);

        lblCartesianCount = new QLabel(sw_c);
        lblCartesianCount->setObjectName("lblCartesianCount");

        vboxCirclePerf->addWidget(lblCartesianCount);

        scrollC->setWidget(sw_c);

        vboxCP->addWidget(scrollC);


        verticalLayoutCircle->addWidget(groupBoxCirclePerf);

        groupBoxCircleActions = new QGroupBox(tabCircle);
        groupBoxCircleActions->setObjectName("groupBoxCircleActions");
        hboxCircleActions = new QHBoxLayout(groupBoxCircleActions);
        hboxCircleActions->setObjectName("hboxCircleActions");
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
        tabEllipse = new QWidget();
        tabEllipse->setObjectName("tabEllipse");
        verticalLayoutEllipse = new QVBoxLayout(tabEllipse);
        verticalLayoutEllipse->setObjectName("verticalLayoutEllipse");
        groupBoxEllipseAlgo = new QGroupBox(tabEllipse);
        groupBoxEllipseAlgo->setObjectName("groupBoxEllipseAlgo");
        vboxEllipseAlgo = new QVBoxLayout(groupBoxEllipseAlgo);
        vboxEllipseAlgo->setObjectName("vboxEllipseAlgo");
        comboEllipseAlgo = new QComboBox(groupBoxEllipseAlgo);
        comboEllipseAlgo->addItem(QString());
        comboEllipseAlgo->addItem(QString());
        comboEllipseAlgo->addItem(QString());
        comboEllipseAlgo->addItem(QString());
        comboEllipseAlgo->setObjectName("comboEllipseAlgo");

        vboxEllipseAlgo->addWidget(comboEllipseAlgo);


        verticalLayoutEllipse->addWidget(groupBoxEllipseAlgo);

        groupBoxEllipseControls = new QGroupBox(tabEllipse);
        groupBoxEllipseControls->setObjectName("groupBoxEllipseControls");
        gridEllipseControls = new QGridLayout(groupBoxEllipseControls);
        gridEllipseControls->setObjectName("gridEllipseControls");
        comboEllipsePoint = new QComboBox(groupBoxEllipseControls);
        comboEllipsePoint->addItem(QString());
        comboEllipsePoint->addItem(QString());
        comboEllipsePoint->addItem(QString());
        comboEllipsePoint->setObjectName("comboEllipsePoint");

        gridEllipseControls->addWidget(comboEllipsePoint, 0, 0, 1, 2);

        labelRx = new QLabel(groupBoxEllipseControls);
        labelRx->setObjectName("labelRx");

        gridEllipseControls->addWidget(labelRx, 1, 0, 1, 1);

        spinBoxRx = new QSpinBox(groupBoxEllipseControls);
        spinBoxRx->setObjectName("spinBoxRx");
        spinBoxRx->setMaximum(1000);

        gridEllipseControls->addWidget(spinBoxRx, 1, 1, 1, 1);

        labelRy = new QLabel(groupBoxEllipseControls);
        labelRy->setObjectName("labelRy");

        gridEllipseControls->addWidget(labelRy, 2, 0, 1, 1);

        spinBoxRy = new QSpinBox(groupBoxEllipseControls);
        spinBoxRy->setObjectName("spinBoxRy");
        spinBoxRy->setMaximum(1000);

        gridEllipseControls->addWidget(spinBoxRy, 2, 1, 1, 1);

        chkQuadColors = new QCheckBox(groupBoxEllipseControls);
        chkQuadColors->setObjectName("chkQuadColors");

        gridEllipseControls->addWidget(chkQuadColors, 3, 0, 1, 2);

        chkRegionSplit = new QCheckBox(groupBoxEllipseControls);
        chkRegionSplit->setObjectName("chkRegionSplit");

        gridEllipseControls->addWidget(chkRegionSplit, 4, 0, 1, 2);

        labelRotation = new QLabel(groupBoxEllipseControls);
        labelRotation->setObjectName("labelRotation");

        gridEllipseControls->addWidget(labelRotation, 4, 0, 1, 1);

        sliderRotation = new QSlider(groupBoxEllipseControls);
        sliderRotation->setObjectName("sliderRotation");
        sliderRotation->setOrientation(Qt::Horizontal);
        sliderRotation->setMinimum(-180);
        sliderRotation->setMaximum(180);
        sliderRotation->setValue(0);

        gridEllipseControls->addWidget(sliderRotation, 4, 1, 1, 1);

        labelThickness = new QLabel(groupBoxEllipseControls);
        labelThickness->setObjectName("labelThickness");

        gridEllipseControls->addWidget(labelThickness, 5, 0, 1, 1);

        sliderThickness = new QSlider(groupBoxEllipseControls);
        sliderThickness->setObjectName("sliderThickness");
        sliderThickness->setOrientation(Qt::Horizontal);
        sliderThickness->setMinimum(1);
        sliderThickness->setMaximum(15);
        sliderThickness->setValue(1);

        gridEllipseControls->addWidget(sliderThickness, 5, 1, 1, 1);


        verticalLayoutEllipse->addWidget(groupBoxEllipseControls);

        groupBoxEllipsePerf = new QGroupBox(tabEllipse);
        groupBoxEllipsePerf->setObjectName("groupBoxEllipsePerf");
        vboxEP = new QVBoxLayout(groupBoxEllipsePerf);
        vboxEP->setObjectName("vboxEP");
        scrollE = new QScrollArea(groupBoxEllipsePerf);
        scrollE->setObjectName("scrollE");
        scrollE->setWidgetResizable(true);
        sw_e = new QWidget();
        sw_e->setObjectName("sw_e");
        vboxEllipsePerf = new QVBoxLayout(sw_e);
        vboxEllipsePerf->setObjectName("vboxEllipsePerf");
        lblEllipsePolarTime = new QLabel(sw_e);
        lblEllipsePolarTime->setObjectName("lblEllipsePolarTime");
        QFont font;
        font.setBold(true);
        lblEllipsePolarTime->setFont(font);

        vboxEllipsePerf->addWidget(lblEllipsePolarTime);

        lblEllipsePolarCount = new QLabel(sw_e);
        lblEllipsePolarCount->setObjectName("lblEllipsePolarCount");

        vboxEllipsePerf->addWidget(lblEllipsePolarCount);

        lblEllipseMidpointTime = new QLabel(sw_e);
        lblEllipseMidpointTime->setObjectName("lblEllipseMidpointTime");
        lblEllipseMidpointTime->setFont(font);

        vboxEllipsePerf->addWidget(lblEllipseMidpointTime);

        lblEllipseMidpointCount = new QLabel(sw_e);
        lblEllipseMidpointCount->setObjectName("lblEllipseMidpointCount");

        vboxEllipsePerf->addWidget(lblEllipseMidpointCount);

        lblEllipseCartesianTime = new QLabel(sw_e);
        lblEllipseCartesianTime->setObjectName("lblEllipseCartesianTime");
        lblEllipseCartesianTime->setFont(font);

        vboxEllipsePerf->addWidget(lblEllipseCartesianTime);

        lblEllipseCartesianCount = new QLabel(sw_e);
        lblEllipseCartesianCount->setObjectName("lblEllipseCartesianCount");

        vboxEllipsePerf->addWidget(lblEllipseCartesianCount);

        lblEllipseCompare = new QLabel(sw_e);
        lblEllipseCompare->setObjectName("lblEllipseCompare");
        lblEllipseCompare->setFont(font);

        vboxEllipsePerf->addWidget(lblEllipseCompare);

        lblEllipseArea = new QLabel(sw_e);
        lblEllipseArea->setObjectName("lblEllipseArea");
        lblEllipseArea->setFont(font);

        vboxEllipsePerf->addWidget(lblEllipseArea);

        lblEllipsePerimeter = new QLabel(sw_e);
        lblEllipsePerimeter->setObjectName("lblEllipsePerimeter");
        lblEllipsePerimeter->setFont(font);

        vboxEllipsePerf->addWidget(lblEllipsePerimeter);

        scrollE->setWidget(sw_e);

        vboxEP->addWidget(scrollE);


        verticalLayoutEllipse->addWidget(groupBoxEllipsePerf);

        groupBoxEllipseActions = new QGroupBox(tabEllipse);
        groupBoxEllipseActions->setObjectName("groupBoxEllipseActions");
        gridEllipseActions = new QGridLayout(groupBoxEllipseActions);
        gridEllipseActions->setObjectName("gridEllipseActions");
        btnDrawEllipse = new QPushButton(groupBoxEllipseActions);
        btnDrawEllipse->setObjectName("btnDrawEllipse");

        gridEllipseActions->addWidget(btnDrawEllipse, 0, 0, 1, 1);

        btnAnimateEllipse = new QPushButton(groupBoxEllipseActions);
        btnAnimateEllipse->setObjectName("btnAnimateEllipse");

        gridEllipseActions->addWidget(btnAnimateEllipse, 0, 1, 1, 1);

        btnClearEllipse = new QPushButton(groupBoxEllipseActions);
        btnClearEllipse->setObjectName("btnClearEllipse");

        gridEllipseActions->addWidget(btnClearEllipse, 1, 0, 1, 2);

        btnCommitEllipse = new QPushButton(groupBoxEllipseActions);
        btnCommitEllipse->setObjectName("btnCommitEllipse");

        gridEllipseActions->addWidget(btnCommitEllipse, 2, 0, 1, 1);

        btnClearCanvas = new QPushButton(groupBoxEllipseActions);
        btnClearCanvas->setObjectName("btnClearCanvas");

        gridEllipseActions->addWidget(btnClearCanvas, 2, 1, 1, 1);


        verticalLayoutEllipse->addWidget(groupBoxEllipseActions);

        tabWidget->addTab(tabEllipse, QString());

        verticalLayoutControls->addWidget(tabWidget);

        groupBoxDebugger = new QGroupBox(centralwidget);
        groupBoxDebugger->setObjectName("groupBoxDebugger");
        vboxDebugger = new QVBoxLayout(groupBoxDebugger);
        vboxDebugger->setObjectName("vboxDebugger");
        textDebugger = new QTextBrowser(groupBoxDebugger);
        textDebugger->setObjectName("textDebugger");
        textDebugger->setMaximumHeight(200);

        vboxDebugger->addWidget(textDebugger);


        verticalLayoutControls->addWidget(groupBoxDebugger);


        horizontalLayout->addLayout(verticalLayoutControls);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1200, 24));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Computer Graphics Lab - High Grade", nullptr));
        MainWindow->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"QMainWindow { background-color: #11111b; color: #cdd6f4; font-size: 14px; font-family: 'Inter', 'Roboto', sans-serif; }\n"
"QWidget { color: #cdd6f4; font-size: 14px; }\n"
"QGroupBox { border: 1px solid #313244; border-radius: 8px; margin-top: 1.5ex; font-weight: bold; background-color: #181825; padding: 10px; }\n"
"QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 0 8px; left: 10px; color: #89b4fa; }\n"
"QPushButton { background-color: #313244; border: 1px solid #45475a; border-radius: 6px; padding: 8px 16px; color: #cdd6f4; font-weight: bold; }\n"
"QPushButton:hover { background-color: #45475a; border: 1px solid #89b4fa; color: #ffffff; }\n"
"QPushButton:pressed { background-color: #585b70; border: 1px solid #b4befe; }\n"
"QComboBox, QSpinBox { background-color: #1e1e2e; border: 1px solid #313244; border-radius: 6px; padding: 8px; color: #cdd6f4; }\n"
"QComboBox:hover, QSpinBox:hover { border: 1px solid #45475a; }\n"
"QTabWidget::pane { border: 1px solid #313244; bord"
                        "er-radius: 8px; background-color: #181825; }\n"
"QTabBar::tab { background-color: #1e1e2e; border: 1px solid #313244; padding: 12px 20px; margin-right: 4px; border-top-left-radius: 6px; border-top-right-radius: 6px; color: #a6adc8; font-weight: bold; }\n"
"QTabBar::tab:selected { background-color: #89b4fa; color: #11111b; }\n"
"QTabBar::tab:hover:!selected { background-color: #313244; color: #cdd6f4; }\n"
"QTextBrowser { background-color: #11111b; border: 1px solid #313244; border-radius: 6px; padding: 10px; color: #a6e3a1; font-family: 'Menlo', 'Fira Code', monospace; font-size: 12px; }\n"
"QScrollArea { border: none; background-color: transparent; }\n"
"QScrollBar:vertical { background: #1e1e2e; width: 10px; margin: 0; border-radius: 5px; }\n"
"QScrollBar::handle:vertical { background: #45475a; min-height: 20px; border-radius: 5px; }\n"
"QScrollBar::handle:vertical:hover { background: #585b70; }\n"
"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }\n"
"QSlider::groove:horizontal { b"
                        "order: 1px solid #313244; height: 6px; background: #1e1e2e; border-radius: 3px; }\n"
"QSlider::handle:horizontal { background: #89b4fa; border: 1px solid #89b4fa; width: 14px; height: 14px; margin: -4px 0; border-radius: 7px; }\n"
"QSlider::handle:horizontal:hover { background: #b4befe; }\n"
"QCheckBox { spacing: 8px; }\n"
"QCheckBox::indicator { width: 18px; height: 18px; border-radius: 4px; border: 1px solid #45475a; background: #1e1e2e; }\n"
"QCheckBox::indicator:checked { background: #89b4fa; border: 1px solid #89b4fa; }\n"
"", nullptr));
        centralwidget->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"/* Awwwards Level Catppuccin Mocha QSS */\n"
"QWidget {\n"
"    background-color: #1e1e2e;\n"
"    color: #cdd6f4;\n"
"    font-family: 'Menlo';\n"
"}\n"
"QGroupBox {\n"
"    border: 1px solid #313244;\n"
"    border-radius: 8px;\n"
"    margin-top: 1.5ex;\n"
"    font-weight: bold;\n"
"    color: #89b4fa;\n"
"}\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}\n"
"QPushButton {\n"
"    background-color: #313244;\n"
"    border: 1px solid #45475a;\n"
"    border-radius: 6px;\n"
"    padding: 6px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #45475a;\n"
"    border: 1px solid #89b4fa;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #585b70;\n"
"}\n"
"QSlider::groove:horizontal {\n"
"    border: 1px solid #45475a;\n"
"    height: 6px;\n"
"    background: #313244;\n"
"    border-radius: 3px;\n"
"}\n"
"QSlider::handle:horizontal {\n"
"    background: #cba6f7;\n"
"    border: 2px solid #1e1e2e;\n"
" "
                        "   width: 14px;\n"
"    margin: -4px 0;\n"
"    border-radius: 7px;\n"
"}\n"
"QSlider::handle:horizontal:hover {\n"
"    background: #f5c2e7;\n"
"}\n"
"QTabWidget::pane {\n"
"    border: 1px solid #313244;\n"
"    border-radius: 6px;\n"
"}\n"
"QTabBar::tab {\n"
"    background: #313244;\n"
"    border: 1px solid #45475a;\n"
"    padding: 8px 16px;\n"
"    margin-right: 2px;\n"
"    border-top-left-radius: 6px;\n"
"    border-top-right-radius: 6px;\n"
"}\n"
"QTabBar::tab:selected {\n"
"    background: #89b4fa;\n"
"    color: #1e1e2e;\n"
"    font-weight: bold;\n"
"}\n"
"", nullptr));
        frame->setText(QString());
        frame->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #0f111a; border: 2px solid #313244; border-radius: 12px;", nullptr));
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
        tabWidget->setTabText(tabWidget->indexOf(tabLine), QCoreApplication::translate("MainWindow", "Line", nullptr));
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
        tabWidget->setTabText(tabWidget->indexOf(tabCircle), QCoreApplication::translate("MainWindow", "Circle", nullptr));
        groupBoxEllipseAlgo->setTitle(QCoreApplication::translate("MainWindow", "Algorithm Selection", nullptr));
        comboEllipseAlgo->setItemText(0, QCoreApplication::translate("MainWindow", "Polar Ellipse Algorithm", nullptr));
        comboEllipseAlgo->setItemText(1, QCoreApplication::translate("MainWindow", "Midpoint (Bresenham) Ellipse", nullptr));
        comboEllipseAlgo->setItemText(2, QCoreApplication::translate("MainWindow", "Cartesian Ellipse", nullptr));
        comboEllipseAlgo->setItemText(3, QCoreApplication::translate("MainWindow", "All (Overlap)", nullptr));

        groupBoxEllipseControls->setTitle(QCoreApplication::translate("MainWindow", "Controls", nullptr));
        comboEllipsePoint->setItemText(0, QCoreApplication::translate("MainWindow", "Select Center (Point 1)", nullptr));
        comboEllipsePoint->setItemText(1, QCoreApplication::translate("MainWindow", "Select Radius X (Major)", nullptr));
        comboEllipsePoint->setItemText(2, QCoreApplication::translate("MainWindow", "Select Radius Y (Minor)", nullptr));

        labelRx->setText(QCoreApplication::translate("MainWindow", "Radius X:", nullptr));
        labelRy->setText(QCoreApplication::translate("MainWindow", "Radius Y:", nullptr));
        chkQuadColors->setText(QCoreApplication::translate("MainWindow", "4-Quadrant Color Tinting", nullptr));
        chkRegionSplit->setText(QCoreApplication::translate("MainWindow", "Highlight Region 1/2 Boundary", nullptr));
        labelRotation->setText(QCoreApplication::translate("MainWindow", "Rotation (\302\260):", nullptr));
        labelThickness->setText(QCoreApplication::translate("MainWindow", "Thickness:", nullptr));
        groupBoxEllipsePerf->setTitle(QCoreApplication::translate("MainWindow", "Performance Benchmark (in ns)", nullptr));
        lblEllipsePolarTime->setText(QCoreApplication::translate("MainWindow", "Polar Time: -", nullptr));
        lblEllipsePolarCount->setText(QCoreApplication::translate("MainWindow", "Polar Pixels: -", nullptr));
        lblEllipseMidpointTime->setText(QCoreApplication::translate("MainWindow", "Midpoint Time: -", nullptr));
        lblEllipseMidpointCount->setText(QCoreApplication::translate("MainWindow", "Midpoint Pixels: -", nullptr));
        lblEllipseCartesianTime->setText(QCoreApplication::translate("MainWindow", "Cartesian Time: -", nullptr));
        lblEllipseCartesianCount->setText(QCoreApplication::translate("MainWindow", "Cartesian Pixels: -", nullptr));
        lblEllipseCompare->setText(QCoreApplication::translate("MainWindow", "Efficiency: -", nullptr));
        lblEllipseArea->setText(QCoreApplication::translate("MainWindow", "Area: -", nullptr));
        lblEllipsePerimeter->setText(QCoreApplication::translate("MainWindow", "Perimeter: -", nullptr));
        groupBoxEllipseActions->setTitle(QCoreApplication::translate("MainWindow", "Actions", nullptr));
        btnDrawEllipse->setText(QCoreApplication::translate("MainWindow", "Draw Instantly", nullptr));
        btnAnimateEllipse->setText(QCoreApplication::translate("MainWindow", "Animate Symmetry", nullptr));
        btnClearEllipse->setText(QCoreApplication::translate("MainWindow", "Clear Ellipse", nullptr));
        btnCommitEllipse->setText(QCoreApplication::translate("MainWindow", "Commit to Canvas", nullptr));
        btnClearCanvas->setText(QCoreApplication::translate("MainWindow", "Clear Canvas", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabEllipse), QCoreApplication::translate("MainWindow", "Ellipse", nullptr));
        groupBoxDebugger->setTitle(QCoreApplication::translate("MainWindow", "Algorithm State Debugger", nullptr));
        textDebugger->setPlaceholderText(QCoreApplication::translate("MainWindow", "Awaiting animation to trace algorithm state...", nullptr));
        statusbar->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #a6adc8;", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
