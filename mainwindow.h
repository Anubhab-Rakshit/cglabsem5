#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QPainter>
#include <QColor>
#include <QVector>
#include <QString>
#include <QElapsedTimer>
#include <QtMath>
#include <QResizeEvent>
#include <QFileDialog>
#include <QTimer>
#include <algorithm>
#include <QHash>
#include <QList>
#include <QSet>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showmouseposition(QPoint &pos);
    void mouse_pressed();
    void mouse_dragged(QPoint &pos);
    void mouse_released();
    void mouse_wheel(int delta);

    void on_clear_clicked();
    void on_draw_line_clicked();
    void on_btnAnimateLine_clicked();
    void animateLineStep();
    void on_spinBox_textChanged(const QString &arg1);
    void on_spinBox_valueChanged(int arg1);
    void on_sliderSpeed_valueChanged(int value);
    
    void on_btnAnimateCircle_clicked();
    void animateCircleStep();

    void on_comboLineAlgo_currentIndexChanged(int index);
    void on_comboLinePoint_currentIndexChanged(int index);
    void on_comboCircleAlgo_currentIndexChanged(int index);
    void on_comboCirclePoint_currentIndexChanged(int index);
    void on_spinBoxRadius_valueChanged(int arg1);
    void on_btnClearCircle_clicked();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;

    void drawgrid();
    void drawpoint(QPainter &painter, const QPoint &logical, const QColor &color);
    void drawMultiColorPoint(QPainter &painter, const QPoint &logical, const QList<QColor> &colors);
    void drawaxispixel(QPainter &painter, const QPoint &logical);
    void drawdda(QPainter &painter, const QVector<QPoint> &points, bool addToBuffer = false);
    void drawbresenham(QPainter &painter, const QVector<QPoint> &points, bool addToBuffer = false);
    void renderPixelBuffer(QPainter &painter);

    QVector<QPoint> calculatedda(QPoint p0, QPoint p1, qint64 &time);
    QVector<QPoint> calculatebresenham(QPoint p0, QPoint p1, qint64 &time);

    void drawCircleSymmetry(QPainter &painter, const QVector<QPoint> &points, const QColor &color, bool addToBuffer = false);
    QVector<QPoint> calculateCirclePolar(QPoint center, int radius, qint64 &time);
    QVector<QPoint> calculateCircleMidpoint(QPoint center, int radius, qint64 &time);
    QVector<QPoint> calculateCircleCartesian(QPoint center, int radius, qint64 &time);

    QPoint screentological(const QPoint &pos) const;
    QPoint logicaltoscreen(const QPoint &pos) const;
    bool nearpoint(const QPoint &logical, const QPoint &target) const;

    QString formattime(qint64 nanoseconds) const;
    void calculatealgorithms();
    void calculateCircleAlgorithms();

    QPoint point1;
    QPoint point2;

    QVector<QPoint> ddapoints;
    QVector<QPoint> bresenhampoints;
    QVector<QString> ddaLogs;
    QVector<QString> bresenhamLogs;

    bool haspoint1;
    bool haspoint2;
    bool linevisible;

    int draggingpoint;
    bool isDragging;
    int selectedalgorithm;

    QTimer *lineAnimationTimer;
    int lineAnimationStep;
    QVector<QPoint> lineAnimationPoints;
    QVector<QString> lineAnimationLogs;

    qint64 ddatime;
    qint64 bresenhamtime;

    QVector<QPoint> polarPoints;
    QVector<QPoint> midpointPoints;
    QVector<QPoint> cartesianPoints;
    QVector<QString> polarLogs;
    QVector<QString> midpointLogs;
    QVector<QString> cartesianLogs;

    qint64 polarTime;
    qint64 midpointTime;
    qint64 cartesianTime;

    int selectedCircleAlgorithm;
    
    QTimer *animationTimer;
    int animationStep;
    QVector<QPoint> animationPoints;
    QVector<QString> animationLogs;

    QHash<QPoint, QList<QColor>> pixelBuffer;
    
    float animationSpeedMultiplier;

    int gridsize;
    int originx;
    int originy;

    int sc_x;
    int sc_y;
    int org_x;
    int org_y;
};

#endif