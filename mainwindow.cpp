#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QColor>
#include <QElapsedTimer>
#include <QtMath>
#include <QResizeEvent>
#include <QMenu>
#include <QAction>
#include <algorithm>

// ---- TRACE TAG EXTRACTION ----
// Extracts the key numeric value from each algorithm's log format

static QString extractTraceTag(const QVector<QString> &logs, int index, int algoType) {
    if (index < 0 || index >= logs.size()) return {};
    QString log = logs[index];

    if (algoType == 0) {
        // DDA: "Step %1: X=%2, Y=%3 (Rounded: %4, %5)" -> "X=3 Y=5"
        int ri = log.indexOf("Rounded: ");
        if (ri >= 0) {
            QString val = log.mid(ri + 9);
            val.chop(1); // remove trailing ")"
            return "X=" + val;
        }
        return log;
    } else if (algoType == 1) {
        // Bresenham: "Step %1: X=%2, Y=%3 | Error P=%4" -> "P=42"
        int pi = log.indexOf("Error P=");
        if (pi >= 0) return "P=" + log.mid(pi + 8);
        return log;
    } else if (algoType == 2) {
        // Circle/Ellipse Polar: "Theta=%1 rad | X=%2, Y=%3" -> "θ=0.524"
        int ti = log.indexOf("Theta=");
        if (ti >= 0) {
            QString val = log.mid(ti + 6);
            int ri = val.indexOf(" rad");
            if (ri >= 0) val.truncate(ri);
            return "\xce\xb8=" + val;  // θ
        }
        return log;
    } else if (algoType == 3) {
        // Circle/Ellipse Midpoint: "P=%3" or "P1=%3" or "P2=%3"
        int pi = log.indexOf("P1=");
        if (pi >= 0) return "P1=" + log.mid(pi + 3);
        pi = log.indexOf("P2=");
        if (pi >= 0) return "P2=" + log.mid(pi + 3);
        pi = log.indexOf("P=");
        if (pi >= 0) return "P=" + log.mid(pi + 2);
        return log;
    } else if (algoType == 4) {
        // Circle/Cartesian: "X=%1 | Evaluated Y=%2" -> "X=3 Y=5"
        int xi = log.indexOf("| Evaluated Y=");
        if (xi >= 0) {
            QString left = log.left(xi);  // "X=3"
            QString right = log.mid(xi + 14); // "5"
            return left + " " + right;
        }
        // Cartesian: "Cartesian: X=%1 | Evaluated Y=%2"
        xi = log.indexOf("Cartesian: X=");
        if (xi >= 0) {
            QString rest = log.mid(xi + 13);
            int pi = rest.indexOf("| Evaluated Y=");
            if (pi >= 0) {
                return "X=" + rest.left(pi) + " Y=" + rest.mid(pi + 14);
            }
        }
        return log;
    }
    return log;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- NEW UI ARCHITECTURE ---
    QWidget* newCentral = ui->centralwidget;
    if (QLayout* oldLayout = newCentral->layout()) {
        while (QLayoutItem* item = oldLayout->takeAt(0)) {
            if (QWidget* w = item->widget()) w->setParent(newCentral);
            delete item;
        }
        delete oldLayout;
    }

    // Consolidated Catppuccin Mocha stylesheet
    this->setStyleSheet(
        "QMainWindow { background-color: #11111b; }"
        "QMenuBar { background-color: #181825; color: #cdd6f4; border-bottom: 1px solid #313244; }"
        "QMenuBar::item:selected { background-color: #45475a; }"
        "QMenu { background-color: #1e1e2e; color: #cdd6f4; border: 1px solid #313244; }"
        "QMenu::item:selected { background-color: #89b4fa; color: #11111b; }"
        "QStatusBar { background-color: #181825; color: #a6adc8; border-top: 1px solid #313244; }"
    );
    newCentral->setStyleSheet(
        "QWidget { background-color: #1e1e2e; color: #cdd6f4; font-family: 'Menlo'; font-size: 13px; }"
        "QGroupBox { border: 1px solid #313244; border-radius: 6px; margin-top: 14px; padding: 8px 6px 6px 6px; font-weight: bold; color: #89b4fa; background-color: #181825; }"
        "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; left: 8px; padding: 0 4px; }"
        "QLabel { color: #cdd6f4; }"
        "QPushButton { background-color: #313244; color: #cdd6f4; border: 1px solid #45475a; border-radius: 5px; padding: 5px 10px; font-weight: bold; min-height: 22px; }"
        "QPushButton:hover { background-color: #45475a; border-color: #89b4fa; color: #ffffff; }"
        "QPushButton:pressed { background-color: #585b70; border-color: #b4befe; }"
        "QComboBox { background-color: #313244; color: #cdd6f4; border: 1px solid #45475a; border-radius: 4px; padding: 3px 6px; min-height: 22px; }"
        "QComboBox::drop-down { border: none; width: 20px; }"
        "QComboBox QAbstractItemView { background-color: #313244; color: #cdd6f4; selection-background-color: #89b4fa; selection-color: #11111b; border: 1px solid #45475a; }"
        "QSpinBox { background-color: #313244; color: #cdd6f4; border: 1px solid #45475a; border-radius: 4px; padding: 3px; min-height: 22px; min-width: 50px; }"
        "QSlider::groove:horizontal { border: 1px solid #45475a; height: 4px; background: #313244; border-radius: 2px; }"
        "QSlider::handle:horizontal { background: #89b4fa; width: 12px; height: 12px; margin: -4px 0; border-radius: 6px; }"
        "QSlider::handle:horizontal:hover { background: #b4befe; }"
        "QCheckBox { spacing: 6px; color: #cdd6f4; }"
        "QCheckBox::indicator { width: 16px; height: 16px; border-radius: 3px; border: 1px solid #45475a; background: #313244; }"
        "QCheckBox::indicator:checked { background: #89b4fa; border-color: #89b4fa; }"
        "QTextBrowser { background-color: #11111b; border: 1px solid #313244; border-radius: 6px; padding: 6px; color: #a6e3a1; font-family: 'Menlo', monospace; font-size: 11px; }"
        "QScrollArea { border: none; background-color: transparent; }"
        "QScrollBar:vertical { background: #1e1e2e; width: 8px; border-radius: 4px; }"
        "QScrollBar::handle:vertical { background: #45475a; min-height: 20px; border-radius: 4px; max-height: 40px; }"
        "QScrollBar::handle:vertical:hover { background: #585b70; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }"
    );

    QHBoxLayout* mainLayout = new QHBoxLayout(newCentral);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ---- SIDEBAR with SVG icons ----
    QFrame* sidebar = new QFrame(newCentral);
    sidebar->setFixedWidth(130);
    sidebar->setStyleSheet("QFrame { background-color: #181825; border-right: 1px solid #313244; }");
    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(8, 12, 8, 12);
    sidebarLayout->setSpacing(6);

    const char* svgNormal[] = {
        "<svg xmlns='http://www.w3.org/2000/svg' width='32' height='32'><line x1='5' y1='27' x2='27' y2='5' stroke='#a6adc8' stroke-width='2.5' stroke-linecap='round'/></svg>",
        "<svg xmlns='http://www.w3.org/2000/svg' width='32' height='32'><circle cx='16' cy='16' r='11' fill='none' stroke='#a6adc8' stroke-width='2.5'/></svg>",
        "<svg xmlns='http://www.w3.org/2000/svg' width='32' height='32'><ellipse cx='16' cy='16' rx='13' ry='8' fill='none' stroke='#a6adc8' stroke-width='2.5'/></svg>",
        "<svg xmlns='http://www.w3.org/2000/svg' width='32' height='32'><polygon points='16,3 29,27 3,27' fill='none' stroke='#a6adc8' stroke-width='2.5' stroke-linejoin='round'/></svg>",
        "<svg xmlns='http://www.w3.org/2000/svg' width='32' height='32'><path d='M6 26 L16 6 L26 26 Z' fill='none' stroke='#a6adc8' stroke-width='2.5' stroke-linejoin='round'/><path d='M2 16 L6 12 M2 16 L6 20 M30 16 L26 12 M30 16 L26 20' stroke='#a6adc8' stroke-width='2.5' stroke-linecap='round'/></svg>",
        "<svg xmlns='http://www.w3.org/2000/svg' width='32' height='32'><path d='M5 27 C10 5, 22 27, 27 5' fill='none' stroke='#a6adc8' stroke-width='2.5'/><circle cx='5' cy='27' r='2' fill='#a6adc8'/><circle cx='10' cy='5' r='2' fill='#a6adc8'/><circle cx='22' cy='27' r='2' fill='#a6adc8'/><circle cx='27' cy='5' r='2' fill='#a6adc8'/></svg>"
    };
    const char* svgChecked[] = {
        "<svg xmlns='http://www.w3.org/2000/svg' width='32' height='32'><line x1='5' y1='27' x2='27' y2='5' stroke='#11111b' stroke-width='3' stroke-linecap='round'/></svg>",
        "<svg xmlns='http://www.w3.org/2000/svg' width='32' height='32'><circle cx='16' cy='16' r='11' fill='none' stroke='#11111b' stroke-width='3'/></svg>",
        "<svg xmlns='http://www.w3.org/2000/svg' width='32' height='32'><ellipse cx='16' cy='16' rx='13' ry='8' fill='none' stroke='#11111b' stroke-width='3'/></svg>",
        "<svg xmlns='http://www.w3.org/2000/svg' width='32' height='32'><polygon points='16,3 29,27 3,27' fill='none' stroke='#11111b' stroke-width='3' stroke-linejoin='round'/></svg>",
        "<svg xmlns='http://www.w3.org/2000/svg' width='32' height='32'><path d='M6 26 L16 6 L26 26 Z' fill='none' stroke='#11111b' stroke-width='3' stroke-linejoin='round'/><path d='M2 16 L6 12 M2 16 L6 20 M30 16 L26 12 M30 16 L26 20' stroke='#11111b' stroke-width='3' stroke-linecap='round'/></svg>",
        "<svg xmlns='http://www.w3.org/2000/svg' width='32' height='32'><path d='M5 27 C10 5, 22 27, 27 5' fill='none' stroke='#11111b' stroke-width='3'/><circle cx='5' cy='27' r='2' fill='#11111b'/><circle cx='10' cy='5' r='2' fill='#11111b'/><circle cx='22' cy='27' r='2' fill='#11111b'/><circle cx='27' cy='5' r='2' fill='#11111b'/></svg>"
    };
    QStringList toolNames = {"Line", "Circle", "Ellipse", "Polygon", "Transform", "Curve"};
    QList<QPixmap> normalPixList, checkedPixList;
    for (int i = 0; i < 6; ++i) {
        QSvgRenderer r{QByteArray(svgNormal[i])};
        QPixmap nPix(32, 32); nPix.fill(Qt::transparent);
        QPainter np(&nPix); r.render(&np); np.end();
        normalPixList.append(nPix);

        QSvgRenderer rc{QByteArray(svgChecked[i])};
        QPixmap cPix(32, 32); cPix.fill(Qt::transparent);
        QPainter cp(&cPix); rc.render(&cp); cp.end();
        checkedPixList.append(cPix);

        QToolButton* btn = new QToolButton(sidebar);
        btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        btn->setAutoRaise(true);
        btn->setCheckable(true);
        btn->setFixedSize(110, 60);
        btn->setText(toolNames[i]);
        btn->setIconSize(QSize(32, 32));
        btn->setIcon(QIcon(nPix));
        btn->setStyleSheet(
            "QToolButton { background-color: transparent; border: none; border-radius: 8px; color: #a6adc8; padding: 4px; }"
            "QToolButton:hover { background-color: #313244; color: #cdd6f4; }"
            "QToolButton:checked { background-color: #89b4fa; color: #11111b; }"
        );
        sidebarButtons.append(btn);
        sidebarLayout->addWidget(btn);
        connect(btn, &QToolButton::clicked, this, [this, i]() { selectTool(i); });
    }
    sidebarLayout->addStretch();

    // ---- RIGHT PANEL ----
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setContentsMargins(10, 10, 10, 10);
    rightLayout->setSpacing(6);

    // ---- TOP TOOLBAR ----
    navbar = new QFrame(newCentral);
    navbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    navbar->setFixedHeight(40);
    navbar->setStyleSheet("QFrame { background-color: #181825; border-bottom: 1px solid #313244; }");
    QHBoxLayout* navbarLayout = new QHBoxLayout(navbar);
    navbarLayout->setContentsMargins(8, 4, 8, 4);
    navbarLayout->setSpacing(6);

    btnToggleNavbar = new QPushButton("<", navbar);
    btnToggleNavbar->setFixedSize(30, 30);
    btnToggleNavbar->setToolTip("Collapse / Expand Settings");
    btnToggleNavbar->setStyleSheet(
        "QPushButton { background-color: transparent; color: #a6adc8; border: none; font-size: 16px; padding: 0; }"
        "QPushButton:hover { color: #cdd6f4; background-color: #313244; border-radius: 4px; }"
    );
    connect(btnToggleNavbar, &QPushButton::clicked, this, &MainWindow::handleToggleNavbar);
    navbarLayout->addWidget(btnToggleNavbar);

    toolNameLabel = new QLabel("Line");
    toolNameLabel->setStyleSheet("QLabel { color: #89b4fa; font-weight: bold; font-size: 14px; border: none; background-color: transparent; }");
    navbarLayout->addWidget(toolNameLabel);

    navbarLayout->addStretch();

    QPushButton* btnClearCanvas = new QPushButton("Clear All", navbar);
    btnClearCanvas->setFixedHeight(30);
    btnClearCanvas->setStyleSheet(
        "QPushButton { background-color: #f38ba8; color: #11111b; font-weight: bold; border: none; border-radius: 4px; padding: 4px 12px; }"
        "QPushButton:hover { background-color: #eba0ac; }"
    );
    connect(btnClearCanvas, &QPushButton::clicked, this, &MainWindow::handleClearCanvasClicked);
    navbarLayout->addWidget(btnClearCanvas);

    QLabel* lblGrid = new QLabel("Grid:");
    lblGrid->setStyleSheet("QLabel { color: #a6adc8; border: none; background-color: transparent; }");
    navbarLayout->addWidget(lblGrid);
    ui->spinBox->setFixedWidth(50);
    ui->spinBox->setFixedHeight(24);
    navbarLayout->addWidget(ui->spinBox);

    QLabel* lblSpd = new QLabel("Speed:");
    lblSpd->setStyleSheet("QLabel { color: #a6adc8; border: none; background-color: transparent; }");
    navbarLayout->addWidget(lblSpd);
    ui->sliderSpeed->setFixedWidth(80);
    ui->sliderSpeed->setFixedHeight(20);
    navbarLayout->addWidget(ui->sliderSpeed);
    ui->lblSpeed->setStyleSheet("QLabel { color: #a6adc8; border: none; background-color: transparent; min-width: 38px; }");
    ui->lblSpeed->setFixedHeight(24);
    navbarLayout->addWidget(ui->lblSpeed);

    rightLayout->addWidget(navbar);

    // ---- SETTINGS BAR (collapsible row below toolbar) ----
    settingsStack = new QStackedWidget();
    settingsStack->setStyleSheet("QStackedWidget { background-color: #1e1e2e; border-bottom: 1px solid #313244; }");

    // ---- SETTINGS STACK PAGES (flat, no scroll wrapper) ----
    QWidget* pageLine = new QWidget();
    pageLine->setStyleSheet("background-color: #1e1e2e;");
    QHBoxLayout* lLine = new QHBoxLayout(pageLine);
    lLine->setContentsMargins(6, 2, 6, 2);
    lLine->setSpacing(8);
    lLine->addWidget(ui->groupBoxLineAlgo);
    lLine->addWidget(ui->groupBoxLineControls);
    lLine->addWidget(ui->groupBoxLinePerf);
    lLine->addWidget(ui->groupBoxLineActions);
    lLine->addStretch();
    settingsStack->addWidget(pageLine);

    QWidget* pageCircle = new QWidget();
    pageCircle->setStyleSheet("background-color: #1e1e2e;");
    QHBoxLayout* lCircle = new QHBoxLayout(pageCircle);
    lCircle->setContentsMargins(6, 2, 6, 2);
    lCircle->setSpacing(8);
    lCircle->addWidget(ui->groupBoxCircleAlgo);
    lCircle->addWidget(ui->groupBoxCircleControls);
    lCircle->addWidget(ui->groupBoxCirclePerf);
    lCircle->addWidget(ui->groupBoxCircleActions);
    lCircle->addStretch();
    settingsStack->addWidget(pageCircle);

    if (ui->groupBoxEllipseControls->layout()) {
        delete ui->groupBoxEllipseControls->layout();
    }
    QHBoxLayout* lEllipseOverride = new QHBoxLayout(ui->groupBoxEllipseControls);
    lEllipseOverride->setContentsMargins(6, 14, 6, 4);
    lEllipseOverride->setSpacing(6);
    lEllipseOverride->addWidget(ui->comboEllipsePoint);
    lEllipseOverride->addWidget(ui->labelRx);
    lEllipseOverride->addWidget(ui->spinBoxRx);
    lEllipseOverride->addWidget(ui->labelRy);
    lEllipseOverride->addWidget(ui->spinBoxRy);

    QWidget* pageEllipse = new QWidget();
    pageEllipse->setStyleSheet("background-color: #1e1e2e;");
    QHBoxLayout* lEllipse = new QHBoxLayout(pageEllipse);
    lEllipse->setContentsMargins(6, 2, 6, 2);
    lEllipse->setSpacing(8);
    lEllipse->addWidget(ui->groupBoxEllipseAlgo);
    lEllipse->addWidget(ui->groupBoxEllipseControls);
    lEllipse->addWidget(ui->groupBoxEllipsePerf);
    lEllipse->addWidget(ui->groupBoxEllipseActions);
    lEllipse->addStretch();
    settingsStack->addWidget(pageEllipse);

    QWidget* pagePolygon = new QWidget();
    pagePolygon->setStyleSheet("background-color: #1e1e2e;");
    QHBoxLayout* lPolygon = new QHBoxLayout(pagePolygon);
    lPolygon->setContentsMargins(6, 2, 6, 2);
    lPolygon->setSpacing(6);

    comboPolygonMode = new QComboBox(pagePolygon);
    comboPolygonMode->setFixedWidth(140);
    comboPolygonMode->addItems({"Draw Polygon", "Flood Fill", "Boundary Fill", "Scanline Fill"});
    connect(comboPolygonMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        if (index == 0) currentTool = TOOL_POLYGON;
        else if (index == 1) currentTool = TOOL_FLOOD_FILL;
        else if (index == 2) currentTool = TOOL_BOUNDARY_FILL;
        else if (index == 3) {
            currentTool = TOOL_SCANLINE_FILL;
            if (lastClosedPolygonVertices.size() >= 3) {
                scanlineFillPolygon(lastClosedPolygonVertices, currentFillColor, &committedPolygonPixels);
                drawgrid();
            }
        }
    });

    QPushButton* btnPolygonColor = new QPushButton("Edge Color", pagePolygon);
    QPushButton* btnPolygonClear = new QPushButton("Clear", pagePolygon);
    QPushButton* btnPolygonClose = new QPushButton("Close", pagePolygon);
    QPushButton* btnFillColor = new QPushButton("Fill Color", pagePolygon);
    QPushButton* btnBoundaryColor = new QPushButton("Boundary", pagePolygon);
    QPushButton* btnPickColor = new QPushButton("Pick Color", pagePolygon);

    QComboBox* comboConnectivity = new QComboBox(pagePolygon);
    comboConnectivity->setFixedWidth(100);
    comboConnectivity->addItems({"4-Way", "8-Way"});
    connect(comboConnectivity, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        fillConnectivity = (index == 0) ? 4 : 8;
    });

    lPolygon->addWidget(comboPolygonMode);
    lPolygon->addWidget(btnPolygonColor);
    lPolygon->addWidget(btnPolygonClose);
    lPolygon->addWidget(btnFillColor);
    lPolygon->addWidget(btnBoundaryColor);
    lPolygon->addWidget(btnPickColor);
    lPolygon->addWidget(comboConnectivity);
    lPolygon->addWidget(btnPolygonClear);
    lPolygon->addStretch();
    settingsStack->addWidget(pagePolygon);

    // ---- TRANSFORM PAGE ----
    QWidget* pageTransform = new QWidget();
    pageTransform->setStyleSheet("background-color: #1e1e2e;");
    QHBoxLayout* lTransform = new QHBoxLayout(pageTransform);
    lTransform->setContentsMargins(6, 14, 6, 4);

    QScrollArea* scrollTransform = new QScrollArea();
    scrollTransform->setWidgetResizable(true);
    scrollTransform->setFrameShape(QFrame::NoFrame);
    scrollTransform->setWidget(pageTransform);
    scrollTransform->setFixedHeight(130);
    scrollTransform->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollTransform->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollTransform->viewport()->setAutoFillBackground(false);
    scrollTransform->setStyleSheet("QScrollArea { background: transparent; } QScrollArea > QWidget > QWidget { background: #1e1e2e; } QScrollBar:horizontal { height: 8px; background: #1e1e2e; } QScrollBar::handle:horizontal { background: #45475a; border-radius: 4px; } QScrollBar::add-line, QScrollBar::sub-line { width: 0; }");

    QString groupStyle = "QGroupBox { color: #a6adc8; font-weight: bold; font-size: 10px; border: 1px solid #45475a; border-radius: 4px; margin-top: 14px; } QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 0 4px; left: 8px; }";
    QString spinStyle = "QSpinBox, QDoubleSpinBox { background-color: #181825; color: #cdd6f4; border: 1px solid #313244; border-radius: 4px; padding: 2px; }";
    QString btnStyle = "QPushButton { background-color: #313244; color: #cdd6f4; border: none; border-radius: 4px; padding: 4px 8px; } QPushButton:hover { background-color: #45475a; }";

    // Translate
    QGroupBox* gbTranslate = new QGroupBox("Translate");
    gbTranslate->setStyleSheet(groupStyle + spinStyle + btnStyle);
    QHBoxLayout* lTrans = new QHBoxLayout(gbTranslate);
    lTrans->setContentsMargins(4, 8, 4, 4);
    QSpinBox* spinTx = new QSpinBox(); spinTx->setObjectName("spinTx"); spinTx->setRange(-1000, 1000); spinTx->setPrefix("x: ");
    QSpinBox* spinTy = new QSpinBox(); spinTy->setObjectName("spinTy"); spinTy->setRange(-1000, 1000); spinTy->setPrefix("y: ");
    QPushButton* btnTranslate = new QPushButton("Apply");
    lTrans->addWidget(spinTx); lTrans->addWidget(spinTy); lTrans->addWidget(btnTranslate);
    gbTranslate->setMinimumWidth(220);
    lTransform->addWidget(gbTranslate);

    // Rotate
    QGroupBox* gbRotate = new QGroupBox("Rotate");
    gbRotate->setStyleSheet(groupStyle + spinStyle + btnStyle);
    QHBoxLayout* lRot = new QHBoxLayout(gbRotate);
    lRot->setContentsMargins(4, 8, 4, 4);
    QDoubleSpinBox* spinAngle = new QDoubleSpinBox(); spinAngle->setObjectName("spinAngle"); spinAngle->setRange(-360, 360); spinAngle->setSuffix("°");
    QPushButton* btnRotate = new QPushButton("Apply");
    lRot->addWidget(spinAngle); lRot->addWidget(btnRotate);
    gbRotate->setMinimumWidth(150);
    lTransform->addWidget(gbRotate);

    // Scale
    QGroupBox* gbScale = new QGroupBox("Scale");
    gbScale->setStyleSheet(groupStyle + spinStyle + btnStyle);
    QHBoxLayout* lScale = new QHBoxLayout(gbScale);
    lScale->setContentsMargins(4, 8, 4, 4);
    QDoubleSpinBox* spinSx = new QDoubleSpinBox(); spinSx->setObjectName("spinSx"); spinSx->setValue(1.0); spinSx->setSingleStep(0.1); spinSx->setPrefix("x: ");
    QDoubleSpinBox* spinSy = new QDoubleSpinBox(); spinSy->setObjectName("spinSy"); spinSy->setValue(1.0); spinSy->setSingleStep(0.1); spinSy->setPrefix("y: ");
    QPushButton* btnScale = new QPushButton("Apply");
    lScale->addWidget(spinSx); lScale->addWidget(spinSy); lScale->addWidget(btnScale);
    gbScale->setMinimumWidth(220);
    lTransform->addWidget(gbScale);

    // Shear
    QGroupBox* gbShear = new QGroupBox("Shear");
    gbShear->setStyleSheet(groupStyle + spinStyle + btnStyle);
    QHBoxLayout* lShear = new QHBoxLayout(gbShear);
    lShear->setContentsMargins(4, 8, 4, 4);
    QDoubleSpinBox* spinShx = new QDoubleSpinBox(); spinShx->setObjectName("spinShx"); spinShx->setSingleStep(0.1); spinShx->setPrefix("x: "); spinShx->setRange(-10.0, 10.0);
    QDoubleSpinBox* spinShy = new QDoubleSpinBox(); spinShy->setObjectName("spinShy"); spinShy->setSingleStep(0.1); spinShy->setPrefix("y: "); spinShy->setRange(-10.0, 10.0);
    QPushButton* btnShear = new QPushButton("Apply");
    lShear->addWidget(spinShx); lShear->addWidget(spinShy); lShear->addWidget(btnShear);
    gbShear->setMinimumWidth(220);
    lTransform->addWidget(gbShear);

    // Reflect
    QGroupBox* gbReflect = new QGroupBox("Reflect");
    gbReflect->setStyleSheet(groupStyle + btnStyle);
    QHBoxLayout* lRef = new QHBoxLayout(gbReflect);
    lRef->setContentsMargins(4, 8, 4, 4);
    QPushButton* btnRefX = new QPushButton("X-Axis");
    QPushButton* btnRefY = new QPushButton("Y-Axis");
    QPushButton* btnRefO = new QPushButton("Origin");
    lRef->addWidget(btnRefX); lRef->addWidget(btnRefY); lRef->addWidget(btnRefO);
    gbReflect->setMinimumWidth(180);
    lTransform->addWidget(gbReflect);

    // Arb Line Reflect
    QGroupBox* gbArbLine = new QGroupBox("Reflect Arb Line");
    gbArbLine->setStyleSheet(groupStyle + spinStyle + btnStyle);
    QHBoxLayout* lArbLine = new QHBoxLayout(gbArbLine);
    lArbLine->setContentsMargins(4, 8, 4, 4);
    QSpinBox* alX1 = new QSpinBox(); alX1->setObjectName("alX1"); alX1->setRange(-1000, 1000); alX1->setPrefix("x1:");
    QSpinBox* alY1 = new QSpinBox(); alY1->setObjectName("alY1"); alY1->setRange(-1000, 1000); alY1->setPrefix("y1:");
    QSpinBox* alX2 = new QSpinBox(); alX2->setObjectName("alX2"); alX2->setRange(-1000, 1000); alX2->setPrefix("x2:"); alX2->setValue(10);
    QSpinBox* alY2 = new QSpinBox(); alY2->setObjectName("alY2"); alY2->setRange(-1000, 1000); alY2->setPrefix("y2:"); alY2->setValue(10);
    QPushButton* btnArbLine = new QPushButton("Apply");
    lArbLine->addWidget(alX1); lArbLine->addWidget(alY1); lArbLine->addWidget(alX2); lArbLine->addWidget(alY2); lArbLine->addWidget(btnArbLine);
    gbArbLine->setMinimumWidth(380);
    lTransform->addWidget(gbArbLine);

    // Arb Point Rotate
    QGroupBox* gbArbPt = new QGroupBox("Rotate Arb Point");
    gbArbPt->setStyleSheet(groupStyle + spinStyle + btnStyle);
    QHBoxLayout* lArbPt = new QHBoxLayout(gbArbPt);
    lArbPt->setContentsMargins(4, 8, 4, 4);
    QSpinBox* apX = new QSpinBox(); apX->setObjectName("apX"); apX->setRange(-1000, 1000); apX->setPrefix("x:");
    QSpinBox* apY = new QSpinBox(); apY->setObjectName("apY"); apY->setRange(-1000, 1000); apY->setPrefix("y:");
    QDoubleSpinBox* apA = new QDoubleSpinBox(); apA->setObjectName("apA"); apA->setRange(-360, 360); apA->setSuffix("°");
    QPushButton* btnArbPt = new QPushButton("Apply");
    lArbPt->addWidget(apX); lArbPt->addWidget(apY); lArbPt->addWidget(apA); lArbPt->addWidget(btnArbPt);
    gbArbPt->setMinimumWidth(260);
    lTransform->addWidget(gbArbPt);

    QCheckBox* chkKeepOriginal = new QCheckBox("Keep Original (Ghost)");
    chkKeepOriginal->setObjectName("chkKeepOriginal");
    chkKeepOriginal->setStyleSheet("QCheckBox { color: #a6adc8; } QCheckBox::indicator { width: 14px; height: 14px; }");
    chkKeepOriginal->setToolTip("When enabled, transforms keep the original shape as a ghost");
    lTransform->addWidget(chkKeepOriginal);
    connect(chkKeepOriginal, &QCheckBox::toggled, this, [this](bool checked) {
        keepOriginalOnTransform = checked;
    });

    lTransform->addStretch();
    lTransform->setSpacing(8);
    settingsStack->addWidget(scrollTransform);

    // ---- BEZIER CURVE PAGE ----
    QWidget* pageCurve = new QWidget();
    pageCurve->setStyleSheet("background-color: #1e1e2e;");
    QHBoxLayout* lCurve = new QHBoxLayout(pageCurve);
    lCurve->setContentsMargins(6, 2, 6, 2);
    lCurve->setSpacing(8);

    QString curveGroupStyle = "QGroupBox { color: #a6adc8; font-weight: bold; font-size: 10px; border: 1px solid #45475a; border-radius: 4px; margin-top: 10px; } QGroupBox::title { subcontrol-origin: margin; left: 8px; top: -6px; }";
    QString curveBtnStyle = "QPushButton { background-color: #313244; color: #cdd6f4; border: none; border-radius: 4px; padding: 4px 8px; } QPushButton:hover { background-color: #45475a; }";

    // Curve Controls
    QGroupBox* gbCurveCtrl = new QGroupBox("Curve Controls");
    gbCurveCtrl->setStyleSheet(curveGroupStyle);
    QVBoxLayout* lCurveCtrl = new QVBoxLayout(gbCurveCtrl);
    lCurveCtrl->setContentsMargins(4, 8, 4, 4);
    QLabel* lblCurveInfo = new QLabel("Click 4 control points on canvas");
    lblCurveInfo->setStyleSheet("color: #a6adc8; font-size: 10px;");
    lCurveCtrl->addWidget(lblCurveInfo);
    QPushButton* btnCurveDraw = new QPushButton("Draw Instantly");
    QPushButton* btnCurveAnimate = new QPushButton("Animate");
    QPushButton* btnCurveClear = new QPushButton("Clear");
    btnCurveDraw->setStyleSheet(curveBtnStyle);
    btnCurveAnimate->setStyleSheet(curveBtnStyle);
    btnCurveClear->setStyleSheet(curveBtnStyle);
    lCurveCtrl->addWidget(btnCurveDraw);
    lCurveCtrl->addWidget(btnCurveAnimate);
    lCurveCtrl->addWidget(btnCurveClear);
    gbCurveCtrl->setMinimumWidth(160);
    lCurve->addWidget(gbCurveCtrl);

    // Curve Perf
    QGroupBox* gbCurvePerf = new QGroupBox("Performance");
    gbCurvePerf->setStyleSheet(curveGroupStyle);
    QVBoxLayout* lCurvePerf = new QVBoxLayout(gbCurvePerf);
    lCurvePerf->setContentsMargins(4, 8, 4, 4);
    QLabel* lblCurveTime = new QLabel("Bezier Time: -");
    QLabel* lblCurveCount = new QLabel("Curve Points: -");
    QLabel* lblCurveSteps = new QLabel("Steps: -");
    lblCurveTime->setStyleSheet("color: #a6adc8; font-size: 10px;");
    lblCurveCount->setStyleSheet("color: #a6adc8; font-size: 10px;");
    lblCurveSteps->setStyleSheet("color: #a6adc8; font-size: 10px;");
    lCurvePerf->addWidget(lblCurveTime);
    lCurvePerf->addWidget(lblCurveCount);
    lCurvePerf->addWidget(lblCurveSteps);
    gbCurvePerf->setMinimumWidth(160);
    lCurve->addWidget(gbCurvePerf);

    lCurve->addStretch();
    settingsStack->addWidget(pageCurve);

    // Connections
    connect(btnTranslate, &QPushButton::clicked, this, &MainWindow::handleTransformTranslate);
    connect(btnRotate, &QPushButton::clicked, this, &MainWindow::handleTransformRotate);
    connect(btnScale, &QPushButton::clicked, this, &MainWindow::handleTransformScale);
    connect(btnShear, &QPushButton::clicked, this, &MainWindow::handleTransformShear);
    connect(btnRefX, &QPushButton::clicked, this, &MainWindow::handleTransformReflectX);
    connect(btnRefY, &QPushButton::clicked, this, &MainWindow::handleTransformReflectY);
    connect(btnRefO, &QPushButton::clicked, this, &MainWindow::handleTransformReflectOrigin);
    connect(btnArbLine, &QPushButton::clicked, this, &MainWindow::handleTransformArbitraryLine);
    connect(btnArbPt, &QPushButton::clicked, this, &MainWindow::handleTransformArbitraryPoint);

    // Bezier connections
    connect(btnCurveDraw, &QPushButton::clicked, this, &MainWindow::handleBezierDrawInstantly);
    connect(btnCurveAnimate, &QPushButton::clicked, this, &MainWindow::handleBezierAnimate);
    connect(btnCurveClear, &QPushButton::clicked, this, &MainWindow::handleBezierClear);

    rightLayout->addWidget(settingsStack);

    // ---- DRAWING FRAME ----
    ui->frame->setStyleSheet("background-color: #0f111a; border: 2px solid #313244; border-radius: 8px;");
    rightLayout->addWidget(ui->frame, 1);

    // ---- DEBUGGER ----
    ui->groupBoxDebugger->setTitle("Algorithm Debugger");
    ui->groupBoxDebugger->setFixedHeight(120);
    rightLayout->addWidget(ui->groupBoxDebugger);

    // ---- PLAYBACK BAR ----
    QFrame* playbackBar = new QFrame();
    playbackBar->setFixedHeight(32);
    playbackBar->setStyleSheet("QFrame { background-color: #181825; border-bottom: 1px solid #313244; }");
    QHBoxLayout* lPlayback = new QHBoxLayout(playbackBar);
    lPlayback->setContentsMargins(8, 2, 8, 2);
    lPlayback->setSpacing(6);

    QString pbBtnStyle = "QPushButton { background-color: #313244; color: #cdd6f4; border: none; border-radius: 4px; padding: 4px 10px; font-weight: bold; } QPushButton:hover { background-color: #45475a; } QPushButton:disabled { color: #585b70; }";
    QString pbSliderStyle = "QSlider::groove:horizontal { height: 6px; background: #313244; border-radius: 3px; } QSlider::handle:horizontal { background: #89b4fa; width: 14px; height: 14px; margin: -4px 0; border-radius: 7px; } QSlider::sub-page:horizontal { background: #45475a; border-radius: 3px; }";

    QPushButton* btnStepBack = new QPushButton("\u2039");  // ‹
    btnStepBack->setObjectName("btnStepBack");
    btnStepBack->setFixedSize(28, 26);
    btnStepBack->setStyleSheet(pbBtnStyle);
    btnStepBack->setToolTip("Step Back");

    QPushButton* btnPlayPause = new QPushButton("\u25B6");  // ▶
    btnPlayPause->setObjectName("btnPlayPause");
    btnPlayPause->setFixedSize(28, 26);
    btnPlayPause->setStyleSheet(pbBtnStyle);
    btnPlayPause->setToolTip("Play / Pause");

    QPushButton* btnStepForward = new QPushButton("\u203A");  // ›
    btnStepForward->setObjectName("btnStepForward");
    btnStepForward->setFixedSize(28, 26);
    btnStepForward->setStyleSheet(pbBtnStyle);
    btnStepForward->setToolTip("Step Forward");

    QSlider* scrubSlider = new QSlider(Qt::Horizontal);
    scrubSlider->setObjectName("scrubSlider");
    scrubSlider->setRange(0, 0);
    scrubSlider->setStyleSheet(pbSliderStyle);
    scrubSlider->setMinimumWidth(120);

    QLabel* lblStepInfo = new QLabel("0 / 0");
    lblStepInfo->setObjectName("lblStepInfo");
    lblStepInfo->setStyleSheet("QLabel { color: #a6adc8; border: none; background-color: transparent; font-size: 11px; min-width: 50px; }");

    QCheckBox* chkShowTrace = new QCheckBox("Show Trace");
    chkShowTrace->setObjectName("chkShowTrace");
    chkShowTrace->setStyleSheet("QCheckBox { color: #a6adc8; border: none; background-color: transparent; font-size: 11px; } QCheckBox::indicator { width: 14px; height: 14px; } QCheckBox::indicator:checked { background-color: #89b4fa; border-radius: 3px; } QCheckBox::indicator:unchecked { background-color: #313244; border-radius: 3px; }");

    lPlayback->addWidget(btnStepBack);
    lPlayback->addWidget(btnPlayPause);
    lPlayback->addWidget(btnStepForward);
    lPlayback->addSpacing(8);
    lPlayback->addWidget(scrubSlider, 1);
    lPlayback->addWidget(lblStepInfo);
    lPlayback->addSpacing(8);
    lPlayback->addWidget(chkShowTrace);

    rightLayout->addWidget(playbackBar);

    // Playback connections
    connect(btnStepBack, &QPushButton::clicked, this, &MainWindow::onStepBackClicked);
    connect(btnStepForward, &QPushButton::clicked, this, &MainWindow::onStepForwardClicked);
    connect(btnPlayPause, &QPushButton::clicked, this, [this]() {
        int step = getActiveAnimationStep();
        int total = getActiveAnimationPointCount();
        if (total == 0) {
            statusBar()->showMessage("No animation to play. Use Animate first.");
            return;
        }
        if (step >= total) {
            setActiveAnimationStep(0);
            syncPlaybackState();
            drawgrid();
        }
        // Start/stop the appropriate timer
        if (currentTool == TOOL_LINE) {
            if (lineAnimationTimer->isActive()) lineAnimationTimer->stop();
            else {
                calculatealgorithms();
                int interval = qRound(20 / animationSpeedMultiplier);
                lineAnimationTimer->start(interval < 1 ? 1 : interval);
            }
        } else if (currentTool == TOOL_CIRCLE) {
            if (animationTimer->isActive()) animationTimer->stop();
            else {
                calculateCircleAlgorithms();
                int interval = qRound(50 / animationSpeedMultiplier);
                animationTimer->start(interval < 1 ? 1 : interval);
            }
        } else if (currentTool == TOOL_ELLIPSE) {
            if (ellipseAnimationTimer->isActive()) ellipseAnimationTimer->stop();
            else {
                calculateEllipseAlgorithms();
                int interval = qRound(30 / animationSpeedMultiplier);
                ellipseAnimationTimer->start(interval < 1 ? 1 : interval);
            }
        } else if (currentTool == TOOL_CURVE) {
            if (bezierAnimTimer->isActive()) bezierAnimTimer->stop();
            else {
                handleBezierAnimate();
            }
        }
    });
    connect(scrubSlider, &QSlider::valueChanged, this, &MainWindow::onScrubSliderChanged);
    connect(chkShowTrace, &QCheckBox::toggled, this, [this](bool checked) {
        showTrace = checked;
        drawgrid();
    });
    mainLayout->addWidget(sidebar);
    mainLayout->addLayout(rightLayout, 1);

    // ---- DEFAULT STATE ----
    currentTool = TOOL_LINE;
    sidebarButtons[0]->setChecked(true);
    settingsStack->setCurrentIndex(0);
    polygonClosed = false;
    polygonFillColor = QColor(86, 189, 248);
    currentFillColor = QColor(247, 118, 142);
    currentBoundaryColor = polygonFillColor;
    colorPickerActive = false;
    colorPickerMode = 0;
    fillConnectivity = 4;
    showTrace = false;
    bezierAnimStep = -1;
    bezierColor = QColor(248, 187, 208); // soft pink
    bezierAnimTimer = new QTimer(this);
    connect(bezierAnimTimer, &QTimer::timeout, this, &MainWindow::animateBezierStep);
    hasArbLine = false;
    hasArbPoint = false;
    keepOriginalOnTransform = false;
    selectedShapeIndex = -1;
    viewScale = 1.0;
    viewOffsetX = 0;
    viewOffsetY = 0;
    isPanning = false;

    connect(btnPolygonClear, &QPushButton::clicked, this, &MainWindow::handlePolygonClearClicked);
    connect(btnPolygonClose, &QPushButton::clicked, this, &MainWindow::handlePolygonCloseClicked);
    connect(btnPolygonColor, &QPushButton::clicked, this, [this]() {
        QColor color = QColorDialog::getColor(polygonFillColor, this, "Select Polygon Edge Color");
        if (color.isValid()) { polygonFillColor = color; currentBoundaryColor = color; }
    });
    connect(btnFillColor, &QPushButton::clicked, this, &MainWindow::handleFillColorClicked);
    connect(btnBoundaryColor, &QPushButton::clicked, this, &MainWindow::handleBoundaryColorClicked);
    connect(btnPickColor, &QPushButton::clicked, this, [this]() {
        QMenu* pickMenu = new QMenu(this);
        pickMenu->setStyleSheet(
            "QMenu { background-color: #1e1e2e; color: #cdd6f4; border: 1px solid #313244; }"
            "QMenu::item:selected { background-color: #89b4fa; color: #11111b; }"
        );
        QAction* pickFill = pickMenu->addAction("Pick Fill Color");
        QAction* pickEdge = pickMenu->addAction("Pick Edge Color");
        QAction* pickBoundary = pickMenu->addAction("Pick Boundary Color");
        QAction* chosen = pickMenu->exec(QCursor::pos());
        if (chosen == pickFill) {
            colorPickerMode = 0;
            colorPickerActive = true;
            statusBar()->showMessage("Click a pixel to pick fill color...");
        } else if (chosen == pickEdge) {
            colorPickerMode = 1;
            colorPickerActive = true;
            statusBar()->showMessage("Click a pixel to pick edge color...");
        } else if (chosen == pickBoundary) {
            colorPickerMode = 2;
            colorPickerActive = true;
            statusBar()->showMessage("Click a pixel to pick boundary color...");
        }
        delete pickMenu;
    });
    
    // Old connections and layouts
    ui->tabWidget->hide();

    animationTimer = new QTimer(this);
    connect(animationTimer, SIGNAL(timeout()), this, SLOT(animateCircleStep()));
    animationStep = -1;

    lineAnimationTimer = new QTimer(this);
    connect(lineAnimationTimer, SIGNAL(timeout()), this, SLOT(animateLineStep()));
    lineAnimationStep = -1;

    isDragging = false;
    animationSpeedMultiplier = 1.0;

    polarTime = 0;
    midpointTime = 0;
    cartesianTime = 0;
    selectedCircleAlgorithm = 0; // Default Polar
    selectedalgorithm = 0; // Default DDA
    ellipseAnimationTimer = new QTimer(this);
    connect(ellipseAnimationTimer, SIGNAL(timeout()), this, SLOT(animateEllipseStep()));
    ellipseAnimationStep = -1;

    ellipsePolarTime = 0;
    ellipseMidpointTime = 0;
    ellipseCartesianTime = 0;
    selectedEllipseAlgorithm = 3; 
    
    ellipseCenter = QPoint(0, 0);
    ellipseRx = 0;
    ellipseRy = 0;
    hasEllipseCenter = false;
    hasEllipseRx = false;
    hasEllipseRy = false;
    ellipseVisible = false;
    ellipseDraggingPoint = 0;
    ellipseQuadColors = false;
    ellipseRegionSplit = false;
    ellipseRotation = 0;
    ellipseThickness = 1;


    connect(ui->tabWidget, &QTabWidget::currentChanged, this, [this](int index) {
        Q_UNUSED(index);
        animationStep = -1;
        lineAnimationStep = -1;
        if (animationTimer->isActive()) animationTimer->stop();
        if (lineAnimationTimer->isActive()) lineAnimationTimer->stop();
        if (ellipseAnimationTimer->isActive()) ellipseAnimationTimer->stop();
        ellipseAnimationStep = -1;
        drawgrid();
    });
    gridsize = ui->spinBox->value();

    if (gridsize < 5)
        gridsize = 25;

    point1 = QPoint(0, 0);
    point2 = QPoint(0, 0);

    haspoint1 = false;
    haspoint2 = false;
    linevisible = false;

    draggingpoint = 0;

    ellipseDraggingPoint = 0;

    selectedalgorithm = 1;

    ddatime = 0;
    bresenhamtime = 0;

    sc_x = 0;
    sc_y = 0;
    org_x = 0;
    org_y = 0;

    connect(ui->frame, SIGNAL(Mouse_Pos()), this, SLOT(mouse_pressed()));
    connect(ui->frame, SIGNAL(sendMousePosition(QPoint&)), this, SLOT(showmouseposition(QPoint&)));
    connect(ui->frame, SIGNAL(sendMouseDrag(QPoint&)), this, SLOT(mouse_dragged(QPoint&)));
    connect(ui->frame, SIGNAL(Mouse_Release()), this, SLOT(mouse_released()));
    connect(ui->frame, SIGNAL(sendMouseWheel(int)), this, SLOT(mouse_wheel(int)));

    ui->frame->installEventFilter(this);

    drawgrid();
    statusBar()->showMessage("Select Point 1");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    drawgrid();
}

QPoint MainWindow::screentological(const QPoint &pos) const
{
    int x = (pos.x() - originx) / gridsize;
    int y = (originy - pos.y()) / gridsize;

    if (pos.x() < originx)
        x--;

    if (pos.y() > originy)
        y--;

    return QPoint(x, y);
}

QPoint MainWindow::logicaltoscreen(const QPoint &pos) const
{
    int x = originx + pos.x() * gridsize;
    int y = originy - (pos.y() + 1) * gridsize;

    return QPoint(x, y);
}

bool MainWindow::nearpoint(const QPoint &logical, const QPoint &target) const
{
    int tolerance = 1;

    return qAbs(logical.x() - target.x()) <= tolerance &&
           qAbs(logical.y() - target.y()) <= tolerance;
}

void MainWindow::drawpoint(QPainter &painter, const QPoint &logical, const QColor &color)
{
    QPoint topLeft = logicaltoscreen(logical);

    int margin;

    if (gridsize <= 6)
        margin = 0;
    else if (gridsize <= 10)
        margin = 1;
    else
        margin = 2;

    int size = gridsize - 2 * margin;

    if (size < 1)
        size = 1;

    painter.fillRect(topLeft.x() + margin, topLeft.y() + margin, size, size, color);
}

void MainWindow::drawaxispixel(QPainter &painter, const QPoint &logical)
{
    QPoint topLeft = logicaltoscreen(logical);
    painter.fillRect(topLeft.x(), topLeft.y(), gridsize, gridsize, Qt::white);
}

void MainWindow::drawMultiColorPoint(QPainter &painter, const QPoint &logical, const QList<QColor> &colors)
{
    if (colors.isEmpty()) return;
    
    QPoint topLeft = logicaltoscreen(logical);
    int margin;
    if (gridsize <= 6) margin = 0;
    else if (gridsize <= 10) margin = 1;
    else margin = 2;
    int size = gridsize - 2 * margin;
    if (size < 1) size = 1;
    
    int x = topLeft.x() + margin;
    int y = topLeft.y() + margin;
    
    if (colors.size() == 1) {
        painter.fillRect(x, y, size, size, colors[0]);
    } else if (colors.size() == 2) {
        painter.fillRect(x, y, size, size / 2, colors[0]);
        painter.fillRect(x, y + size / 2, size, size - size / 2, colors[1]);
    } else if (colors.size() == 3) {
        painter.fillRect(x, y, size, size / 3, colors[0]);
        painter.fillRect(x, y + size / 3, size, size / 3, colors[1]);
        painter.fillRect(x, y + 2 * (size / 3), size, size - 2 * (size / 3), colors[2]);
    } else {
        painter.fillRect(x, y, size, size, colors[0]);
    }
}

void MainWindow::renderPixelBuffer(QPainter &painter)
{
    for (auto it = pixelBuffer.constBegin(); it != pixelBuffer.constEnd(); ++it) {
        drawMultiColorPoint(painter, it.key(), it.value());
    }
}

QVector<QPoint> MainWindow::calculatedda(QPoint p0, QPoint p1, qint64 &time)
{
    QElapsedTimer timer;
    timer.start();
    
    ddaLogs.clear();

    int dx = p1.x() - p0.x();
    int dy = p1.y() - p0.y();

    int steps = std::max(qAbs(dx), qAbs(dy));

    QVector<QPoint> points;
    points.reserve(steps + 1);

    if (steps == 0)
    {
        points.append(p0);
        ddaLogs.append(QString("P1 = P2. X=%1, Y=%2").arg(p0.x()).arg(p0.y()));
        time = timer.nsecsElapsed();
        return points;
    }

    double x = p0.x();
    double y = p0.y();

    double xincrement = static_cast<double>(dx) / steps;
    double yincrement = static_cast<double>(dy) / steps;

    for (int i = 0; i <= steps; ++i)
    {
        points.append(QPoint(qRound(x), qRound(y)));
        ddaLogs.append(QString("Step %1: X=%2, Y=%3 (Rounded: %4, %5)")
                       .arg(i).arg(x, 0, 'f', 2).arg(y, 0, 'f', 2)
                       .arg(qRound(x)).arg(qRound(y)));
        x += xincrement;
        y += yincrement;
    }

    time = timer.nsecsElapsed();

    return points;
}

QVector<QPoint> MainWindow::calculatebresenham(QPoint p0, QPoint p1, qint64 &time)
{
    QElapsedTimer timer;
    timer.start();
    
    bresenhamLogs.clear();

    int x0 = p0.x();
    int y0 = p0.y();
    int x1 = p1.x();
    int y1 = p1.y();

    int dx = qAbs(x1 - x0);
    int dy = qAbs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int error = dx - dy;

    QVector<QPoint> points;
    points.reserve(std::max(dx, dy) + 1);
    
    int step = 0;
    while (true)
    {
        points.append(QPoint(x0, y0));
        bresenhamLogs.append(QString("Step %1: X=%2, Y=%3 | Error P=%4").arg(step++).arg(x0).arg(y0).arg(error));

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * error;

        if (e2 > -dy)
        {
            error -= dy;
            x0 += sx;
        }

        if (e2 < dx)
        {
            error += dx;
            y0 += sy;
        }
    }

    time = timer.nsecsElapsed();

    return points;
}
QString MainWindow::formattime(qint64 nanoseconds) const
{
    if (nanoseconds < 1000)
        return QString::number(nanoseconds) + " ns";

    if (nanoseconds < 1000000)
        return QString::number(nanoseconds / 1000.0, 'f', 3) + " us";

    return QString::number(nanoseconds / 1000000.0, 'f', 3) + " ms";
}

QVector<QPoint> MainWindow::calculateCirclePolar(QPoint center, int radius, qint64 &time)
{
    Q_UNUSED(center);
    QElapsedTimer timer;
    timer.start();
    polarLogs.clear();
    QVector<QPoint> points;
    if (radius == 0) { points.append(QPoint(0,0)); time = timer.nsecsElapsed(); return points; }

    double dtheta = 1.0 / radius; 
    for (double theta = 0; theta <= M_PI_4; theta += dtheta) {
        int x = qRound(radius * qCos(theta));
        int y = qRound(radius * qSin(theta));
        points.append(QPoint(x, y));
        polarLogs.append(QString("Theta=%1 rad | X=%2, Y=%3").arg(theta, 0, 'f', 3).arg(x).arg(y));
    }
    time = timer.nsecsElapsed();
    return points;
}

QVector<QPoint> MainWindow::calculateCircleMidpoint(QPoint center, int radius, qint64 &time)
{
    Q_UNUSED(center);
    QElapsedTimer timer;
    timer.start();
    midpointLogs.clear();
    QVector<QPoint> points;
    int x = 0;
    int y = radius;
    int p = 1 - radius;

    while (x <= y) {
        points.append(QPoint(x, y));
        midpointLogs.append(QString("X=%1, Y=%2 | P=%3").arg(x).arg(y).arg(p));
        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
    }
    time = timer.nsecsElapsed();
    return points;
}

QVector<QPoint> MainWindow::calculateCircleCartesian(QPoint center, int radius, qint64 &time)
{
    Q_UNUSED(center);
    QElapsedTimer timer;
    timer.start();
    cartesianLogs.clear();
    QVector<QPoint> points;
    if (radius == 0) { points.append(QPoint(0,0)); time = timer.nsecsElapsed(); return points; }
    
    int limit = qRound(radius / qSqrt(2.0));
    for (int x = 0; x <= limit; ++x) {
        int y = qRound(qSqrt(radius * radius - x * x));
        points.append(QPoint(x, y));
        cartesianLogs.append(QString("X=%1 | Evaluated Y=%2").arg(x).arg(y));
    }
    time = timer.nsecsElapsed();
    return points;
}

void MainWindow::calculateCircleAlgorithms()
{
    if (!haspoint1 || !haspoint2) return;
    int dx = point2.x() - point1.x();
    int dy = point2.y() - point1.y();
    int radius = qRound(qSqrt(dx*dx + dy*dy));
    
    ui->spinBoxRadius->blockSignals(true);
    ui->spinBoxRadius->setValue(radius);
    ui->spinBoxRadius->blockSignals(false);

    polarPoints = calculateCirclePolar(point1, radius, polarTime);
    midpointPoints = calculateCircleMidpoint(point1, radius, midpointTime);
    cartesianPoints = calculateCircleCartesian(point1, radius, cartesianTime);

    QSet<QPoint> polarSet, midSet, cartSet;
    auto addSymmetryToSet = [this](QSet<QPoint> &set, const QVector<QPoint> &pts) {
        for (const QPoint &p : pts) {
            int x = p.x(); int y = p.y(); int xc = point1.x(); int yc = point1.y();
            set.insert(QPoint(xc + x, yc + y)); set.insert(QPoint(xc - x, yc + y));
            set.insert(QPoint(xc + x, yc - y)); set.insert(QPoint(xc - x, yc - y));
            set.insert(QPoint(xc + y, yc + x)); set.insert(QPoint(xc - y, yc + x));
            set.insert(QPoint(xc + y, yc - x)); set.insert(QPoint(xc - y, yc - x));
        }
    };
    addSymmetryToSet(polarSet, polarPoints);
    addSymmetryToSet(midSet, midpointPoints);
    addSymmetryToSet(cartSet, cartesianPoints);

    ui->lblPolarTime->setText("Polar Time: " + formattime(polarTime));
    ui->lblMidpointTime->setText("Midpoint Time: " + formattime(midpointTime));
    ui->lblCartesianTime->setText("Cartesian Time: " + formattime(cartesianTime));
    ui->lblPolarCount->setText("Polar Pixels: " + QString::number(polarSet.size()));
    ui->lblMidpointCount->setText("Midpoint Pixels: " + QString::number(midSet.size()));
    ui->lblCartesianCount->setText("Cartesian Pixels: " + QString::number(cartSet.size()));

    // ---- CIRCLE ACCURACY REPORT ----
    if (radius > 0) {
        auto radialError = [radius](const QSet<QPoint> &pts) -> QPair<double,double> {
            double avgErr = 0, maxErr = 0;
            for (const QPoint &p : pts) {
                double dist = qSqrt((double)p.x() * p.x() + (double)p.y() * p.y());
                double err = qAbs(dist - radius);
                avgErr += err;
                if (err > maxErr) maxErr = err;
            }
            avgErr /= qMax(1, pts.size());
            return {avgErr, maxErr};
        };

        auto [polarAvg, polarMax] = radialError(polarSet);
        auto [midAvg, midMax] = radialError(midSet);
        auto [cartAvg, cartMax] = radialError(cartSet);

        ui->textDebugger->append(QString("<span style='color:#89b4fa'>[Circle Accuracy]</span> "
            "Radius=%1 | Polar: avg err=%2 max err=%3 | Mid: avg=%4 max=%5 | Cart: avg=%6 max=%7")
            .arg(radius)
            .arg(polarAvg, 0, 'f', 3).arg(polarMax, 0, 'f', 3)
            .arg(midAvg, 0, 'f', 3).arg(midMax, 0, 'f', 3)
            .arg(cartAvg, 0, 'f', 3).arg(cartMax, 0, 'f', 3));
    }
}

void MainWindow::drawCircleSymmetry(QPainter &painter, const QVector<QPoint> &points, const QColor &color, bool addToBuffer)
{
    int xc = point1.x();
    int yc = point1.y();

    for (const QPoint &p : points) {
        int x = p.x();
        int y = p.y();
        QVector<QPoint> sym = {
            QPoint(xc + x, yc + y), QPoint(xc - x, yc + y),
            QPoint(xc + x, yc - y), QPoint(xc - x, yc - y),
            QPoint(xc + y, yc + x), QPoint(xc - y, yc + x),
            QPoint(xc + y, yc - x), QPoint(xc - y, yc - x)
        };
        for (const QPoint &sp : sym) {
            if (addToBuffer) {
                if (!pixelBuffer[sp].contains(color)) pixelBuffer[sp].append(color);
            } else {
                drawpoint(painter, sp, color);
            }
        }
    }
}

void MainWindow::calculatealgorithms()
{
    if (!haspoint1 || !haspoint2)
        return;

    ddapoints = calculatedda(point1, point2, ddatime);
    bresenhampoints = calculatebresenham(point1, point2, bresenhamtime);

    QSet<QPoint> ddaSet(ddapoints.begin(), ddapoints.end());
    QSet<QPoint> bresSet(bresenhampoints.begin(), bresenhampoints.end());

    ui->dda_time_label->setText("DDA Time: " + formattime(ddatime));
    ui->bresenham_time_label->setText("Bresenham Time: " + formattime(bresenhamtime));
    ui->lblDdaCount->setText("DDA Pixels: " + QString::number(ddaSet.size()));
    ui->lblBresenhamCount->setText("Bresenham Pixels: " + QString::number(bresSet.size()));

    // ---- LINE ACCURACY REPORT ----
    double dx = point2.x() - point1.x();
    double dy = point2.y() - point1.y();
    double len = qSqrt(dx * dx + dy * dy);
    if (len > 0) {
        double nx = -dy / len;  // normal to line
        double ny = dx / len;

        // Perpendicular error: avg/max distance of each pixel from ideal line
        double ddaAvgErr = 0, ddaMaxErr = 0;
        for (const QPoint &p : ddapoints) {
            double dist = qAbs((p.x() - point1.x()) * nx + (p.y() - point1.y()) * ny);
            ddaAvgErr += dist;
            if (dist > ddaMaxErr) ddaMaxErr = dist;
        }
        ddaAvgErr /= qMax(1, ddapoints.size());

        double bresAvgErr = 0, bresMaxErr = 0;
        for (const QPoint &p : bresenhampoints) {
            double dist = qAbs((p.x() - point1.x()) * nx + (p.y() - point1.y()) * ny);
            bresAvgErr += dist;
            if (dist > bresMaxErr) bresMaxErr = dist;
        }
        bresAvgErr /= qMax(1, bresenhampoints.size());

        // Overlap ratio: Jaccard similarity between DDA and Bresenham pixel sets
        QSet<QPoint> intersection = ddaSet;
        intersection.intersect(bresSet);
        QSet<QPoint> unionSet = ddaSet;
        unionSet.unite(bresSet);
        double overlap = unionSet.isEmpty() ? 0.0 : (double)intersection.size() / unionSet.size() * 100.0;

        ui->textDebugger->append(QString("<span style='color:#89b4fa'>[Line Accuracy]</span> "
            "DDA: avg err=%1 max err=%2 | Bres: avg err=%3 max err=%4 | Overlap: %5%")
            .arg(ddaAvgErr, 0, 'f', 3).arg(ddaMaxErr, 0, 'f', 3)
            .arg(bresAvgErr, 0, 'f', 3).arg(bresMaxErr, 0, 'f', 3)
            .arg(overlap, 0, 'f', 1));
    }
}

void MainWindow::drawdda(QPainter &painter, const QVector<QPoint> &points, bool addToBuffer)
{
    QColor c(40, 150, 255);
    for (const QPoint &point : points) {
        if (addToBuffer) {
            if (!pixelBuffer[point].contains(c)) pixelBuffer[point].append(c);
        } else {
            drawpoint(painter, point, c);
        }
    }
}

void MainWindow::drawbresenham(QPainter &painter, const QVector<QPoint> &points, bool addToBuffer)
{
    QColor c(255, 210, 0);
    for (const QPoint &point : points) {
        if (addToBuffer) {
            if (!pixelBuffer[point].contains(c)) pixelBuffer[point].append(c);
        } else {
            drawpoint(painter, point, c);
        }
    }
}

void MainWindow::drawgrid()
{
    gridsize = ui->spinBox->value();

    if (gridsize < 5)
        gridsize = 5;

    originx = ui->frame->width() / 2;
    originy = ui->frame->height() / 2;

    QPixmap pix(ui->frame->width(), ui->frame->height());
    pix.fill(QColor(10, 10, 10));

    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing, false);

    QPen gridPen(QColor(150, 150, 150));
    gridPen.setWidth(1);
    painter.setPen(gridPen);

    for (int x = originx; x <= ui->frame->width(); x += gridsize)
        painter.drawLine(x, 0, x, ui->frame->height());

    for (int x = originx - gridsize; x >= 0; x -= gridsize)
        painter.drawLine(x, 0, x, ui->frame->height());

    for (int y = originy; y <= ui->frame->height(); y += gridsize)
        painter.drawLine(0, y, ui->frame->width(), y);

    for (int y = originy - gridsize; y >= 0; y -= gridsize)
        painter.drawLine(0, y, ui->frame->width(), y);

    int limitX = originx / gridsize + 1;
    int limitY = originy / gridsize + 1;
    for (int x = -limitX; x <= limitX; ++x) {
        drawpoint(painter, QPoint(x, 0), QColor(200, 200, 200));
    }
    for (int y = -limitY; y <= limitY; ++y) {
        if (y != 0) drawpoint(painter, QPoint(0, y), QColor(200, 200, 200));
    }


    if (haspoint1 && haspoint2 && linevisible)
    {
        bool isLineTool = (currentTool == TOOL_LINE);
        bool isCircleTool = (currentTool == TOOL_CIRCLE);

        if (isLineTool) {
            if (selectedalgorithm == 2) {
                if (lineAnimationStep >= 0) {
                    int subsetDda = (lineAnimationStep < ddapoints.size()) ? lineAnimationStep : ddapoints.size();
                    int subsetBres = (lineAnimationStep < bresenhampoints.size()) ? lineAnimationStep : bresenhampoints.size();
                    drawdda(painter, ddapoints.mid(0, subsetDda), false);
                    drawbresenham(painter, bresenhampoints.mid(0, subsetBres), false);
                } else {
                    drawdda(painter, ddapoints, false);
                    drawbresenham(painter, bresenhampoints, false);
                }
            } else {
                QVector<QPoint> activePoints = (selectedalgorithm == 0) ? ddapoints : bresenhampoints;
                if (lineAnimationStep >= 0) {
                    int subsetSize = (lineAnimationStep < activePoints.size()) ? lineAnimationStep : activePoints.size();
                    QVector<QPoint> animatedSubset = activePoints.mid(0, subsetSize);
                    if (selectedalgorithm == 0) drawdda(painter, animatedSubset, false);
                    else drawbresenham(painter, animatedSubset, false);
                } else {
                    if (selectedalgorithm == 0) drawdda(painter, ddapoints, false);
                    else drawbresenham(painter, bresenhampoints, false);
                }
            }
        }

        if (isCircleTool) {
            bool circleToBuffer = (draggingpoint == 0 && animationStep < 0);

            if (selectedCircleAlgorithm == 3) {
                if (circleToBuffer) {
                    drawCircleSymmetry(painter, polarPoints, QColor(255, 100, 200), true);
                    drawCircleSymmetry(painter, midpointPoints, QColor(100, 255, 100), true);
                    drawCircleSymmetry(painter, cartesianPoints, QColor(100, 200, 255), true);
                }
                if (animationStep >= 0) {
                    int sPolar = (animationStep < polarPoints.size()) ? animationStep : polarPoints.size();
                    int sMid = (animationStep < midpointPoints.size()) ? animationStep : midpointPoints.size();
                    int sCart = (animationStep < cartesianPoints.size()) ? animationStep : cartesianPoints.size();
                    drawCircleSymmetry(painter, polarPoints.mid(0, sPolar), QColor(255, 100, 200), false);
                    drawCircleSymmetry(painter, midpointPoints.mid(0, sMid), QColor(100, 255, 100), false);
                    drawCircleSymmetry(painter, cartesianPoints.mid(0, sCart), QColor(100, 200, 255), false);
                } else if (!circleToBuffer) {
                    drawCircleSymmetry(painter, polarPoints, QColor(255, 100, 200), false);
                    drawCircleSymmetry(painter, midpointPoints, QColor(100, 255, 100), false);
                    drawCircleSymmetry(painter, cartesianPoints, QColor(100, 200, 255), false);
                }
            } else {
                QVector<QPoint> activePoints;
                QColor color;
                if (selectedCircleAlgorithm == 0) { activePoints = polarPoints; color = QColor(255, 100, 200); }
                else if (selectedCircleAlgorithm == 1) { activePoints = midpointPoints; color = QColor(100, 255, 100); }
                else { activePoints = cartesianPoints; color = QColor(100, 200, 255); }
                
                if (circleToBuffer) {
                    drawCircleSymmetry(painter, activePoints, color, true);
                }
                if (animationStep >= 0) {
                    int subsetSize = (animationStep < activePoints.size()) ? animationStep : activePoints.size();
                    QVector<QPoint> animatedSubset = activePoints.mid(0, subsetSize);
                    drawCircleSymmetry(painter, animatedSubset, color, false);
                } else if (!circleToBuffer) {
                    drawCircleSymmetry(painter, activePoints, color, false);
                }
            }
        }
    }
    
    // Draw Persistent Ellipses (always visible)
    for (const PersistentEllipse &pe : persistentEllipses) {
        qint64 dummy_time = 0;
        QVector<QPoint> pts;
        if (pe.algorithm == 0) pts = calculateEllipsePolar(pe.center, pe.rx, pe.ry, dummy_time);
        else if (pe.algorithm == 1) pts = calculateEllipseMidpoint(pe.center, pe.rx, pe.ry, dummy_time);
        else if (pe.algorithm == 2) pts = calculateEllipseCartesian(pe.center, pe.rx, pe.ry, dummy_time);
        else {
            pts = calculateEllipsePolar(pe.center, pe.rx, pe.ry, dummy_time);
            drawEllipseSymmetry(painter, pts, QColor(255, 0, 127, 80), pe.center, pe.rx, pe.ry, pe.rotation, pe.thickness, true, false);
            pts = calculateEllipseMidpoint(pe.center, pe.rx, pe.ry, dummy_time);
            drawEllipseSymmetry(painter, pts, QColor(0, 245, 212, 80), pe.center, pe.rx, pe.ry, pe.rotation, pe.thickness, true, false);
            pts = calculateEllipseCartesian(pe.center, pe.rx, pe.ry, dummy_time);
            drawEllipseSymmetry(painter, pts, QColor(56, 189, 248, 80), pe.center, pe.rx, pe.ry, pe.rotation, pe.thickness, true, false);
            continue;
        }
        drawEllipseSymmetry(painter, pts, pe.color, pe.center, pe.rx, pe.ry, pe.rotation, pe.thickness, true, false);
    }

    // Draw active ellipse preview
    bool ellipseToBuffer = (currentTool == TOOL_ELLIPSE && ellipseVisible && ellipseDraggingPoint == 0 && ellipseAnimationStep < 0);
    if (currentTool == TOOL_ELLIPSE && ellipseVisible) {
            if (selectedEllipseAlgorithm == 3) {
                if (ellipseToBuffer) {
                    drawEllipseSymmetry(painter, ellipsePolarPoints, QColor(255, 0, 127), ellipseCenter, ellipseRx, ellipseRy, ellipseRotation, ellipseThickness, true, false);
                    drawEllipseSymmetry(painter, ellipseMidpointPoints, QColor(0, 245, 212), ellipseCenter, ellipseRx, ellipseRy, ellipseRotation, ellipseThickness, true, ellipseRegionSplit);
                    drawEllipseSymmetry(painter, ellipseCartesianPoints, QColor(56, 189, 248), ellipseCenter, ellipseRx, ellipseRy, ellipseRotation, ellipseThickness, true, false);
                }
                if (ellipseAnimationStep >= 0) {
                    int sPol = (ellipseAnimationStep < ellipsePolarPoints.size()) ? ellipseAnimationStep : ellipsePolarPoints.size();
                    int sMid = (ellipseAnimationStep < ellipseMidpointPoints.size()) ? ellipseAnimationStep : ellipseMidpointPoints.size();
                    int sCar = (ellipseAnimationStep < ellipseCartesianPoints.size()) ? ellipseAnimationStep : ellipseCartesianPoints.size();
                    drawEllipseSymmetry(painter, ellipsePolarPoints.mid(0, sPol), QColor(255, 0, 127), ellipseCenter, ellipseRx, ellipseRy, ellipseRotation, ellipseThickness, false, false);
                    drawEllipseSymmetry(painter, ellipseMidpointPoints.mid(0, sMid), QColor(0, 245, 212), ellipseCenter, ellipseRx, ellipseRy, ellipseRotation, ellipseThickness, false, ellipseRegionSplit);
                    drawEllipseSymmetry(painter, ellipseCartesianPoints.mid(0, sCar), QColor(56, 189, 248), ellipseCenter, ellipseRx, ellipseRy, ellipseRotation, ellipseThickness, false, false);
                } else if (!ellipseToBuffer) {
                    drawEllipseSymmetry(painter, ellipsePolarPoints, QColor(255, 0, 127), ellipseCenter, ellipseRx, ellipseRy, ellipseRotation, ellipseThickness, false, false);
                    drawEllipseSymmetry(painter, ellipseMidpointPoints, QColor(0, 245, 212), ellipseCenter, ellipseRx, ellipseRy, ellipseRotation, ellipseThickness, false, ellipseRegionSplit);
                    drawEllipseSymmetry(painter, ellipseCartesianPoints, QColor(56, 189, 248), ellipseCenter, ellipseRx, ellipseRy, ellipseRotation, ellipseThickness, false, false);
                }
            } else {
                QVector<QPoint> activePoints;
                QColor color;
                if (selectedEllipseAlgorithm == 0) { activePoints = ellipsePolarPoints; color = QColor(255, 0, 127); }
                else if (selectedEllipseAlgorithm == 1) { activePoints = ellipseMidpointPoints; color = QColor(0, 245, 212); }
                else { activePoints = ellipseCartesianPoints; color = QColor(56, 189, 248); }
                
                if (ellipseToBuffer) {
                    drawEllipseSymmetry(painter, activePoints, color, ellipseCenter, ellipseRx, ellipseRy, ellipseRotation, ellipseThickness, true, (selectedEllipseAlgorithm == 1 && ellipseRegionSplit));
                }
                if (ellipseAnimationStep >= 0) {
                    int subsetSize = (ellipseAnimationStep < activePoints.size()) ? ellipseAnimationStep : activePoints.size();
                    QVector<QPoint> animatedSubset = activePoints.mid(0, subsetSize);
                    drawEllipseSymmetry(painter, animatedSubset, color, ellipseCenter, ellipseRx, ellipseRy, ellipseRotation, ellipseThickness, false, (selectedEllipseAlgorithm == 1 && ellipseRegionSplit));
                } else if (!ellipseToBuffer) {
                    drawEllipseSymmetry(painter, activePoints, color, ellipseCenter, ellipseRx, ellipseRy, ellipseRotation, ellipseThickness, false, (selectedEllipseAlgorithm == 1 && ellipseRegionSplit));
                }
            }
        }
    
    if (currentTool == TOOL_ELLIPSE) {
        if (hasEllipseCenter) drawpoint(painter, ellipseCenter, QColor(247, 118, 142));
        if (hasEllipseRx) drawpoint(painter, QPoint(ellipseCenter.x() + ellipseRx, ellipseCenter.y()), QColor(122, 162, 247));
        if (hasEllipseRy) drawpoint(painter, QPoint(ellipseCenter.x(), ellipseCenter.y() + ellipseRy), QColor(115, 218, 202));
    }

    if (haspoint1)
        drawpoint(painter, point1, QColor(255, 70, 70));

    if (haspoint2)
        drawpoint(painter, point2, QColor(70, 255, 120));

    if (currentTool == TOOL_POLYGON || currentTool == TOOL_FLOOD_FILL || currentTool == TOOL_BOUNDARY_FILL || currentTool == TOOL_SCANLINE_FILL) {
        for (const QPoint &pt : activePolygonPoints) {
            drawpoint(painter, pt, polygonFillColor);
        }
        if (activePolygonPoints.size() >= 2) {
            qint64 dummyTime = 0;
            for (int i = 0; i < activePolygonPoints.size() - 1; ++i) {
                QVector<QPoint> edge = calculatedda(activePolygonPoints[i], activePolygonPoints[i + 1], dummyTime);
                for (const QPoint &p : edge) {
                    drawpoint(painter, p, polygonFillColor);
                }
            }
        }
    }

    // ---- BEZIER CURVE ----
    if (currentTool == TOOL_CURVE) {
        // Draw control polygon (dashed)
        if (bezierControlPoints.size() >= 2) {
            QPen ctrlPen(QColor(180, 180, 180, 120));
            ctrlPen.setWidth(1);
            ctrlPen.setStyle(Qt::DashLine);
            painter.setPen(ctrlPen);
            for (int i = 0; i < bezierControlPoints.size() - 1; ++i) {
                QPoint s1 = logicaltoscreen(bezierControlPoints[i]);
                QPoint s2 = logicaltoscreen(bezierControlPoints[i + 1]);
                painter.drawLine(s1, s2);
            }
        }
        // Draw control points
        for (int i = 0; i < bezierControlPoints.size(); ++i) {
            QColor ptColor = (i == 0 || i == 3) ? QColor(248, 187, 208) : QColor(137, 180, 250);
            drawpoint(painter, bezierControlPoints[i], ptColor);
        }
        // Draw evaluated curve
        if (bezierCurvePoints.size() >= 2) {
            QPen curvePen(bezierColor);
            curvePen.setWidth(2);
            painter.setPen(curvePen);
            for (int i = 0; i < bezierCurvePoints.size() - 1; ++i) {
                QPoint s1 = logicaltoscreen(bezierCurvePoints[i]);
                QPoint s2 = logicaltoscreen(bezierCurvePoints[i + 1]);
                painter.drawLine(s1, s2);
            }
        }
        // Draw animated construction (if stepping)
        if (bezierAnimStep >= 0 && bezierAnimStep < bezierAnimPoints.size()) {
            QPoint ap = bezierAnimPoints[bezierAnimStep];
            QPoint sp = logicaltoscreen(ap);
            painter.fillRect(sp.x() - gridsize / 2 + 1, sp.y() - gridsize / 2 + 1,
                             gridsize - 2, gridsize - 2,
                             QColor(255, 255, 80, 180));
            painter.setPen(QPen(QColor(255, 255, 80), 1));
            painter.drawRect(sp.x() - gridsize / 2, sp.y() - gridsize / 2,
                             gridsize - 1, gridsize - 1);
            if (bezierAnimStep < bezierTraceTags.size()) {
                QFont traceFont("Menlo", 9);
                painter.setFont(traceFont);
                painter.setPen(QColor(255, 255, 255));
                painter.drawText(sp.x() + gridsize / 2 + 4, sp.y() + 4,
                                 bezierTraceTags[bezierAnimStep]);
            }
        }
    }

    // ---- ON-CANVAS ALGORITHM TRACE ----
    if (showTrace) {
        int step = getActiveAnimationStep();
        QVector<QPoint> points = getActiveAnimationPoints();
        int total = points.size();
        if (step >= 0 && step < total && step < traceTags.size()) {
            QPoint traceLocal = points[step];
            QPoint traceCenter(0, 0);
            if (currentTool == TOOL_CIRCLE && haspoint1) traceCenter = point1;
            else if (currentTool == TOOL_ELLIPSE && hasEllipseCenter) traceCenter = ellipseCenter;
            QPoint traceLogical(traceLocal.x() + traceCenter.x(), traceLocal.y() + traceCenter.y());
            QPoint traceScreen = logicaltoscreen(traceLogical);

            // Highlight trail (dim) for recent steps
            int trailStart = qMax(0, step - 5);
            for (int t = trailStart; t < step; ++t) {
                if (t >= points.size()) break;
                QPoint lp = points[t];
                QPoint sl(lp.x() + traceCenter.x(), lp.y() + traceCenter.y());
                QPoint sc = logicaltoscreen(sl);
                float fade = 0.15 + 0.15 * (t - trailStart);
                painter.fillRect(sc.x() - gridsize / 2 + 1, sc.y() - gridsize / 2 + 1,
                                 gridsize - 2, gridsize - 2,
                                 QColor(255, 255, 100, (int)(fade * 255)));
            }

            // Current step highlight
            painter.fillRect(traceScreen.x() - gridsize / 2 + 1, traceScreen.y() - gridsize / 2 + 1,
                             gridsize - 2, gridsize - 2,
                             QColor(255, 255, 80, 180));
            painter.setPen(QPen(QColor(255, 255, 80), 1));
            painter.drawRect(traceScreen.x() - gridsize / 2, traceScreen.y() - gridsize / 2,
                             gridsize - 1, gridsize - 1);

            // Tag text
            QFont traceFont("Menlo", 9);
            painter.setFont(traceFont);
            painter.setPen(QColor(255, 255, 255));
            painter.drawText(traceScreen.x() + gridsize / 2 + 4, traceScreen.y() + 4, traceTags[step]);
        }
    }

    // ---- PERSISTENT SCENE SHAPES ----
    drawSceneShapes(painter);

    // ---- TRANSFORM OVERLAYS ----
    if (hasArbLine) {
        QPoint s1 = logicaltoscreen(QPoint((int)arbLineX1, (int)arbLineY1));
        QPoint s2 = logicaltoscreen(QPoint((int)arbLineX2, (int)arbLineY2));
        QPen linePen(QColor(255, 200, 0, 200));
        linePen.setWidth(2);
        linePen.setStyle(Qt::DashDotLine);
        painter.setPen(linePen);
        painter.drawLine(s1, s2);
        drawpoint(painter, QPoint((int)arbLineX1, (int)arbLineY1), QColor(255, 200, 0));
        drawpoint(painter, QPoint((int)arbLineX2, (int)arbLineY2), QColor(255, 200, 0));
        QFont ovlFont("Menlo", 9);
        painter.setFont(ovlFont);
        painter.setPen(QColor(255, 200, 0));
        painter.drawText(s2.x() + 8, s2.y() - 4, "Mirror");
    }
    if (hasArbPoint) {
        QPoint sp = logicaltoscreen(QPoint((int)arbPointX, (int)arbPointY));
        painter.setPen(QPen(QColor(0, 220, 255, 220), 2));
        int r = gridsize;
        painter.drawLine(sp.x() - r, sp.y(), sp.x() + r, sp.y());
        painter.drawLine(sp.x(), sp.y() - r, sp.x(), sp.y() + r);
        painter.drawRect(sp.x() - r / 2, sp.y() - r / 2, r, r);
        drawpoint(painter, QPoint((int)arbPointX, (int)arbPointY), QColor(0, 220, 255));
        QFont ovlFont("Menlo", 9);
        painter.setFont(ovlFont);
        painter.setPen(QColor(0, 220, 255));
        painter.drawText(sp.x() + r + 4, sp.y() - 4, "Pivot");
    }

    renderPixelBuffer(painter);

    painter.end();

    ui->frame->setPixmap(pix);
}

void MainWindow::showmouseposition(QPoint &pos)
{
    QPoint logical = screentological(pos);

    sc_x = logical.x();
    sc_y = logical.y();

    org_x = pos.x();
    org_y = pos.y();



    statusBar()->showMessage("Mouse (Screen): ( " + QString::number(pos.x()) + " , " + QString::number(pos.y()) + " )    |    Mouse (Cartesian): ( " + QString::number(logical.x()) + " , " + QString::number(logical.y()) + " )    |    Grid: " + QString::number(gridsize));
}

void MainWindow::mouse_pressed()
{
    animationStep = -1;
    lineAnimationStep = -1;
    if (animationTimer->isActive()) animationTimer->stop();
    if (lineAnimationTimer->isActive()) lineAnimationTimer->stop();
        if (ellipseAnimationTimer->isActive()) ellipseAnimationTimer->stop();
        ellipseAnimationStep = -1;
    
    QPoint clickpos(org_x, org_y);
    QPoint logical = screentological(clickpos);

    if (colorPickerActive) {
        colorPickerActive = false;
        QColor picked;
        if (pixelBuffer.contains(logical) && !pixelBuffer[logical].isEmpty()) {
            picked = pixelBuffer[logical].last();
        } else {
            QPixmap pix = ui->frame->pixmap();
            if (!pix.isNull()) {
                QColor c = pix.toImage().pixelColor(org_x, org_y);
                if (c.isValid() && c != QColor(10, 10, 10)) {
                    picked = c;
                }
            }
        }
        if (picked.isValid()) {
            if (colorPickerMode == 0) {
                currentFillColor = picked;
                statusBar()->showMessage("Fill color set: " + picked.name());
            } else if (colorPickerMode == 1) {
                polygonFillColor = picked;
                currentBoundaryColor = picked;
                statusBar()->showMessage("Edge color set: " + picked.name());
            } else if (colorPickerMode == 2) {
                currentBoundaryColor = picked;
                statusBar()->showMessage("Boundary color set: " + picked.name());
            }
        } else {
            statusBar()->showMessage("No color found at that pixel.");
        }
        return;
    }

    if (currentTool == TOOL_TRANSFORM) {
        int hit = hitTestShape(logical);
        if (hit >= 0) {
            selectedShapeIndex = hit;
            lastDragLogical = logical;
            pushUndo();
            statusBar()->showMessage("Shape selected. Press Delete to remove, or drag to move.");
            drawgrid();
            return;
        }
        selectedShapeIndex = -1;
        drawgrid();
        return;
    }

    if (currentTool == TOOL_ELLIPSE) {
        if (hasEllipseCenter && nearpoint(logical, ellipseCenter)) {
            if (ellipseVisible) {
                qint64 dt = 0;
                QVector<QPoint> oldPts = calculateEllipsePolar(ellipseCenter, ellipseRx, ellipseRy, dt);
                removeShapeFromBuffer(oldPts, QColor(255, 0, 127));
                oldPts = calculateEllipseMidpoint(ellipseCenter, ellipseRx, ellipseRy, dt);
                removeShapeFromBuffer(oldPts, QColor(0, 245, 212));
                oldPts = calculateEllipseCartesian(ellipseCenter, ellipseRx, ellipseRy, dt);
                removeShapeFromBuffer(oldPts, QColor(56, 189, 248));
            }
            ellipseDraggingPoint = 1; statusBar()->showMessage("Dragging Ellipse Center"); return;
        }
        if (hasEllipseRx && nearpoint(logical, QPoint(ellipseCenter.x() + ellipseRx, ellipseCenter.y()))) {
            if (ellipseVisible) {
                qint64 dt = 0;
                QVector<QPoint> oldPts = calculateEllipsePolar(ellipseCenter, ellipseRx, ellipseRy, dt);
                removeShapeFromBuffer(oldPts, QColor(255, 0, 127));
                oldPts = calculateEllipseMidpoint(ellipseCenter, ellipseRx, ellipseRy, dt);
                removeShapeFromBuffer(oldPts, QColor(0, 245, 212));
                oldPts = calculateEllipseCartesian(ellipseCenter, ellipseRx, ellipseRy, dt);
                removeShapeFromBuffer(oldPts, QColor(56, 189, 248));
            }
            ellipseDraggingPoint = 2; statusBar()->showMessage("Dragging Ellipse Radius X"); return;
        }
        if (hasEllipseRy && nearpoint(logical, QPoint(ellipseCenter.x(), ellipseCenter.y() + ellipseRy))) {
            if (ellipseVisible) {
                qint64 dt = 0;
                QVector<QPoint> oldPts = calculateEllipsePolar(ellipseCenter, ellipseRx, ellipseRy, dt);
                removeShapeFromBuffer(oldPts, QColor(255, 0, 127));
                oldPts = calculateEllipseMidpoint(ellipseCenter, ellipseRx, ellipseRy, dt);
                removeShapeFromBuffer(oldPts, QColor(0, 245, 212));
                oldPts = calculateEllipseCartesian(ellipseCenter, ellipseRx, ellipseRy, dt);
                removeShapeFromBuffer(oldPts, QColor(56, 189, 248));
            }
            ellipseDraggingPoint = 3; statusBar()->showMessage("Dragging Ellipse Radius Y"); return;
        }

        if (!hasEllipseCenter) {
            ellipseCenter = logical; hasEllipseCenter = true;
            ui->comboEllipsePoint->setItemText(0, "Center : (" + QString::number(ellipseCenter.x()) + ", " + QString::number(ellipseCenter.y()) + ")");
            drawgrid(); statusBar()->showMessage("Center selected. Now select Point 2 for Radius X."); return;
        }
        if (!hasEllipseRx) {
            ellipseRx = qAbs(logical.x() - ellipseCenter.x());
            if (ellipseRx == 0) ellipseRx = 1;
            hasEllipseRx = true;
            ui->spinBoxRx->blockSignals(true); ui->spinBoxRx->setValue(ellipseRx); ui->spinBoxRx->blockSignals(false);
            ui->comboEllipsePoint->setItemText(1, "Radius X : " + QString::number(ellipseRx));
            drawgrid(); statusBar()->showMessage("Radius X set. Now select Point 3 for Radius Y."); return;
        }
        if (!hasEllipseRy) {
            ellipseRy = qAbs(logical.y() - ellipseCenter.y());
            if (ellipseRy == 0) ellipseRy = 1;
            hasEllipseRy = true; ellipseVisible = true;
            ui->spinBoxRy->blockSignals(true); ui->spinBoxRy->setValue(ellipseRy); ui->spinBoxRy->blockSignals(false);
            ui->comboEllipsePoint->setItemText(2, "Radius Y : " + QString::number(ellipseRy));
            calculateEllipseAlgorithms();
            on_btnAnimateEllipse_clicked();
            Shape s; s.type = Shape::ELLIPSE; s.color = QColor(255, 0, 127);
            s.ellipseCenter = ellipseCenter;
            s.ellipseRx = ellipseRx; s.ellipseRy = ellipseRy;
            s.selected = false;
            addShapeToScene(s);
            drawgrid(); statusBar()->showMessage("Ellipse fully defined."); return;
        }
        return;
    } else if (currentTool == TOOL_POLYGON || currentTool == TOOL_SCANLINE_FILL) {
        if (currentTool == TOOL_SCANLINE_FILL && lastClosedPolygonVertices.size() >= 3) {
            scanlineFillPolygon(lastClosedPolygonVertices, currentFillColor, &committedPolygonPixels);
            drawgrid();
        } else if (currentTool == TOOL_POLYGON && !polygonClosed) {
            activePolygonPoints.append(logical);
            drawgrid();
            statusBar()->showMessage(QString("Polygon point %1 added.").arg(activePolygonPoints.size()));
        }
        return;
    } else if (currentTool == TOOL_FLOOD_FILL) {
        floodFill(logical, QColor(), currentFillColor); // Target color is detected inside floodFill
        drawgrid();
        return;
    } else if (currentTool == TOOL_BOUNDARY_FILL) {
        boundaryFill(logical, currentFillColor, currentBoundaryColor);
        drawgrid();
        return;
    } else if (currentTool == TOOL_CURVE) {
        if (bezierControlPoints.size() < 4) {
            bezierControlPoints.append(logical);
            drawgrid();
            statusBar()->showMessage(QString("Control point %1 added.").arg(bezierControlPoints.size()));
        }
        return;
    }

    if (haspoint1 && nearpoint(logical, point1))
    {
        if (haspoint2 && linevisible && currentTool == TOOL_CIRCLE) {
            int radius = qRound(qSqrt(qPow(point1.x() - point2.x(), 2) + qPow(point1.y() - point2.y(), 2)));
            if (radius > 0) {
                qint64 dt = 0;
                QVector<QPoint> oldPts = calculateCirclePolar(point1, radius, dt);
                removeShapeFromBuffer(oldPts, QColor(255, 100, 200));
                oldPts = calculateCircleMidpoint(point1, radius, dt);
                removeShapeFromBuffer(oldPts, QColor(100, 255, 100));
                oldPts = calculateCircleCartesian(point1, radius, dt);
                removeShapeFromBuffer(oldPts, QColor(100, 200, 255));
            }
        }
        draggingpoint = 1;
        statusBar()->showMessage("Dragging Point 1");
        return;
    }

    if (haspoint2 && nearpoint(logical, point2))
    {
        if (haspoint1 && linevisible && currentTool == TOOL_CIRCLE) {
            int radius = qRound(qSqrt(qPow(point1.x() - point2.x(), 2) + qPow(point1.y() - point2.y(), 2)));
            if (radius > 0) {
                qint64 dt = 0;
                QVector<QPoint> oldPts = calculateCirclePolar(point1, radius, dt);
                removeShapeFromBuffer(oldPts, QColor(255, 100, 200));
                oldPts = calculateCircleMidpoint(point1, radius, dt);
                removeShapeFromBuffer(oldPts, QColor(100, 255, 100));
                oldPts = calculateCircleCartesian(point1, radius, dt);
                removeShapeFromBuffer(oldPts, QColor(100, 200, 255));
            }
        }
        draggingpoint = 2;
        statusBar()->showMessage("Dragging Point 2");
        return;
    }

    if (!haspoint1)
    {
        point1 = logical;
        haspoint1 = true;
        linevisible = false;

        ui->comboLinePoint->setItemText(0, "Point 1 : (" + QString::number(point1.x()) + ", " + QString::number(point1.y()) + ")");
        ui->comboCirclePoint->setItemText(0, "Center : (" + QString::number(point1.x()) + ", " + QString::number(point1.y()) + ")");

        drawgrid();

        statusBar()->showMessage("Point 1 selected. Now select Point 2.");
        return;
    }

    if (!haspoint2)
    {
        point2 = logical;
        haspoint2 = true;
        linevisible = true;

        ui->comboLinePoint->setItemText(1, "Point 2 : (" + QString::number(point2.x()) + ", " + QString::number(point2.y()) + ")");
        ui->comboCirclePoint->setItemText(1, "Radius Pt : (" + QString::number(point2.x()) + ", " + QString::number(point2.y()) + ")");

        if (currentTool == TOOL_LINE) {
            calculatealgorithms();
            on_btnAnimateLine_clicked(); // Auto animate on click
            Shape s; s.type = Shape::LINE; s.color = QColor(255, 255, 255);
            s.lineP1 = point1; s.lineP2 = point2; s.selected = false;
            addShapeToScene(s);
        } else if (currentTool == TOOL_CIRCLE) {
            calculateCircleAlgorithms();
            on_btnAnimateCircle_clicked(); // Auto animate on click
            Shape s; s.type = Shape::CIRCLE; s.color = QColor(100, 255, 100);
            s.circleCenter = point1;
            s.circleRadius = qRound(qSqrt(qPow(point1.x()-point2.x(),2)+qPow(point1.y()-point2.y(),2)));
            s.selected = false;
            addShapeToScene(s);
        }
            
        drawgrid();

        statusBar()->showMessage("Both points selected.");
        return;
    }

    statusBar()->showMessage("Drag Point 1 or Point 2 to move the line.");
}

void MainWindow::mouse_dragged(QPoint &pos)
{
    isDragging = true;
    animationStep = -1;
    lineAnimationStep = -1;
    if (animationTimer->isActive()) animationTimer->stop();
    if (lineAnimationTimer->isActive()) lineAnimationTimer->stop();
        if (ellipseAnimationTimer->isActive()) ellipseAnimationTimer->stop();
        ellipseAnimationStep = -1;

    if (currentTool == TOOL_TRANSFORM && selectedShapeIndex >= 0 && selectedShapeIndex < scene.size()) {
        QPoint logical = screentological(pos);
        QPoint delta = logical - lastDragLogical;
        lastDragLogical = logical;
        Shape &s = scene[selectedShapeIndex];
        switch (s.type) {
        case Shape::LINE:
            s.lineP1 += delta; s.lineP2 += delta; break;
        case Shape::CIRCLE:
            s.circleCenter += delta; break;
        case Shape::ELLIPSE:
            s.ellipseCenter += delta; break;
        case Shape::POLYGON:
            for (QPoint &v : s.polygonVertices) v += delta; break;
        case Shape::BEZIER:
            for (QPoint &cp : s.bezierControlPoints) cp += delta;
            for (QPoint &bp : s.bezierCurvePoints) bp += delta; break;
        }
        pixelBuffer.clear();
        for (Shape &sh : scene) rasterizeShapeToBuffer(sh);
        drawgrid();
        return;
    }
    
    
    if (currentTool == TOOL_ELLIPSE) {
        if (ellipseDraggingPoint == 0) return;
        QPoint logical = screentological(pos);
        if (ellipseDraggingPoint == 1 && hasEllipseCenter) {
            ellipseCenter = logical;
            ui->comboEllipsePoint->setItemText(0, "Center : (" + QString::number(ellipseCenter.x()) + ", " + QString::number(ellipseCenter.y()) + ")");
        } else if (ellipseDraggingPoint == 2 && hasEllipseRx) {
            ellipseRx = qAbs(logical.x() - ellipseCenter.x());
            if (ellipseRx == 0) ellipseRx = 1;
            ui->spinBoxRx->blockSignals(true); ui->spinBoxRx->setValue(ellipseRx); ui->spinBoxRx->blockSignals(false);
            ui->comboEllipsePoint->setItemText(1, "Radius X : " + QString::number(ellipseRx));
        } else if (ellipseDraggingPoint == 3 && hasEllipseRy) {
            ellipseRy = qAbs(logical.y() - ellipseCenter.y());
            if (ellipseRy == 0) ellipseRy = 1;
            ui->spinBoxRy->blockSignals(true); ui->spinBoxRy->setValue(ellipseRy); ui->spinBoxRy->blockSignals(false);
            ui->comboEllipsePoint->setItemText(2, "Radius Y : " + QString::number(ellipseRy));
        }
        if (hasEllipseCenter && hasEllipseRx && hasEllipseRy) {
            calculateEllipseAlgorithms();
        }
        drawgrid();
        return;
    }

    if (draggingpoint == 0)
        return;

    QPoint logical = screentological(pos);

    if (draggingpoint == 1)
    {
        point1 = logical;
        ui->comboLinePoint->setItemText(0, "Point 1 : (" + QString::number(point1.x()) + ", " + QString::number(point1.y()) + ")");
        ui->comboCirclePoint->setItemText(0, "Center : (" + QString::number(point1.x()) + ", " + QString::number(point1.y()) + ")");
    }
    else if (draggingpoint == 2)
    {
        point2 = logical;
        ui->comboLinePoint->setItemText(1, "Point 2 : (" + QString::number(point2.x()) + ", " + QString::number(point2.y()) + ")");
        ui->comboCirclePoint->setItemText(1, "Radius Pt : (" + QString::number(point2.x()) + ", " + QString::number(point2.y()) + ")");
    }

    if (currentTool == TOOL_LINE) {
        calculatealgorithms();
    } else if (currentTool == TOOL_CIRCLE) {
        calculateCircleAlgorithms();
    }

    drawgrid();

    statusBar()->showMessage("Point 1: (" + QString::number(point1.x()) + ", " + QString::number(point1.y()) + ")    |    Point 2: (" + QString::number(point2.x()) + ", " + QString::number(point2.y()) + ")    |    Dragging...");
}

void MainWindow::mouse_released()
{
    draggingpoint = 0;

    ellipseDraggingPoint = 0;

    isDragging = false;

    if (haspoint1 && haspoint2)
        statusBar()->showMessage("Line updated.");
}

void MainWindow::mouse_wheel(int delta)
{
    if (delta > 0)
    {
        gridsize += 2;
    }
    else if (delta < 0)
    {
        gridsize -= 2;
        if (gridsize < 5) gridsize = 5;
    }
    
    ui->spinBox->blockSignals(true);
    ui->spinBox->setValue(gridsize);
    ui->spinBox->blockSignals(false);
    
    drawgrid();
}

void MainWindow::on_clear_clicked()
{
    haspoint1 = false;
    haspoint2 = false;
    linevisible = false;
    draggingpoint = 0;

    ellipseDraggingPoint = 0;


    point1 = QPoint(0, 0);
    point2 = QPoint(0, 0);

    ddapoints.clear();
    bresenhampoints.clear();

    ddatime = 0;
    bresenhamtime = 0;
    
    lineAnimationStep = -1;
    if (lineAnimationTimer->isActive()) lineAnimationTimer->stop();
        if (ellipseAnimationTimer->isActive()) ellipseAnimationTimer->stop();
        ellipseAnimationStep = -1;
    
    ui->comboLinePoint->setItemText(0, "Select Point 1");
    ui->comboLinePoint->setItemText(1, "Select Point 2");
    ui->dda_time_label->setText("DDA Time: -");
    ui->bresenham_time_label->setText("Bresenham Time: -");
    ui->lblDdaCount->setText("DDA Pixels: -");
    ui->lblBresenhamCount->setText("Bresenham Pixels: -");

    drawgrid();

    statusBar()->showMessage("Cleared. Select Point 1.");
}

void MainWindow::on_draw_line_clicked()
{
    if (!haspoint1 || !haspoint2)
    {
        statusBar()->showMessage("Select both points first.");
        return;
    }

    calculatealgorithms();
    linevisible = true;
    drawgrid();

    if (selectedalgorithm == 0)
        statusBar()->showMessage("DDA line drawn.");
    else if (selectedalgorithm == 1)
        statusBar()->showMessage("Bresenham line drawn.");
    else
        statusBar()->showMessage("Both lines drawn.");
}

void MainWindow::on_btnAnimateLine_clicked()
{
    if (!haspoint1 || !haspoint2) {
        statusBar()->showMessage("Select both points first.");
        return;
    }
    calculatealgorithms();
    
    ui->textDebugger->clear();
    traceTags.clear();
    if (selectedalgorithm == 0 || selectedalgorithm == 2) {
        lineAnimationPoints = ddapoints;
        lineAnimationLogs = ddaLogs;
        ui->textDebugger->append("Starting DDA Line Animation...");
        for (int i = 0; i < ddaLogs.size(); ++i)
            traceTags.append(extractTraceTag(ddaLogs, i, 0));
    } else {
        lineAnimationPoints = bresenhampoints;
        lineAnimationLogs = bresenhamLogs;
        ui->textDebugger->append("Starting Bresenham Line Animation...");
        for (int i = 0; i < bresenhamLogs.size(); ++i)
            traceTags.append(extractTraceTag(bresenhamLogs, i, 1));
    }
    
    lineAnimationStep = 0;
    syncPlaybackState();
    int interval = qRound(20 / animationSpeedMultiplier);
    if (interval < 1) interval = 1;
    lineAnimationTimer->start(interval);
    statusBar()->showMessage("Animating Line...");
}

void MainWindow::animateLineStep()
{
    if (lineAnimationStep < 0 || lineAnimationStep >= lineAnimationPoints.size()) {
        lineAnimationTimer->stop();
        statusBar()->showMessage("Animation Complete.");
        syncPlaybackState();
        return;
    }
    if (lineAnimationStep < lineAnimationLogs.size()) {
        ui->textDebugger->append(lineAnimationLogs[lineAnimationStep]);
    }
    lineAnimationStep++;
    syncPlaybackState();
    drawgrid();
}

void MainWindow::on_spinBox_textChanged(const QString &)
{
    gridsize = ui->spinBox->value();

    if (gridsize < 5)
        gridsize = 5;

    drawgrid();
}

void MainWindow::on_spinBox_valueChanged(int)
{
    gridsize = ui->spinBox->value();

    if (gridsize < 5)
        gridsize = 5;

    drawgrid();
}


void MainWindow::on_btnAnimateCircle_clicked()
{
    if (!haspoint1 || !haspoint2) {
        statusBar()->showMessage("Select Center (Point 1) and Radius (Point 2) first.");
        return;
    }
    
    calculateCircleAlgorithms();
    
    ui->textDebugger->clear();
    traceTags.clear();
    if (selectedCircleAlgorithm == 0) {
        animationPoints = polarPoints; animationLogs = polarLogs;
        ui->textDebugger->append("Starting Polar Animation...");
        for (int i = 0; i < polarLogs.size(); ++i)
            traceTags.append(extractTraceTag(polarLogs, i, 2));
    } else if (selectedCircleAlgorithm == 2) {
        animationPoints = cartesianPoints; animationLogs = cartesianLogs;
        ui->textDebugger->append("Starting Cartesian Animation...");
        for (int i = 0; i < cartesianLogs.size(); ++i)
            traceTags.append(extractTraceTag(cartesianLogs, i, 4));
    } else {
        animationPoints = midpointPoints; animationLogs = midpointLogs;
        ui->textDebugger->append("Starting Midpoint Animation...");
        for (int i = 0; i < midpointLogs.size(); ++i)
            traceTags.append(extractTraceTag(midpointLogs, i, 3));
    }
    
    animationStep = 0;
    syncPlaybackState();
    int interval = qRound(50 / animationSpeedMultiplier);
    if (interval < 1) interval = 1;
    animationTimer->start(interval);
    statusBar()->showMessage("Animating Circle Symmetry...");
}

void MainWindow::animateCircleStep()
{
    if (animationStep < 0 || animationStep >= animationPoints.size()) {
        animationTimer->stop();
        statusBar()->showMessage("Animation Complete.");
        syncPlaybackState();
        return;
    }
    if (animationStep < animationLogs.size()) {
        ui->textDebugger->append(animationLogs[animationStep]);
    }
    animationStep++;
    syncPlaybackState();
    drawgrid();
}

void MainWindow::on_sliderSpeed_valueChanged(int value)
{
    animationSpeedMultiplier = value / 10.0;
    ui->lblSpeed->setText(QString("Speed: %1x").arg(animationSpeedMultiplier, 0, 'f', 1));
    
    if (animationTimer->isActive()) {
        int interval = qRound(50 / animationSpeedMultiplier);
        if (interval < 1) interval = 1;
        animationTimer->setInterval(interval);
    }
    if (lineAnimationTimer->isActive()) {
        int interval = qRound(20 / animationSpeedMultiplier);
        if (interval < 1) interval = 1;
        lineAnimationTimer->setInterval(interval);
    }

    if (ellipseAnimationTimer->isActive()) {
        int interval = qRound(30 / animationSpeedMultiplier);
        if (interval < 1) interval = 1;
        ellipseAnimationTimer->setInterval(interval);
    }

}

void MainWindow::on_comboLineAlgo_currentIndexChanged(int index) {
    selectedalgorithm = index;
    lineAnimationStep = -1;
    if (haspoint1 && haspoint2) calculatealgorithms();
    drawgrid();
}
void MainWindow::on_comboLinePoint_currentIndexChanged(int) {}
void MainWindow::on_comboCircleAlgo_currentIndexChanged(int index) {
    selectedCircleAlgorithm = index;
    animationStep = -1;
    if (haspoint1 && haspoint2) calculateCircleAlgorithms();
    drawgrid();
}
void MainWindow::on_comboCirclePoint_currentIndexChanged(int) {}

void MainWindow::on_spinBoxRadius_valueChanged(int arg1) {
    if (haspoint1) {
        point2 = QPoint(point1.x() + arg1, point1.y());
        haspoint2 = true;
        linevisible = true;
        calculateCircleAlgorithms();
        
        ui->comboLinePoint->setItemText(1, "Point 2 : (" + QString::number(point2.x()) + ", " + QString::number(point2.y()) + ")");
        ui->comboCirclePoint->setItemText(1, "Radius Pt : (" + QString::number(point2.x()) + ", " + QString::number(point2.y()) + ")");
        drawgrid();
    }
}

void MainWindow::on_btnClearCircle_clicked() {
    haspoint1 = false;
    haspoint2 = false;
    linevisible = false;
    animationStep = -1;
    if (animationTimer->isActive()) animationTimer->stop();
    polarPoints.clear();
    midpointPoints.clear();
    cartesianPoints.clear();
    ui->comboCirclePoint->setItemText(0, "Select Center (Pt 1)");
    ui->comboCirclePoint->setItemText(1, "Select Radius (Pt 2)");
    ui->lblPolarTime->setText("Polar Time: -");
    ui->lblMidpointTime->setText("Midpoint Time: -");
    ui->lblCartesianTime->setText("Cartesian Time: -");
    ui->lblPolarCount->setText("Polar Pixels: -");
    ui->lblMidpointCount->setText("Midpoint Pixels: -");
    ui->lblCartesianCount->setText("Cartesian Pixels: -");
    ui->spinBoxRadius->blockSignals(true);
    ui->spinBoxRadius->setValue(0);
    ui->spinBoxRadius->blockSignals(false);
    drawgrid();
    statusBar()->showMessage("Circle cleared. Select Center.");
}

QVector<QPoint> MainWindow::calculateEllipsePolar(QPoint center, int rx, int ry, qint64 &time)
{
    Q_UNUSED(center);
    QElapsedTimer timer; timer.start();
    ellipsePolarLogs.clear();
    QVector<QPoint> points;
    if (rx == 0 && ry == 0) { points.append(QPoint(0,0)); time = timer.nsecsElapsed(); return points; }
    
    double max_r = std::max(rx, ry);
    double dtheta = 1.0 / max_r;
    for (double theta = 0; theta <= M_PI_2 + dtheta; theta += dtheta) {
        if (theta > M_PI_2) theta = M_PI_2; // cap at 90 deg
        int x = qRound(rx * qCos(theta));
        int y = qRound(ry * qSin(theta));
        points.append(QPoint(x, y));
        ellipsePolarLogs.append(QString("<span style='color:#7aa2f7'>Polar:</span> Theta=%1 rad | X=%2, Y=%3").arg(theta, 0, 'f', 3).arg(x).arg(y));
        if (theta == M_PI_2) break;
    }
    time = timer.nsecsElapsed();
    return points;
}

QVector<QPoint> MainWindow::calculateEllipseMidpoint(QPoint center, int rx, int ry, qint64 &time)
{
    Q_UNUSED(center);
    QElapsedTimer timer; timer.start();
    ellipseMidpointLogs.clear();
    QVector<QPoint> points;
    if (rx == 0 && ry == 0) { points.append(QPoint(0,0)); time = timer.nsecsElapsed(); return points; }

    long long rx2 = (long long)rx * rx;
    long long ry2 = (long long)ry * ry;
    long long tworx2 = 2 * rx2;
    long long twory2 = 2 * ry2;
    long long x = 0;
    long long y = ry;
    long long px = 0;
    long long py = tworx2 * y;
    
    // Region 1
    long long p1 = ry2 - (rx2 * ry) + (rx2 / 4);
    while (px < py) {
        points.append(QPoint(x, y));
        ellipseMidpointLogs.append(QString("<span style='color:#bb9af7'>[Region 1]</span> X=%1, Y=%2 | P1=%3").arg(x).arg(y).arg(p1));
        x++;
        px += twory2;
        if (p1 < 0) {
            p1 += ry2 + px;
        } else {
            y--;
            py -= tworx2;
            p1 += ry2 + px - py;
        }
    }
    
    // Region 2
    long long p2 = ry2 * (x * x + x) + ry2 / 4 + rx2 * (y - 1) * (y - 1) - rx2 * ry2;
    while (y >= 0) {
        points.append(QPoint(x, y));
        ellipseMidpointLogs.append(QString("<span style='color:#e0af68'>[Region 2]</span> X=%1, Y=%2 | P2=%3").arg(x).arg(y).arg(p2));
        y--;
        py -= tworx2;
        if (p2 > 0) {
            p2 += rx2 - py;
        } else {
            x++;
            px += twory2;
            p2 += rx2 - py + px;
        }
    }
    time = timer.nsecsElapsed();
    return points;
}

QVector<QPoint> MainWindow::calculateEllipseCartesian(QPoint center, int rx, int ry, qint64 &time)
{
    Q_UNUSED(center);
    QElapsedTimer timer; timer.start();
    ellipseCartesianLogs.clear();
    QVector<QPoint> points;
    if (rx == 0 || ry == 0) { points.append(QPoint(0,0)); time = timer.nsecsElapsed(); return points; }
    
    // Evaluate y = ry * sqrt(1 - x^2 / rx^2)
    for (int x = 0; x <= rx; ++x) {
        double inner = 1.0 - (double)(x * x) / (rx * rx);
        if (inner < 0) inner = 0;
        int y = qRound(ry * qSqrt(inner));
        points.append(QPoint(x, y));
        ellipseCartesianLogs.append(QString("Cartesian: X=%1 | Evaluated Y=%2").arg(x).arg(y));
    }
    // Note: Cartesian often has gaps where slope > 1, but we plot purely to show this artifact!
    time = timer.nsecsElapsed();
    return points;
}

void MainWindow::calculateEllipseAlgorithms()
{
    if (!hasEllipseCenter || !hasEllipseRx || !hasEllipseRy) return;
    
    ellipsePolarPoints = calculateEllipsePolar(ellipseCenter, ellipseRx, ellipseRy, ellipsePolarTime);
    ellipseMidpointPoints = calculateEllipseMidpoint(ellipseCenter, ellipseRx, ellipseRy, ellipseMidpointTime);
    ellipseCartesianPoints = calculateEllipseCartesian(ellipseCenter, ellipseRx, ellipseRy, ellipseCartesianTime);

    QSet<QPoint> polarSet(ellipsePolarPoints.begin(), ellipsePolarPoints.end());
    QSet<QPoint> midSet(ellipseMidpointPoints.begin(), ellipseMidpointPoints.end());
    QSet<QPoint> cartSet(ellipseCartesianPoints.begin(), ellipseCartesianPoints.end());

    ui->lblEllipsePolarTime->setText("Polar Time: " + formattime(ellipsePolarTime));
    ui->lblEllipseMidpointTime->setText("Midpoint Time: " + formattime(ellipseMidpointTime));
    ui->lblEllipseCartesianTime->setText("Cartesian Time: " + formattime(ellipseCartesianTime));
    
    ui->lblEllipsePolarCount->setText("Polar Pixels (1 Quad): " + QString::number(polarSet.size()));
    ui->lblEllipseMidpointCount->setText("Midpoint Pixels (1 Quad): " + QString::number(midSet.size()));
    ui->lblEllipseCartesianCount->setText("Cartesian Pixels (1 Quad): " + QString::number(cartSet.size()));

    if (ellipseMidpointTime > 0) {
        double ratio = (double)ellipsePolarTime / (double)ellipseMidpointTime;
        ui->lblEllipseCompare->setText(QString("Efficiency: Bresenham %1x Faster!").arg(ratio, 0, 'f', 2));
    } else {
        ui->lblEllipseCompare->setText("Efficiency: N/A");
    }
    
    // Ramanujan's Approximation & Area
    double a = ellipseRx;
    double b = ellipseRy;
    double area = M_PI * a * b;
    double h = ((a - b) * (a - b)) / ((a + b) * (a + b));
    double perimeter = M_PI * (a + b) * (1 + (3 * h) / (10 + qSqrt(4 - 3 * h)));
    
    ui->lblEllipseArea->setText(QString("Area: %1 px²").arg(area, 0, 'f', 1));
    ui->lblEllipsePerimeter->setText(QString("Perimeter (Ramanujan): %1 px").arg(perimeter, 0, 'f', 1));

    // ---- ELLIPSE ACCURACY REPORT ----
    if (ellipseRx > 0 && ellipseRy > 0) {
        auto ellipseError = [this](const QSet<QPoint> &pts) -> QPair<double,double> {
            double avgErr = 0, maxErr = 0;
            double rx2 = (double)ellipseRx * ellipseRx;
            double ry2 = (double)ellipseRy * ellipseRy;
            for (const QPoint &p : pts) {
                double val = ((double)p.x() * p.x()) / rx2 + ((double)p.y() * p.y()) / ry2;
                double err = qAbs(val - 1.0);
                avgErr += err;
                if (err > maxErr) maxErr = err;
            }
            avgErr /= qMax(1, pts.size());
            return {avgErr, maxErr};
        };

        auto [polarAvg, polarMax] = ellipseError(polarSet);
        auto [midAvg, midMax] = ellipseError(midSet);
        auto [cartAvg, cartMax] = ellipseError(cartSet);

        ui->textDebugger->append(QString("<span style='color:#89b4fa'>[Ellipse Accuracy]</span> "
            "rx=%1 ry=%2 | Polar: avg=%3 max=%4 | Mid: avg=%5 max=%6 | Cart: avg=%7 max=%8")
            .arg(ellipseRx).arg(ellipseRy)
            .arg(polarAvg, 0, 'f', 4).arg(polarMax, 0, 'f', 4)
            .arg(midAvg, 0, 'f', 4).arg(midMax, 0, 'f', 4)
            .arg(cartAvg, 0, 'f', 4).arg(cartMax, 0, 'f', 4));
    }
}

void MainWindow::removeShapeFromBuffer(const QVector<QPoint> &points, const QColor &color) {
    for (const QPoint &p : points) {
        pixelBuffer[p].removeAll(color);
        if (pixelBuffer[p].isEmpty())
            pixelBuffer.remove(p);
    }
}

void MainWindow::drawEllipseSymmetry(QPainter &painter, const QVector<QPoint> &points, const QColor &color, QPoint center, int rx, int ry, int rotation, int thickness, bool addToBuffer, bool regionHighlight)
{
    int xc = center.x();
    int yc = center.y();
    long long rx2 = (long long)rx * rx;
    long long ry2 = (long long)ry * ry;
    
    double rad = rotation * M_PI / 180.0;
    double cos_a = qCos(rad);
    double sin_a = qSin(rad);

    for (const QPoint &p : points) {
        int x = p.x();
        int y = p.y();
        
        QColor drawCol = color;
        if (!regionHighlight && !ellipseQuadColors) {
            double angle = qAtan2(y, x);
            int hueOffset = qRound(angle * 180 / M_PI);
            drawCol = QColor::fromHsv((color.hue() + hueOffset) % 360, color.saturation(), color.value());
        }
        if (regionHighlight) {
            if (2 * ry2 * x < 2 * rx2 * y) drawCol = QColor(187, 154, 247);
            else drawCol = QColor(224, 175, 104);
        }
        
        QVector<QPoint> rel_points = { QPoint(x, y), QPoint(-x, y), QPoint(-x, -y), QPoint(x, -y) };
        QVector<QColor> base_cols = { drawCol, drawCol, drawCol, drawCol };
        if (ellipseQuadColors) {
            base_cols = { QColor(122, 162, 247), QColor(187, 154, 247), QColor(247, 118, 142), QColor(115, 218, 202) };
        }
        
        for (int i=0; i<4; i++) {
            QPoint rp = rel_points[i];
            int rot_x = qRound(rp.x() * cos_a - rp.y() * sin_a);
            int rot_y = qRound(rp.x() * sin_a + rp.y() * cos_a);
            QPoint final_p = QPoint(xc + rot_x, yc + rot_y);
            
            // Draw thickness
            int half_t = thickness / 2;
            for (int dx = -half_t; dx <= half_t; dx++) {
                for (int dy = -half_t; dy <= half_t; dy++) {
                    QPoint thick_p = QPoint(final_p.x() + dx, final_p.y() + dy);
                    if (addToBuffer) {
                        if (!pixelBuffer[thick_p].contains(base_cols[i])) pixelBuffer[thick_p].append(base_cols[i]);
                    } else {
                        drawpoint(painter, thick_p, base_cols[i]);
                    }
                }
            }
        }
    }
}

void MainWindow::on_btnDrawEllipse_clicked() {
    if (!hasEllipseCenter) {
        ellipseCenter = QPoint(0, 0); hasEllipseCenter = true;
        ui->comboEllipsePoint->setItemText(0, "Center : (0, 0)");
    }
    if (!hasEllipseRx) {
        ellipseRx = ui->spinBoxRx->value(); if (ellipseRx == 0) ellipseRx = 5;
        hasEllipseRx = true; ui->comboEllipsePoint->setItemText(1, "Radius X : " + QString::number(ellipseRx));
        ui->spinBoxRx->setValue(ellipseRx);
    }
    if (!hasEllipseRy) {
        ellipseRy = ui->spinBoxRy->value(); if (ellipseRy == 0) ellipseRy = 5;
        hasEllipseRy = true; ui->comboEllipsePoint->setItemText(2, "Radius Y : " + QString::number(ellipseRy));
        ui->spinBoxRy->setValue(ellipseRy);
    }
    calculateEllipseAlgorithms();
    ellipseVisible = true;
    ellipseAnimationStep = -1;
    if (ellipseAnimationTimer->isActive()) ellipseAnimationTimer->stop();
    drawgrid();
    statusBar()->showMessage("Ellipse drawn instantly.");
}

void MainWindow::on_btnAnimateEllipse_clicked() {
    if (!hasEllipseCenter) {
        ellipseCenter = QPoint(0, 0); hasEllipseCenter = true;
        ui->comboEllipsePoint->setItemText(0, "Center : (0, 0)");
    }
    if (!hasEllipseRx) {
        ellipseRx = ui->spinBoxRx->value(); if (ellipseRx == 0) ellipseRx = 5;
        hasEllipseRx = true; ui->comboEllipsePoint->setItemText(1, "Radius X : " + QString::number(ellipseRx));
        ui->spinBoxRx->setValue(ellipseRx);
    }
    if (!hasEllipseRy) {
        ellipseRy = ui->spinBoxRy->value(); if (ellipseRy == 0) ellipseRy = 5;
        hasEllipseRy = true; ui->comboEllipsePoint->setItemText(2, "Radius Y : " + QString::number(ellipseRy));
        ui->spinBoxRy->setValue(ellipseRy);
    }
    calculateEllipseAlgorithms();
    ellipseVisible = true;
    ui->textDebugger->clear();
    traceTags.clear();
    
    if (selectedEllipseAlgorithm == 0) {
        ellipseAnimationPoints = ellipsePolarPoints; ellipseAnimationLogs = ellipsePolarLogs;
        ui->textDebugger->append("Starting Polar Ellipse...");
        for (int i = 0; i < ellipsePolarLogs.size(); ++i)
            traceTags.append(extractTraceTag(ellipsePolarLogs, i, 2));
    } else if (selectedEllipseAlgorithm == 2) {
        ellipseAnimationPoints = ellipseCartesianPoints; ellipseAnimationLogs = ellipseCartesianLogs;
        ui->textDebugger->append("Starting Cartesian Ellipse...");
        for (int i = 0; i < ellipseCartesianLogs.size(); ++i)
            traceTags.append(extractTraceTag(ellipseCartesianLogs, i, 4));
    } else {
        ellipseAnimationPoints = ellipseMidpointPoints; ellipseAnimationLogs = ellipseMidpointLogs;
        ui->textDebugger->append("Starting Midpoint (Bresenham) Ellipse...");
        for (int i = 0; i < ellipseMidpointLogs.size(); ++i)
            traceTags.append(extractTraceTag(ellipseMidpointLogs, i, 3));
    }
    
    ellipseAnimationStep = 0;
    syncPlaybackState();
    int interval = qRound(30 / animationSpeedMultiplier);
    if (interval < 1) interval = 1;
    ellipseAnimationTimer->start(interval);
    statusBar()->showMessage("Animating Ellipse 4-Way Symmetry...");
}

void MainWindow::animateEllipseStep() {
    if (ellipseAnimationStep < 0 || ellipseAnimationStep >= ellipseAnimationPoints.size()) {
        ellipseAnimationTimer->stop(); statusBar()->showMessage("Animation Complete.");
        syncPlaybackState(); return;
    }
    if (ellipseAnimationStep < ellipseAnimationLogs.size()) {
        ui->textDebugger->append(ellipseAnimationLogs[ellipseAnimationStep]);
    }
    
    // Draw the glowing head explicitly directly on the painter bypassing drawgrid for immediate flash
    if (ellipseAnimationStep < ellipseAnimationPoints.size()) {
        QPoint p = ellipseAnimationPoints[ellipseAnimationStep];
        QPixmap pix = ui->frame->pixmap();
        QPainter painter(&pix);
        drawEllipseSymmetry(painter, {p}, QColor(255, 255, 255), ellipseCenter, ellipseRx, ellipseRy, ellipseRotation, ellipseThickness, false, false);
        ui->frame->setPixmap(pix);
        ui->frame->update();
    }
    
    ellipseAnimationStep++;
    syncPlaybackState();
    drawgrid();
}

void MainWindow::on_btnClearEllipse_clicked() {
    hasEllipseCenter = false; hasEllipseRx = false; hasEllipseRy = false; ellipseVisible = false;
    ellipseAnimationStep = -1; if (ellipseAnimationTimer->isActive()) ellipseAnimationTimer->stop();
    ellipsePolarPoints.clear(); ellipseMidpointPoints.clear(); ellipseCartesianPoints.clear();
    ui->comboEllipsePoint->setItemText(0, "Select Center"); ui->comboEllipsePoint->setItemText(1, "Select Radius X"); ui->comboEllipsePoint->setItemText(2, "Select Radius Y");
    ui->lblEllipsePolarTime->setText("Polar Time: -"); ui->lblEllipseMidpointTime->setText("Midpoint Time: -"); ui->lblEllipseCartesianTime->setText("Cartesian Time: -");
    ui->lblEllipsePolarCount->setText("Polar Pixels: -"); ui->lblEllipseMidpointCount->setText("Midpoint Pixels: -"); ui->lblEllipseCartesianCount->setText("Cartesian Pixels: -");
    ui->lblEllipseCompare->setText("Efficiency: -");
    ui->spinBoxRx->blockSignals(true); ui->spinBoxRx->setValue(0); ui->spinBoxRx->blockSignals(false);
    ui->spinBoxRy->blockSignals(true); ui->spinBoxRy->setValue(0); ui->spinBoxRy->blockSignals(false);
    drawgrid(); statusBar()->showMessage("Ellipse cleared.");
}

void MainWindow::on_comboEllipseAlgo_currentIndexChanged(int index) {
    selectedEllipseAlgorithm = index; ellipseAnimationStep = -1;
    if (hasEllipseCenter && hasEllipseRx && hasEllipseRy) calculateEllipseAlgorithms();
    drawgrid();
}

void MainWindow::on_comboEllipsePoint_currentIndexChanged(int index) { Q_UNUSED(index); }

void MainWindow::on_spinBoxRx_valueChanged(int arg1) {
    if (hasEllipseCenter) {
        ellipseRx = arg1; hasEllipseRx = true;
        ui->comboEllipsePoint->setItemText(1, "Radius X : " + QString::number(ellipseRx));
        if (hasEllipseRy) { ellipseVisible = true; calculateEllipseAlgorithms(); }
        drawgrid();
    }
}

void MainWindow::on_spinBoxRy_valueChanged(int arg1) {
    if (hasEllipseCenter) {
        ellipseRy = arg1; hasEllipseRy = true;
        ui->comboEllipsePoint->setItemText(2, "Radius Y : " + QString::number(ellipseRy));
        if (hasEllipseRx) { ellipseVisible = true; calculateEllipseAlgorithms(); }
        drawgrid();
    }
}

void MainWindow::on_chkQuadColors_stateChanged(int arg1) { ellipseQuadColors = (arg1 == Qt::Checked); drawgrid(); }
void MainWindow::on_chkRegionSplit_stateChanged(int arg1) { ellipseRegionSplit = (arg1 == Qt::Checked); drawgrid(); }

void MainWindow::on_sliderRotation_valueChanged(int value) {
    ellipseRotation = value;
    if (hasEllipseCenter && hasEllipseRx && hasEllipseRy) calculateEllipseAlgorithms();
    drawgrid();
}

void MainWindow::on_sliderThickness_valueChanged(int value) {
    ellipseThickness = value;
    drawgrid();
}

void MainWindow::on_btnClearCanvas_clicked() {
    persistentEllipses.clear();
    drawgrid();
}

void MainWindow::on_btnCommitEllipse_clicked() {
    if (!hasEllipseCenter || !hasEllipseRx || !hasEllipseRy) return;
    PersistentEllipse pe;
    pe.center = ellipseCenter;
    pe.rx = ellipseRx;
    pe.ry = ellipseRy;
    pe.rotation = ellipseRotation;
    pe.thickness = ellipseThickness;
    pe.algorithm = selectedEllipseAlgorithm;
    if (selectedEllipseAlgorithm == 0) pe.color = QColor(255, 0, 127, 100);
    else if (selectedEllipseAlgorithm == 1) pe.color = QColor(0, 245, 212, 100);
    else if (selectedEllipseAlgorithm == 2) pe.color = QColor(56, 189, 248, 100);
    else pe.color = QColor(200, 200, 200, 80);
    persistentEllipses.append(pe);
    
    hasEllipseCenter = false; hasEllipseRx = false; hasEllipseRy = false;
    ellipseVisible = false;
    ui->comboEllipsePoint->setItemText(0, "Select Center");
    ui->comboEllipsePoint->setItemText(1, "Select Radius X");
    ui->comboEllipsePoint->setItemText(2, "Select Radius Y");
    ui->spinBoxRx->setValue(0);
    ui->spinBoxRy->setValue(0);
    statusBar()->showMessage(QString("Ellipse saved! Canvas now has %1 ellipses.").arg(persistentEllipses.size()));
    drawgrid();
}

void MainWindow::selectTool(int toolIndex) {
    if (toolIndex >= 6) return;
    QStringList names = {"Line", "Circle", "Ellipse", "Polygon", "Transform", "Curve"};
    static const ActiveTool kToolByIndex[6] = {
        TOOL_LINE, TOOL_CIRCLE, TOOL_ELLIPSE,
        TOOL_POLYGON, TOOL_TRANSFORM, TOOL_CURVE
    };
    for(int i=0; i<sidebarButtons.size(); ++i) {
        sidebarButtons[i]->setChecked(i == toolIndex);
    }
    currentTool = kToolByIndex[toolIndex];
    settingsStack->setCurrentIndex(toolIndex);
    if (toolNameLabel) toolNameLabel->setText(names[toolIndex]);
    if (comboPolygonMode) {
        comboPolygonMode->blockSignals(true);
        comboPolygonMode->setCurrentIndex(0);
        comboPolygonMode->blockSignals(false);
    }
    drawgrid();
}

void MainWindow::handleToggleNavbar() {
    if (settingsStack->isVisible()) {
        settingsStack->setVisible(false);
        btnToggleNavbar->setText(">");
    } else {
        settingsStack->setVisible(true);
        btnToggleNavbar->setText("<");
    }
}

void MainWindow::handleClearCanvasClicked() {
    pixelBuffer.clear();
    persistentEllipses.clear();
    activePolygonPoints.clear();
    lastClosedPolygonVertices.clear();
    committedPolygonPixels.clear();
    haspoint1 = false;
    haspoint2 = false;
    linevisible = false;
    hasEllipseCenter = false;
    hasEllipseRx = false;
    hasEllipseRy = false;
    ellipseVisible = false;
    ellipseCenter = QPoint(0, 0);
    ellipseRx = 0;
    ellipseRy = 0;
    ellipseDraggingPoint = 0;
    polygonClosed = false;
    animationStep = -1;
    lineAnimationStep = -1;
    ellipseAnimationStep = -1;
    draggingpoint = 0;
    ddapoints.clear();
    bresenhampoints.clear();
    polarPoints.clear();
    midpointPoints.clear();
    cartesianPoints.clear();
    ellipsePolarPoints.clear();
    ellipseMidpointPoints.clear();
    ellipseCartesianPoints.clear();
    lineAnimationPoints.clear();
    lineAnimationLogs.clear();
    animationPoints.clear();
    animationLogs.clear();
    ellipseAnimationPoints.clear();
    ellipseAnimationLogs.clear();
    traceTags.clear();
    if (animationTimer->isActive()) animationTimer->stop();
    if (lineAnimationTimer->isActive()) lineAnimationTimer->stop();
    if (ellipseAnimationTimer->isActive()) ellipseAnimationTimer->stop();
    bezierControlPoints.clear();
    bezierCurvePoints.clear();
    bezierAnimPoints.clear();
    bezierLogs.clear();
    bezierTraceTags.clear();
    bezierAnimStep = -1;
    if (bezierAnimTimer->isActive()) bezierAnimTimer->stop();
    scene.clear();
    selectedShapeIndex = -1;
    undoStack.clear();
    redoStack.clear();
    hasArbLine = false;
    hasArbPoint = false;
    drawgrid();
}

void MainWindow::handlePolygonClearClicked() {
    activePolygonPoints.clear();
    lastClosedPolygonVertices.clear();
    for (const QPoint &p : committedPolygonPixels) {
        pixelBuffer[p].removeAll(polygonFillColor);
        if (pixelBuffer[p].isEmpty())
            pixelBuffer.remove(p);
    }
    committedPolygonPixels.clear();
    polygonClosed = false;
    drawgrid();
}

void MainWindow::handlePolygonCloseClicked() {
    if(activePolygonPoints.size() >= 3) {
        polygonClosed = true;
        committedPolygonPixels.clear();

        QVector<QPoint> savedVertices = activePolygonPoints;
        lastClosedPolygonVertices = activePolygonPoints;

        qint64 dummyTime = 0;
        for (int i = 0; i < activePolygonPoints.size(); ++i) {
            QPoint p1 = activePolygonPoints[i];
            QPoint p2 = activePolygonPoints[(i + 1) % activePolygonPoints.size()];
            QVector<QPoint> edge = calculatedda(p1, p2, dummyTime);
            for(const QPoint& p : edge) {
                if (!pixelBuffer[p].contains(polygonFillColor)) {
                    pixelBuffer[p].append(polygonFillColor);
                    committedPolygonPixels.append(p);
                }
            }
        }

        if (currentTool == TOOL_SCANLINE_FILL) {
            scanlineFillPolygon(savedVertices, currentFillColor, &committedPolygonPixels);
        }
        Shape s; s.type = Shape::POLYGON; s.color = polygonFillColor;
        s.polygonVertices = savedVertices;
        s.selected = false;
        addShapeToScene(s);

        activePolygonPoints.clear();
        polygonClosed = false;
        drawgrid();
    }
}

void MainWindow::handleFillColorClicked() {
    QColor color = QColorDialog::getColor(currentFillColor, this, "Select Fill Color");
    if(color.isValid()) {
        currentFillColor = color;
    }
}

void MainWindow::handleBoundaryColorClicked() {
    QColor color = QColorDialog::getColor(currentBoundaryColor, this, "Select Boundary Color");
    if(color.isValid()) {
        currentBoundaryColor = color;
    }
}

void MainWindow::floodFill(const QPoint &startNode, const QColor &targetColor, const QColor &replacementColor)
{
    if (targetColor == replacementColor) return;
    bool targetIsBackground = !targetColor.isValid();

    bool startMatches = false;
    if (targetIsBackground) {
        startMatches = !pixelBuffer.contains(startNode);
    } else {
        startMatches = pixelBuffer.contains(startNode) && pixelBuffer[startNode].contains(targetColor);
    }
    if (!startMatches) return;
    if (startNode.x() == 0 || startNode.y() == 0) return;

    int halfW = ui->frame->width() / 2;
    int halfH = ui->frame->height() / 2;

    const int maxFillCells = 50000;

    QQueue<QPoint> queue;
    queue.enqueue(startNode);
    QSet<QPoint> visited;
    visited.insert(startNode);

    QPoint dirs4[] = { {1,0},{-1,0},{0,1},{0,-1} };
    QPoint dirs8[] = { {1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1} };
    QPoint *dirs = (fillConnectivity == 8) ? dirs8 : dirs4;
    int dirCount = (fillConnectivity == 8) ? 8 : 4;

    while (!queue.isEmpty()) {
        if (visited.size() >= maxFillCells) break;
        QPoint p = queue.dequeue();

        bool matches = false;
        if (targetIsBackground) {
            matches = !pixelBuffer.contains(p);
        } else {
            matches = pixelBuffer.contains(p) && pixelBuffer[p].contains(targetColor);
        }

        if (matches) {
            pixelBuffer[p].clear();
            pixelBuffer[p].append(replacementColor);

            for (int i = 0; i < dirCount; ++i) {
                QPoint np(p.x() + dirs[i].x(), p.y() + dirs[i].y());
                if (np.x() == 0 || np.y() == 0) continue;
                if (!visited.contains(np) && qAbs(np.x()) <= halfW && qAbs(np.y()) <= halfH) {
                    if (dirs[i].x() != 0 && dirs[i].y() != 0) {
                        QPoint orthoX(p.x() + dirs[i].x(), p.y());
                        QPoint orthoY(p.x(), p.y() + dirs[i].y());
                        bool orthoXOk, orthoYOk;
                        if (targetIsBackground) {
                            orthoXOk = !pixelBuffer.contains(orthoX);
                            orthoYOk = !pixelBuffer.contains(orthoY);
                        } else {
                            orthoXOk = pixelBuffer.contains(orthoX) && pixelBuffer[orthoX].contains(targetColor);
                            orthoYOk = pixelBuffer.contains(orthoY) && pixelBuffer[orthoY].contains(targetColor);
                        }
                        if (!orthoXOk || !orthoYOk) continue;
                    }
                    visited.insert(np);
                    queue.enqueue(np);
                }
            }
        }
    }
}

void MainWindow::boundaryFill(const QPoint &startNode, const QColor &fillColor, const QColor &)
{
    int halfW = ui->frame->width() / 2;
    int halfH = ui->frame->height() / 2;

    if (pixelBuffer.contains(startNode)) return;

    const int maxFillCells = 50000;

    QQueue<QPoint> queue;
    queue.enqueue(startNode);
    QSet<QPoint> visited;
    visited.insert(startNode);

    QPoint dirs4[] = { {1,0},{-1,0},{0,1},{0,-1} };
    QPoint dirs8[] = { {1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1} };
    QPoint *dirs = (fillConnectivity == 8) ? dirs8 : dirs4;
    int dirCount = (fillConnectivity == 8) ? 8 : 4;

    while (!queue.isEmpty()) {
        if (visited.size() >= maxFillCells) break;
        QPoint p = queue.dequeue();

        if (pixelBuffer.contains(p)) continue;

        if (!pixelBuffer[p].contains(fillColor))
            pixelBuffer[p].append(fillColor);

        for (int i = 0; i < dirCount; ++i) {
            QPoint np(p.x() + dirs[i].x(), p.y() + dirs[i].y());
            if (!visited.contains(np) && qAbs(np.x()) <= halfW && qAbs(np.y()) <= halfH) {
                if (dirs[i].x() != 0 && dirs[i].y() != 0) {
                    QPoint orthoX(p.x() + dirs[i].x(), p.y());
                    QPoint orthoY(p.x(), p.y() + dirs[i].y());
                    if (pixelBuffer.contains(orthoX) || pixelBuffer.contains(orthoY)) continue;
                }
                visited.insert(np);
                queue.enqueue(np);
            }
        }
    }
}

void MainWindow::scanlineFillPolygon(const QVector<QPoint> &vertices, const QColor &fillColor, QVector<QPoint> *committed)
{
    if (vertices.size() < 3) return;
    
    int minY = vertices[0].y();
    int maxY = vertices[0].y();
    for (const QPoint& p : vertices) {
        if (p.y() < minY) minY = p.y();
        if (p.y() > maxY) maxY = p.y();
    }
    
    for (int y = minY; y <= maxY; ++y) {
        QVector<int> intersections;
        for (int i = 0; i < vertices.size(); ++i) {
            QPoint p1 = vertices[i];
            QPoint p2 = vertices[(i + 1) % vertices.size()];
            
            if (p1.y() == p2.y()) continue; 
            if ((y >= p1.y() && y < p2.y()) || (y >= p2.y() && y < p1.y())) {
                double x = p1.x() + (double)(y - p1.y()) * (p2.x() - p1.x()) / (p2.y() - p1.y());
                intersections.append(qRound(x));
            }
        }
        
        std::sort(intersections.begin(), intersections.end());
        for (int i = 0; i < intersections.size() - 1; i += 2) {
            int x0 = intersections[i];
            int x1 = intersections[i+1];
            for (int x = x0; x <= x1; ++x) {
                if (!pixelBuffer[QPoint(x, y)].contains(fillColor)) {
                    pixelBuffer[QPoint(x, y)].append(fillColor);
                    if (committed) committed->append(QPoint(x, y));
                }
            }
        }
    }
}

void MainWindow::drawPolygonEdges(QPainter &painter, const QVector<QPoint> &vertices, const QColor &color)
{
    qint64 dummyTime = 0;
    for (int i = 0; i < vertices.size(); ++i) {
        QPoint p1 = vertices[i];
        QPoint p2 = vertices[(i + 1) % vertices.size()];
        if (!polygonClosed && i == vertices.size() - 1) break; 
        QVector<QPoint> edge = calculatedda(p1, p2, dummyTime);
        for(const QPoint& p : edge) {
            drawpoint(painter, p, color);
        }
    }
}

void MainWindow::multiplyMatrix3x3(double m1[3][3], double m2[3][3], double result[3][3]) {
    double temp[3][3];
    for(int i=0; i<3; ++i) {
        for(int j=0; j<3; ++j) {
            temp[i][j] = 0;
            for(int k=0; k<3; ++k) {
                temp[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    for(int i=0; i<3; ++i) {
        for(int j=0; j<3; ++j) {
            result[i][j] = temp[i][j];
        }
    }
}

QPoint MainWindow::transformPoint(const QPoint& p, double T[3][3]) {
    double x = p.x() * T[0][0] + p.y() * T[0][1] + 1 * T[0][2];
    double y = p.x() * T[1][0] + p.y() * T[1][1] + 1 * T[1][2];
    double w = p.x() * T[2][0] + p.y() * T[2][1] + 1 * T[2][2];
    if (w != 0 && w != 1) {
        x /= w; y /= w;
    }
    return QPoint(qRound(x), qRound(y));
}

void MainWindow::applyTransformationMatrix(double T[3][3]) {
    if (lastClosedPolygonVertices.isEmpty()) return;
    pushUndo();

    // Always save the current polygon as a ghost before transforming
    {
        Shape ghost;
        ghost.type = Shape::POLYGON;
        QColor ghostColor(polygonFillColor.red() / 2 + 128,
                          polygonFillColor.green() / 2 + 128,
                          polygonFillColor.blue() / 2 + 128);
        ghost.color = ghostColor;
        ghost.selected = false;
        ghost.polygonVertices = lastClosedPolygonVertices;
        qint64 dt = 0;
        for (int i = 0; i < ghost.polygonVertices.size(); ++i) {
            QPoint p1 = ghost.polygonVertices[i];
            QPoint p2 = ghost.polygonVertices[(i + 1) % ghost.polygonVertices.size()];
            QVector<QPoint> edge = calculatedda(p1, p2, dt);
            for (const QPoint &ep : edge) {
                if (!pixelBuffer[ep].contains(ghostColor)) {
                    pixelBuffer[ep].append(ghostColor);
                }
                ghost.shapePixels[ep].append(ghostColor);
            }
        }
        scene.append(ghost);
    }

    // Transform points
    QVector<QPoint> newVertices;
    for (const QPoint& p : lastClosedPolygonVertices) {
        newVertices.append(transformPoint(p, T));
    }

    // Erase old polygon pixels from pixelBuffer
    for (const QPoint& p : committedPolygonPixels) {
        if (pixelBuffer.contains(p)) {
            pixelBuffer[p].removeAll(polygonFillColor);
            pixelBuffer[p].removeAll(currentFillColor);
            pixelBuffer[p].removeAll(currentBoundaryColor);
            if (pixelBuffer[p].isEmpty()) {
                pixelBuffer.remove(p);
            }
        }
    }
    committedPolygonPixels.clear();

    // Redraw new edges
    lastClosedPolygonVertices = newVertices;
    
    qint64 dummyTime = 0;
    for (int i = 0; i < newVertices.size(); ++i) {
        QPoint p1 = newVertices[i];
        QPoint p2 = newVertices[(i + 1) % newVertices.size()];
        QVector<QPoint> edge = calculatedda(p1, p2, dummyTime);
        for(const QPoint& ep : edge) {
            if (!pixelBuffer[ep].contains(polygonFillColor)) {
                pixelBuffer[ep].append(polygonFillColor);
            }
            committedPolygonPixels.append(ep);
        }
    }
    drawgrid();
}

void MainWindow::handleTransformTranslate() {
    QSpinBox* spinTx = this->findChild<QSpinBox*>("spinTx");
    QSpinBox* spinTy = this->findChild<QSpinBox*>("spinTy");
    if(!spinTx || !spinTy) return;
    double dx = spinTx->value();
    double dy = spinTy->value();
    
    double T[3][3] = {
        {1, 0, dx},
        {0, 1, dy},
        {0, 0, 1}
    };
    applyTransformationMatrix(T);
    if (dx != 0 || dy != 0) {
        statusBar()->showMessage(QString("Translated polygon by (%1, %2).").arg(dx).arg(dy));
        spinTx->setValue(0);
        spinTy->setValue(0);
    }
}

void MainWindow::handleTransformRotate() {
    QDoubleSpinBox* spinAngle = this->findChild<QDoubleSpinBox*>("spinAngle");
    if(!spinAngle) return;
    double rad = spinAngle->value() * M_PI / 180.0;
    
    double T[3][3] = {
        {cos(rad), -sin(rad), 0},
        {sin(rad),  cos(rad), 0},
        {0,         0,        1}
    };
    applyTransformationMatrix(T);
    if (spinAngle->value() != 0) {
        statusBar()->showMessage(QString("Rotated polygon by %1°.").arg(spinAngle->value()));
        spinAngle->setValue(0);
    }
}

void MainWindow::handleTransformScale() {
    QDoubleSpinBox* spinSx = this->findChild<QDoubleSpinBox*>("spinSx");
    QDoubleSpinBox* spinSy = this->findChild<QDoubleSpinBox*>("spinSy");
    if(!spinSx || !spinSy) return;
    
    double T[3][3] = {
        {spinSx->value(), 0, 0},
        {0, spinSy->value(), 0},
        {0, 0,               1}
    };
    applyTransformationMatrix(T);
    if (spinSx->value() != 1.0 || spinSy->value() != 1.0) {
        statusBar()->showMessage(QString("Scaled polygon by (x%1, y%2).")
                                 .arg(spinSx->value()).arg(spinSy->value()));
        spinSx->setValue(1.0);
        spinSy->setValue(1.0);
    }
}

void MainWindow::handleTransformShear() {
    QDoubleSpinBox* spinShx = this->findChild<QDoubleSpinBox*>("spinShx");
    QDoubleSpinBox* spinShy = this->findChild<QDoubleSpinBox*>("spinShy");
    if(!spinShx || !spinShy) return;
    
    double T[3][3] = {
        {1, spinShx->value(), 0},
        {spinShy->value(), 1, 0},
        {0, 0,                1}
    };
    applyTransformationMatrix(T);
    if (spinShx->value() != 0 || spinShy->value() != 0) {
        statusBar()->showMessage(QString("Sheared polygon by (x%1, y%2).")
                                 .arg(spinShx->value()).arg(spinShy->value()));
        spinShx->setValue(0);
        spinShy->setValue(0);
    }
}

void MainWindow::handleTransformReflectX() {
    double T[3][3] = {
        {1,  0, 0},
        {0, -1, 0},
        {0,  0, 1}
    };
    applyTransformationMatrix(T);
    statusBar()->showMessage("Reflected polygon about the X-axis.");
}

void MainWindow::handleTransformReflectY() {
    double T[3][3] = {
        {-1, 0, 0},
        {0,  1, 0},
        {0,  0, 1}
    };
    applyTransformationMatrix(T);
    statusBar()->showMessage("Reflected polygon about the Y-axis.");
}

void MainWindow::handleTransformReflectOrigin() {
    double T[3][3] = {
        {-1,  0, 0},
        { 0, -1, 0},
        { 0,  0, 1}
    };
    applyTransformationMatrix(T);
    statusBar()->showMessage("Reflected polygon about the origin.");
}

void MainWindow::handleTransformArbitraryLine() {
    QSpinBox* alX1 = this->findChild<QSpinBox*>("alX1");
    QSpinBox* alY1 = this->findChild<QSpinBox*>("alY1");
    QSpinBox* alX2 = this->findChild<QSpinBox*>("alX2");
    QSpinBox* alY2 = this->findChild<QSpinBox*>("alY2");
    if(!alX1 || !alY1 || !alX2 || !alY2) return;
    
    double x1 = alX1->value(), y1 = alY1->value();
    double x2 = alX2->value(), y2 = alY2->value();
    if (x1 == x2 && y1 == y2) {
        statusBar()->showMessage("Reflect: axis points are identical, pick two distinct points.");
        return;
    }
    
    double finalT[3][3];

    // Vertical line: x = x1  =>  x -> 2*x1 - x, y unchanged
    if (x1 == x2) {
        double k = x1;
        finalT[0][0] = -1;  finalT[0][1] = 0;  finalT[0][2] = 2 * k;
        finalT[1][0] =  0;  finalT[1][1] = 1;  finalT[1][2] = 0;
        finalT[2][0] =  0;  finalT[2][1] = 0;  finalT[2][2] = 1;
    } else {
        double m = (y2 - y1) / (x2 - x1);
        double c = y1 - m * x1;

        double T1[3][3] = { {1, 0, 0}, {0, 1, -c}, {0, 0, 1} };
        double theta = atan(m);
        double T2[3][3] = { {cos(-theta), -sin(-theta), 0}, {sin(-theta), cos(-theta), 0}, {0, 0, 1} };
        double T3[3][3] = { {1, 0, 0}, {0, -1, 0}, {0, 0, 1} };
        double T4[3][3] = { {cos(theta), -sin(theta), 0}, {sin(theta), cos(theta), 0}, {0, 0, 1} };
        double T5[3][3] = { {1, 0, 0}, {0, 1, c}, {0, 0, 1} };

        multiplyMatrix3x3(T2, T1, finalT);
        multiplyMatrix3x3(T3, finalT, finalT);
        multiplyMatrix3x3(T4, finalT, finalT);
        multiplyMatrix3x3(T5, finalT, finalT);
    }
    
    applyTransformationMatrix(finalT);
    hasArbLine = true;
    arbLineX1 = x1; arbLineY1 = y1;
    arbLineX2 = x2; arbLineY2 = y2;
    statusBar()->showMessage(QString("Reflected polygon about line (%1,%2)-(%3,%4).")
                             .arg(x1).arg(y1).arg(x2).arg(y2));
    alX1->setValue(0); alY1->setValue(0); alX2->setValue(10); alY2->setValue(10);
}

void MainWindow::handleTransformArbitraryPoint() {
    QSpinBox* apX = this->findChild<QSpinBox*>("apX");
    QSpinBox* apY = this->findChild<QSpinBox*>("apY");
    QDoubleSpinBox* apA = this->findChild<QDoubleSpinBox*>("apA");
    if(!apX || !apY || !apA) return;
    
    double px = apX->value();
    double py = apY->value();
    double angle = apA->value();
    double rad = angle * M_PI / 180.0;
    
    double T1[3][3] = { {1, 0, -px}, {0, 1, -py}, {0, 0, 1} };
    double T2[3][3] = { {cos(rad), -sin(rad), 0}, {sin(rad), cos(rad), 0}, {0, 0, 1} };
    double T3[3][3] = { {1, 0, px}, {0, 1, py}, {0, 0, 1} };
    
    double finalT[3][3] = { {1,0,0}, {0,1,0}, {0,0,1} };
    multiplyMatrix3x3(T2, T1, finalT);
    multiplyMatrix3x3(T3, finalT, finalT);
    
    applyTransformationMatrix(finalT);
    if (angle != 0) {
        hasArbPoint = true;
        arbPointX = px; arbPointY = py;
        statusBar()->showMessage(QString("Rotated polygon by %1° about point (%2, %3).")
                                 .arg(angle).arg(px).arg(py));
        apA->setValue(0);
    } else {
        statusBar()->showMessage("Enter a nonzero angle to rotate about the chosen point.");
    }
}

// ---- PLAYBACK HELPERS ----

int MainWindow::getActiveAnimationStep() const {
    if (currentTool == TOOL_LINE) return lineAnimationStep;
    if (currentTool == TOOL_CIRCLE) return animationStep;
    if (currentTool == TOOL_ELLIPSE) return ellipseAnimationStep;
    if (currentTool == TOOL_CURVE) return bezierAnimStep;
    return -1;
}

void MainWindow::setActiveAnimationStep(int step) {
    if (currentTool == TOOL_LINE) lineAnimationStep = step;
    else if (currentTool == TOOL_CIRCLE) animationStep = step;
    else if (currentTool == TOOL_ELLIPSE) ellipseAnimationStep = step;
    else if (currentTool == TOOL_CURVE) bezierAnimStep = step;
}

int MainWindow::getActiveAnimationPointCount() const {
    if (currentTool == TOOL_LINE) return lineAnimationPoints.size();
    if (currentTool == TOOL_CIRCLE) return animationPoints.size();
    if (currentTool == TOOL_ELLIPSE) return ellipseAnimationPoints.size();
    if (currentTool == TOOL_CURVE) return bezierAnimPoints.size();
    return 0;
}

QVector<QPoint> MainWindow::getActiveAnimationPoints() const {
    if (currentTool == TOOL_LINE) return lineAnimationPoints;
    if (currentTool == TOOL_CIRCLE) return animationPoints;
    if (currentTool == TOOL_ELLIPSE) return ellipseAnimationPoints;
    if (currentTool == TOOL_CURVE) return bezierAnimPoints;
    return {};
}

void MainWindow::syncPlaybackState() {
    QSlider* slider = findChild<QSlider*>("scrubSlider");
    QLabel* info = findChild<QLabel*>("lblStepInfo");
    if (!slider || !info) return;

    int total = getActiveAnimationPointCount();
    int current = getActiveAnimationStep();

    if (total > 0 && current >= 0) {
        slider->blockSignals(true);
        slider->setRange(0, total - 1);
        slider->setValue(current < total ? current : total - 1);
        slider->blockSignals(false);
        info->setText(QString("%1 / %2").arg(current + 1).arg(total));
    } else {
        slider->blockSignals(true);
        slider->setRange(0, 0);
        slider->setValue(0);
        slider->blockSignals(false);
        info->setText("0 / 0");
    }
}

void MainWindow::onStepBackClicked() {
    int step = getActiveAnimationStep();
    if (step <= 0) return;

    // Stop any running timer so manual stepping doesn't conflict
    if (lineAnimationTimer->isActive()) lineAnimationTimer->stop();
    if (animationTimer->isActive()) animationTimer->stop();
    if (ellipseAnimationTimer->isActive()) ellipseAnimationTimer->stop();
    if (bezierAnimTimer->isActive()) bezierAnimTimer->stop();

    setActiveAnimationStep(step - 1);
    syncPlaybackState();
    drawgrid();
}

void MainWindow::onStepForwardClicked() {
    int step = getActiveAnimationStep();
    int total = getActiveAnimationPointCount();
    if (step < 0 || step >= total) return;

    // Stop any running timer
    if (lineAnimationTimer->isActive()) lineAnimationTimer->stop();
    if (animationTimer->isActive()) animationTimer->stop();
    if (ellipseAnimationTimer->isActive()) ellipseAnimationTimer->stop();
    if (bezierAnimTimer->isActive()) bezierAnimTimer->stop();

    setActiveAnimationStep(step + 1);
    syncPlaybackState();
    drawgrid();
}

void MainWindow::onScrubSliderChanged(int value) {
    int total = getActiveAnimationPointCount();
    if (total == 0) return;

    // Stop any running timer
    if (lineAnimationTimer->isActive()) lineAnimationTimer->stop();
    if (animationTimer->isActive()) animationTimer->stop();
    if (ellipseAnimationTimer->isActive()) ellipseAnimationTimer->stop();
    if (bezierAnimTimer->isActive()) bezierAnimTimer->stop();

    setActiveAnimationStep(value);
    syncPlaybackState();
    drawgrid();
}

// ---- BEZIER CURVE ----

QVector<QPoint> MainWindow::deCasteljauStep(const QVector<QPoint> &pts, double t) {
    QVector<QPoint> result;
    if (pts.size() < 2) return pts;
    for (int i = 0; i < pts.size() - 1; ++i) {
        int x = qRound(pts[i].x() * (1 - t) + pts[i + 1].x() * t);
        int y = qRound(pts[i].y() * (1 - t) + pts[i + 1].y() * t);
        result.append(QPoint(x, y));
    }
    return result;
}

QVector<QPoint> MainWindow::calculateBezierCurve() {
    QElapsedTimer timer;
    timer.start();
    bezierLogs.clear();
    bezierCurvePoints.clear();

    if (bezierControlPoints.size() < 4) return {};

    int steps = 200;
    for (int i = 0; i <= steps; ++i) {
        double t = (double)i / steps;

        // Recursive de Casteljau
        QVector<QPoint> working = bezierControlPoints;
        QVector<QString> intermediateSteps;
        while (working.size() > 1) {
            intermediateSteps.append(QString("  [%1]").arg(working.size()));
            working = deCasteljauStep(working, t);
        }
        QPoint pt = working[0];
        bezierCurvePoints.append(pt);
        bezierLogs.append(QString("<span style='color:#f5c2e7'>t=%1</span> -> (%2, %3)")
                          .arg(t, 0, 'f', 3).arg(pt.x()).arg(pt.y()));
    }

    qint64 elapsed = timer.nsecsElapsed();
    QLabel* lblTime = findChild<QLabel*>("lblCurveTime");
    QLabel* lblCount = findChild<QLabel*>("lblCurveCount");
    QLabel* lblSteps = findChild<QLabel*>("lblCurveSteps");
    if (lblTime) lblTime->setText("Bezier Time: " + formattime(elapsed));
    if (lblCount) lblCount->setText("Curve Points: " + QString::number(bezierCurvePoints.size()));
    if (lblSteps) lblSteps->setText("Steps: " + QString::number(steps));

    return bezierCurvePoints;
}

void MainWindow::handleBezierDrawInstantly() {
    if (bezierControlPoints.size() < 4) {
        statusBar()->showMessage("Place 4 control points first.");
        return;
    }
    calculateBezierCurve();
    ui->textDebugger->clear();
    for (const QString &log : bezierLogs)
        ui->textDebugger->append(log);
    drawgrid();
    statusBar()->showMessage("Bezier curve drawn instantly.");
}

void MainWindow::handleBezierAnimate() {
    if (bezierControlPoints.size() < 4) {
        statusBar()->showMessage("Place 4 control points first.");
        return;
    }
    calculateBezierCurve();
    ui->textDebugger->clear();
    ui->textDebugger->append("Starting Bezier Animation...");

    bezierAnimPoints = bezierCurvePoints;
    bezierTraceTags.clear();
    for (int i = 0; i < bezierLogs.size(); ++i)
        bezierTraceTags.append(bezierLogs[i]);

    bezierAnimStep = 0;
    syncPlaybackState();
    int interval = qRound(20 / animationSpeedMultiplier);
    if (interval < 1) interval = 1;
    bezierAnimTimer->start(interval);
    statusBar()->showMessage("Animating Bezier curve...");
}

void MainWindow::animateBezierStep() {
    if (bezierAnimStep < 0 || bezierAnimStep >= bezierAnimPoints.size()) {
        bezierAnimTimer->stop();
        statusBar()->showMessage("Animation Complete.");
        syncPlaybackState();
        return;
    }
    if (bezierAnimStep < bezierLogs.size()) {
        ui->textDebugger->append(bezierLogs[bezierAnimStep]);
    }
    bezierAnimStep++;
    syncPlaybackState();
    drawgrid();
}

void MainWindow::handleBezierClear() {
    bezierControlPoints.clear();
    bezierCurvePoints.clear();
    bezierAnimPoints.clear();
    bezierLogs.clear();
    bezierTraceTags.clear();
    bezierAnimStep = -1;
    if (bezierAnimTimer->isActive()) bezierAnimTimer->stop();
    QLabel* lblTime = findChild<QLabel*>("lblCurveTime");
    QLabel* lblCount = findChild<QLabel*>("lblCurveCount");
    QLabel* lblSteps = findChild<QLabel*>("lblCurveSteps");
    if (lblTime) lblTime->setText("Bezier Time: -");
    if (lblCount) lblCount->setText("Curve Points: -");
    if (lblSteps) lblSteps->setText("Steps: -");
    drawgrid();
    statusBar()->showMessage("Bezier cleared.");
}

// ---- SCENE / UNDO / DELETE / ZOOM ----

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        deleteSelectedShape();
        return;
    }
    if (event->key() == Qt::Key_Z && (event->modifiers() & Qt::ControlModifier)) {
        if (event->modifiers() & Qt::ShiftModifier) redo();
        else undo();
        return;
    }
    if (event->key() == Qt::Key_Escape) {
        selectedShapeIndex = -1;
        drawgrid();
        return;
    }
    QMainWindow::keyPressEvent(event);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui->frame && event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent*>(event);
        keyPressEvent(ke);
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::pushUndo() {
    SceneSnapshot snap;
    snap.shapes = scene;
    undoStack.append(snap);
    if (undoStack.size() > 200) undoStack.removeFirst();
    redoStack.clear();
}

void MainWindow::undo() {
    if (undoStack.isEmpty()) { statusBar()->showMessage("Nothing to undo."); return; }
    SceneSnapshot current;
    current.shapes = scene;
    redoStack.append(current);
    scene = undoStack.takeLast().shapes;
    selectedShapeIndex = -1;
    pixelBuffer.clear();
    for (Shape &s : scene) rasterizeShapeToBuffer(s);
    drawgrid();
    statusBar()->showMessage("Undo.");
}

void MainWindow::redo() {
    if (redoStack.isEmpty()) { statusBar()->showMessage("Nothing to redo."); return; }
    SceneSnapshot current;
    current.shapes = scene;
    undoStack.append(current);
    scene = redoStack.takeLast().shapes;
    selectedShapeIndex = -1;
    pixelBuffer.clear();
    for (Shape &s : scene) rasterizeShapeToBuffer(s);
    drawgrid();
    statusBar()->showMessage("Redo.");
}

int MainWindow::hitTestShape(const QPoint &logical) const {
    for (int i = scene.size() - 1; i >= 0; --i) {
        const Shape &s = scene[i];
        switch (s.type) {
        case Shape::LINE: {
            double dx = s.lineP2.x() - s.lineP1.x();
            double dy = s.lineP2.y() - s.lineP1.y();
            double len2 = dx * dx + dy * dy;
            if (len2 < 1) { if (nearpoint(logical, s.lineP1)) return i; break; }
            double t = qMax(0.0, qMin(1.0, ((logical.x() - s.lineP1.x()) * dx + (logical.y() - s.lineP1.y()) * dy) / len2));
            QPoint proj(s.lineP1.x() + qRound(t * dx), s.lineP1.y() + qRound(t * dy));
            if (nearpoint(logical, proj)) return i;
            break;
        }
        case Shape::CIRCLE: {
            double dx = logical.x() - s.circleCenter.x();
            double dy = logical.y() - s.circleCenter.y();
            double dist = qSqrt(dx * dx + dy * dy);
            if (qAbs(dist - s.circleRadius) <= 1.5) return i;
            break;
        }
        case Shape::ELLIPSE: {
            double dx = logical.x() - s.ellipseCenter.x();
            double dy = logical.y() - s.ellipseCenter.y();
            double norm = (dx * dx) / (double)(s.ellipseRx * s.ellipseRx) + (dy * dy) / (double)(s.ellipseRy * s.ellipseRy);
            if (qAbs(norm - 1.0) < 0.15) return i;
            break;
        }
        case Shape::POLYGON: {
            for (const QPoint &v : s.polygonVertices)
                if (nearpoint(logical, v)) return i;
            for (int j = 0; j < s.polygonVertices.size(); ++j) {
                QPoint a = s.polygonVertices[j];
                QPoint b = s.polygonVertices[(j + 1) % s.polygonVertices.size()];
                double dx = b.x() - a.x();
                double dy = b.y() - a.y();
                double len2 = dx * dx + dy * dy;
                if (len2 < 1) continue;
                double t = qMax(0.0, qMin(1.0, ((logical.x() - a.x()) * dx + (logical.y() - a.y()) * dy) / len2));
                QPoint proj(a.x() + qRound(t * dx), a.y() + qRound(t * dy));
                if (nearpoint(logical, proj)) return i;
            }
            break;
        }
        case Shape::BEZIER: {
            for (const QPoint &cp : s.bezierControlPoints)
                if (nearpoint(logical, cp)) return i;
            break;
        }
        }
    }
    return -1;
}

void MainWindow::addShapeToScene(Shape &s) {
    scene.append(s);
    selectedShapeIndex = -1;
}

void MainWindow::rasterizeShapeToBuffer(Shape &s) {
    s.shapePixels.clear();
    qint64 dt = 0;
    QColor fillColor = s.color;

    switch (s.type) {
    case Shape::LINE: {
        QVector<QPoint> pts = calculatedda(s.lineP1, s.lineP2, dt);
        for (const QPoint &p : pts) {
            pixelBuffer[p].append(fillColor);
            s.shapePixels[p].append(fillColor);
        }
        QVector<QPoint> pts2 = calculatebresenham(s.lineP1, s.lineP2, dt);
        QColor bc = fillColor.lighter(130);
        for (const QPoint &p : pts2) {
            pixelBuffer[p].append(bc);
            s.shapePixels[p].append(bc);
        }
        break;
    }
    case Shape::CIRCLE: {
        if (s.circleRadius <= 0) break;
        QVector<QPoint> polar = calculateCirclePolar(s.circleCenter, s.circleRadius, dt);
        for (const QPoint &p : polar) {
            pixelBuffer[p].append(fillColor);
            s.shapePixels[p].append(fillColor);
        }
        QVector<QPoint> mid = calculateCircleMidpoint(s.circleCenter, s.circleRadius, dt);
        QColor mc = QColor(100, 255, 100);
        for (const QPoint &p : mid) {
            pixelBuffer[p].append(mc);
            s.shapePixels[p].append(mc);
        }
        QVector<QPoint> cart = calculateCircleCartesian(s.circleCenter, s.circleRadius, dt);
        QColor cc = QColor(100, 200, 255);
        for (const QPoint &p : cart) {
            pixelBuffer[p].append(cc);
            s.shapePixels[p].append(cc);
        }
        break;
    }
    case Shape::ELLIPSE: {
        if (s.ellipseRx <= 0 || s.ellipseRy <= 0) break;
        QVector<QPoint> polar = calculateEllipsePolar(s.ellipseCenter, s.ellipseRx, s.ellipseRy, dt);
        for (const QPoint &p : polar) {
            pixelBuffer[p].append(fillColor);
            s.shapePixels[p].append(fillColor);
        }
        QVector<QPoint> mid = calculateEllipseMidpoint(s.ellipseCenter, s.ellipseRx, s.ellipseRy, dt);
        QColor mc = QColor(0, 245, 212);
        for (const QPoint &p : mid) {
            pixelBuffer[p].append(mc);
            s.shapePixels[p].append(mc);
        }
        QVector<QPoint> cart = calculateEllipseCartesian(s.ellipseCenter, s.ellipseRx, s.ellipseRy, dt);
        QColor cc = QColor(56, 189, 248);
        for (const QPoint &p : cart) {
            pixelBuffer[p].append(cc);
            s.shapePixels[p].append(cc);
        }
        break;
    }
    case Shape::POLYGON: {
        for (int i = 0; i < s.polygonVertices.size(); ++i) {
            QPoint p1 = s.polygonVertices[i];
            QPoint p2 = s.polygonVertices[(i + 1) % s.polygonVertices.size()];
            QVector<QPoint> edge = calculatedda(p1, p2, dt);
            for (const QPoint &p : edge) {
                if (!pixelBuffer[p].contains(fillColor)) {
                    pixelBuffer[p].append(fillColor);
                }
                s.shapePixels[p].append(fillColor);
            }
        }
        break;
    }
    case Shape::BEZIER: {
        for (int i = 0; i < s.bezierCurvePoints.size() - 1; ++i) {
            QVector<QPoint> seg = calculatedda(s.bezierCurvePoints[i], s.bezierCurvePoints[i+1], dt);
            for (const QPoint &p : seg) {
                pixelBuffer[p].append(fillColor);
                s.shapePixels[p].append(fillColor);
            }
        }
        break;
    }
    }
}

void MainWindow::removeShapePixelsFromBuffer(const Shape &s) {
    for (auto it = s.shapePixels.constBegin(); it != s.shapePixels.constEnd(); ++it) {
        if (pixelBuffer.contains(it.key())) {
            for (const QColor &c : it.value()) {
                pixelBuffer[it.key()].removeAll(c);
            }
            if (pixelBuffer[it.key()].isEmpty()) {
                pixelBuffer.remove(it.key());
            }
        }
    }
}

void MainWindow::deleteSelectedShape() {
    if (selectedShapeIndex < 0 || selectedShapeIndex >= scene.size()) {
        statusBar()->showMessage("No shape selected to delete.");
        return;
    }
    pushUndo();
    scene.remove(selectedShapeIndex);
    selectedShapeIndex = -1;
    pixelBuffer.clear();
    for (Shape &s : scene) rasterizeShapeToBuffer(s);
    drawgrid();
    statusBar()->showMessage("Shape deleted.");
}

void MainWindow::drawSceneShapes(QPainter &painter) {
    for (int i = 0; i < scene.size(); ++i) {
        const Shape &s = scene[i];
        if (i != selectedShapeIndex) continue;

        QPen selPen(QColor(255, 255, 100, 200));
        selPen.setWidth(2);
        selPen.setStyle(Qt::DashLine);
        painter.setPen(selPen);

        switch (s.type) {
        case Shape::LINE: {
            QPoint s1 = logicaltoscreen(s.lineP1);
            QPoint s2 = logicaltoscreen(s.lineP2);
            painter.drawLine(s1, s2);
            painter.fillRect(s1.x() - 3, s1.y() - 3, 7, 7, QColor(255, 255, 100, 180));
            painter.fillRect(s2.x() - 3, s2.y() - 3, 7, 7, QColor(255, 255, 100, 180));
            break;
        }
        case Shape::CIRCLE: {
            QPoint sc = logicaltoscreen(s.circleCenter);
            int sr = s.circleRadius * gridsize;
            if (sr > 0) {
                painter.drawEllipse(sc, sr + 3, sr + 3);
                painter.drawEllipse(sc, sr - 3, sr - 3);
            }
            painter.fillRect(sc.x() - 3, sc.y() - 3, 7, 7, QColor(255, 255, 100, 180));
            break;
        }
        case Shape::ELLIPSE: {
            QPoint sc = logicaltoscreen(s.ellipseCenter);
            int srx = s.ellipseRx * gridsize;
            int sry = s.ellipseRy * gridsize;
            if (srx > 0 && sry > 0) {
                painter.drawEllipse(sc, srx + 3, sry + 3);
                painter.drawEllipse(sc, srx - 3, sry - 3);
            }
            painter.fillRect(sc.x() - 3, sc.y() - 3, 7, 7, QColor(255, 255, 100, 180));
            break;
        }
        case Shape::POLYGON: {
            if (s.polygonVertices.size() >= 2) {
                for (int j = 0; j < s.polygonVertices.size(); ++j) {
                    QPoint a = logicaltoscreen(s.polygonVertices[j]);
                    QPoint b = logicaltoscreen(s.polygonVertices[(j + 1) % s.polygonVertices.size()]);
                    painter.drawLine(a.x(), a.y(), b.x(), b.y());
                }
            }
            for (const QPoint &v : s.polygonVertices) {
                QPoint sv = logicaltoscreen(v);
                painter.fillRect(sv.x() - 3, sv.y() - 3, 7, 7, QColor(255, 255, 100, 180));
            }
            break;
        }
        case Shape::BEZIER: {
            if (s.bezierCurvePoints.size() >= 2) {
                for (int j = 0; j < s.bezierCurvePoints.size() - 1; ++j) {
                    painter.drawLine(logicaltoscreen(s.bezierCurvePoints[j]),
                                     logicaltoscreen(s.bezierCurvePoints[j + 1]));
                }
            }
            for (const QPoint &cp : s.bezierControlPoints) {
                QPoint scp = logicaltoscreen(cp);
                painter.fillRect(scp.x() - 3, scp.y() - 3, 7, 7, QColor(255, 255, 100, 180));
            }
            break;
        }
        }
    }
}
