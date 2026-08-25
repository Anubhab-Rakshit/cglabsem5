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
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, [this](int index) {
        Q_UNUSED(index);
        animationStep = -1;
        lineAnimationStep = -1;
        if (animationTimer->isActive()) animationTimer->stop();
        if (lineAnimationTimer->isActive()) lineAnimationTimer->stop();
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
        if (ui->tabWidget->currentIndex() == 0) {
            pixelBuffer.clear();
            if (selectedalgorithm == 2) {
                // Overlap Both
                if (lineAnimationStep >= 0) {
                    int subsetDda = (lineAnimationStep < ddapoints.size()) ? lineAnimationStep : ddapoints.size();
                    int subsetBres = (lineAnimationStep < bresenhampoints.size()) ? lineAnimationStep : bresenhampoints.size();
                    drawdda(painter, ddapoints.mid(0, subsetDda), true);
                    drawbresenham(painter, bresenhampoints.mid(0, subsetBres), true);
                } else {
                    drawdda(painter, ddapoints, true);
                    drawbresenham(painter, bresenhampoints, true);
                }
                renderPixelBuffer(painter);
            } else {
                QVector<QPoint> activePoints = (selectedalgorithm == 0) ? ddapoints : bresenhampoints;
                if (lineAnimationStep >= 0) {
                    int subsetSize = (lineAnimationStep < activePoints.size()) ? lineAnimationStep : activePoints.size();
                    QVector<QPoint> animatedSubset = activePoints.mid(0, subsetSize);
                    if (selectedalgorithm == 0) drawdda(painter, animatedSubset);
                    else drawbresenham(painter, animatedSubset);
                } else {
                    if (selectedalgorithm == 0) drawdda(painter, ddapoints);
                    else drawbresenham(painter, bresenhampoints);
                }
            }
        } else if (ui->tabWidget->currentIndex() == 1) {
            pixelBuffer.clear();
            if (selectedCircleAlgorithm == 3) {
                // Draw All (Overlap)
                if (animationStep >= 0) {
                    int sPolar = (animationStep < polarPoints.size()) ? animationStep : polarPoints.size();
                    int sMid = (animationStep < midpointPoints.size()) ? animationStep : midpointPoints.size();
                    int sCart = (animationStep < cartesianPoints.size()) ? animationStep : cartesianPoints.size();
                    drawCircleSymmetry(painter, polarPoints.mid(0, sPolar), QColor(255, 100, 200), true);
                    drawCircleSymmetry(painter, midpointPoints.mid(0, sMid), QColor(100, 255, 100), true);
                    drawCircleSymmetry(painter, cartesianPoints.mid(0, sCart), QColor(100, 200, 255), true);
                } else {
                    drawCircleSymmetry(painter, polarPoints, QColor(255, 100, 200), true);
                    drawCircleSymmetry(painter, midpointPoints, QColor(100, 255, 100), true);
                    drawCircleSymmetry(painter, cartesianPoints, QColor(100, 200, 255), true);
                }
                renderPixelBuffer(painter);
            } else {
                QVector<QPoint> activePoints;
                QColor color;
                if (selectedCircleAlgorithm == 0) { activePoints = polarPoints; color = QColor(255, 100, 200); }
                else if (selectedCircleAlgorithm == 1) { activePoints = midpointPoints; color = QColor(100, 255, 100); }
                else { activePoints = cartesianPoints; color = QColor(100, 200, 255); }
                
                if (animationStep >= 0) {
                    int subsetSize = (animationStep < activePoints.size()) ? animationStep : activePoints.size();
                    QVector<QPoint> animatedSubset = activePoints.mid(0, subsetSize);
                    drawCircleSymmetry(painter, animatedSubset, color);
                } else {
                    drawCircleSymmetry(painter, activePoints, color);
                }
            }
        }
    }

    if (haspoint1)
        drawpoint(painter, point1, QColor(255, 70, 70));

    if (haspoint2)
        drawpoint(painter, point2, QColor(70, 255, 120));

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
    
    QPoint clickpos(org_x, org_y);
    QPoint logical = screentological(clickpos);

    if (haspoint1 && nearpoint(logical, point1))
    {
        draggingpoint = 1;
        statusBar()->showMessage("Dragging Point 1");
        return;
    }

    if (haspoint2 && nearpoint(logical, point2))
    {
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

        if (ui->tabWidget->currentIndex() == 0) {
            calculatealgorithms();
            on_btnAnimateLine_clicked(); // Auto animate on click
        } else {
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
    
    if (draggingpoint == 0)
        return;

    QPoint logical = screentological(pos);

    if (draggingpoint == 1 && haspoint1)
    {
        point1 = logical;
        ui->comboLinePoint->setItemText(0, "Point 1 : (" + QString::number(point1.x()) + ", " + QString::number(point1.y()) + ")");
        ui->comboCirclePoint->setItemText(0, "Center : (" + QString::number(point1.x()) + ", " + QString::number(point1.y()) + ")");
    }
    else if (draggingpoint == 2 && haspoint2)
    {
        point2 = logical;
        ui->comboLinePoint->setItemText(1, "Point 2 : (" + QString::number(point2.x()) + ", " + QString::number(point2.y()) + ")");
        ui->comboCirclePoint->setItemText(1, "Radius Pt : (" + QString::number(point2.x()) + ", " + QString::number(point2.y()) + ")");
    }

    linevisible = haspoint1 && haspoint2;

    if (linevisible) {
        if (ui->tabWidget->currentIndex() == 0)
            calculatealgorithms();
        else
            calculateCircleAlgorithms();
    }

    drawgrid();

    statusBar()->showMessage("Point 1: (" + QString::number(point1.x()) + ", " + QString::number(point1.y()) + ")    |    Point 2: (" + QString::number(point2.x()) + ", " + QString::number(point2.y()) + ")    |    Dragging...");
}

void MainWindow::mouse_released()
{
    draggingpoint = 0;
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

    point1 = QPoint(0, 0);
    point2 = QPoint(0, 0);

    ddapoints.clear();
    bresenhampoints.clear();

    ddatime = 0;
    bresenhamtime = 0;
    
    lineAnimationStep = -1;
    if (lineAnimationTimer->isActive()) lineAnimationTimer->stop();
    
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