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
        "<svg xmlns='http://www.w3.org/2000/svg' width='32' height='32'><polygon points='16,3 29,27 3,27' fill='none' stroke='#a6adc8' stroke-width='2.5' stroke-linejoin='round'/></svg>"
    };
    const char* svgChecked[] = {
        "<svg xmlns='http://www.w3.org/2000/svg' width='32' height='32'><line x1='5' y1='27' x2='27' y2='5' stroke='#11111b' stroke-width='3' stroke-linecap='round'/></svg>",
        "<svg xmlns='http://www.w3.org/2000/svg' width='32' height='32'><circle cx='16' cy='16' r='11' fill='none' stroke='#11111b' stroke-width='3'/></svg>",
        "<svg xmlns='http://www.w3.org/2000/svg' width='32' height='32'><ellipse cx='16' cy='16' rx='13' ry='8' fill='none' stroke='#11111b' stroke-width='3'/></svg>",
        "<svg xmlns='http://www.w3.org/2000/svg' width='32' height='32'><polygon points='16,3 29,27 3,27' fill='none' stroke='#11111b' stroke-width='3' stroke-linejoin='round'/></svg>"
    };
    QStringList toolNames = {"Line", "Circle", "Ellipse", "Polygon"};
    QList<QPixmap> normalPixList, checkedPixList;
    for (int i = 0; i < 4; ++i) {
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

    QComboBox* comboPolygonMode = new QComboBox(pagePolygon);
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

    rightLayout->addWidget(settingsStack);

    // ---- DRAWING FRAME ----
    ui->frame->setStyleSheet("background-color: #0f111a; border: 2px solid #313244; border-radius: 8px;");
    rightLayout->addWidget(ui->frame, 1);

    // ---- DEBUGGER ----
    ui->groupBoxDebugger->setTitle("Algorithm Debugger");
    ui->groupBoxDebugger->setFixedHeight(120);
    rightLayout->addWidget(ui->groupBoxDebugger);

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

        bool circleToBuffer = (draggingpoint == 0);

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
    bool ellipseToBuffer = (currentTool == TOOL_ELLIPSE && ellipseVisible && ellipseDraggingPoint == 0);
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
        } else if (currentTool == TOOL_CIRCLE) {
            calculateCircleAlgorithms();
            on_btnAnimateCircle_clicked(); // Auto animate on click
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
    if (selectedalgorithm == 0 || selectedalgorithm == 2) {
        lineAnimationPoints = ddapoints;
        lineAnimationLogs = ddaLogs;
        ui->textDebugger->append("Starting DDA Line Animation...");
    } else {
        lineAnimationPoints = bresenhampoints;
        lineAnimationLogs = bresenhamLogs;
        ui->textDebugger->append("Starting Bresenham Line Animation...");
    }
    
    lineAnimationStep = 0;
    int interval = qRound(20 / animationSpeedMultiplier);
    if (interval < 1) interval = 1;
    lineAnimationTimer->start(interval);
    statusBar()->showMessage("Animating Line...");
}

void MainWindow::animateLineStep()
{
    if (lineAnimationStep >= lineAnimationPoints.size()) {
        lineAnimationTimer->stop();
        statusBar()->showMessage("Animation Complete.");
        return;
    }
    if (lineAnimationStep < lineAnimationLogs.size()) {
        ui->textDebugger->append(lineAnimationLogs[lineAnimationStep]);
    }
    lineAnimationStep++;
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
    if (selectedCircleAlgorithm == 0) {
        animationPoints = polarPoints; animationLogs = polarLogs;
        ui->textDebugger->append("Starting Polar Animation...");
    } else if (selectedCircleAlgorithm == 2) {
        animationPoints = cartesianPoints; animationLogs = cartesianLogs;
        ui->textDebugger->append("Starting Cartesian Animation...");
    } else {
        animationPoints = midpointPoints; animationLogs = midpointLogs; // Midpoint for All (Overlap)
        ui->textDebugger->append("Starting Midpoint Animation...");
    }
    
    animationStep = 0;
    int interval = qRound(50 / animationSpeedMultiplier);
    if (interval < 1) interval = 1;
    animationTimer->start(interval);
    statusBar()->showMessage("Animating Circle Symmetry...");
}

void MainWindow::animateCircleStep()
{
    if (animationStep >= animationPoints.size()) {
        animationTimer->stop();
        statusBar()->showMessage("Animation Complete.");
        return;
    }
    if (animationStep < animationLogs.size()) {
        ui->textDebugger->append(animationLogs[animationStep]);
    }
    animationStep++;
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
    
    if (selectedEllipseAlgorithm == 0) { ellipseAnimationPoints = ellipsePolarPoints; ellipseAnimationLogs = ellipsePolarLogs; ui->textDebugger->append("Starting Polar Ellipse..."); }
    else if (selectedEllipseAlgorithm == 2) { ellipseAnimationPoints = ellipseCartesianPoints; ellipseAnimationLogs = ellipseCartesianLogs; ui->textDebugger->append("Starting Cartesian Ellipse..."); }
    else { ellipseAnimationPoints = ellipseMidpointPoints; ellipseAnimationLogs = ellipseMidpointLogs; ui->textDebugger->append("Starting Midpoint (Bresenham) Ellipse..."); }
    
    ellipseAnimationStep = 0;
    int interval = qRound(30 / animationSpeedMultiplier);
    if (interval < 1) interval = 1;
    ellipseAnimationTimer->start(interval);
    statusBar()->showMessage("Animating Ellipse 4-Way Symmetry...");
}

void MainWindow::animateEllipseStep() {
    if (ellipseAnimationStep >= ellipseAnimationPoints.size()) {
        ellipseAnimationTimer->stop(); statusBar()->showMessage("Animation Complete."); return;
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
    if (toolIndex >= 4) return;
    QStringList names = {"Line", "Circle", "Ellipse", "Polygon"};
    for(int i=0; i<sidebarButtons.size(); ++i) {
        sidebarButtons[i]->setChecked(i == toolIndex);
    }
    currentTool = static_cast<ActiveTool>(toolIndex);
    settingsStack->setCurrentIndex(toolIndex);
    if (toolNameLabel) toolNameLabel->setText(names[toolIndex]);
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
