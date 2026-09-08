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
#include <QStackedWidget>
#include <QToolButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QColorDialog>
#include <QQueue>
#include <QMessageBox>
#include <QSvgRenderer>
#include <QScrollArea>
#include <QKeyEvent>
#include <QUndoStack>
#include <QUndoCommand>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    struct Shape {
        enum Type { LINE, CIRCLE, ELLIPSE, POLYGON, BEZIER };
        Type type;
        QColor color;
        bool selected;
        QPoint lineP1, lineP2;
        QPoint circleCenter;
        int circleRadius;
        QPoint ellipseCenter;
        int ellipseRx, ellipseRy;
        QVector<QPoint> polygonVertices;
        QVector<QPoint> bezierControlPoints;
        QVector<QPoint> bezierCurvePoints;
        QHash<QPoint, QList<QColor>> shapePixels;
    };

    struct SceneSnapshot {
        QVector<Shape> shapes;
    };

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

    void on_btnDrawEllipse_clicked();
    void on_btnAnimateEllipse_clicked();
    void animateEllipseStep();
    void on_btnClearEllipse_clicked();
    void on_comboEllipseAlgo_currentIndexChanged(int index);
    void on_comboEllipsePoint_currentIndexChanged(int index);
    void on_spinBoxRx_valueChanged(int arg1);
    void on_spinBoxRy_valueChanged(int arg1);
    void on_chkQuadColors_stateChanged(int arg1);
    void on_chkRegionSplit_stateChanged(int arg1);
    
    void on_btnCommitEllipse_clicked();
    void on_btnClearCanvas_clicked();
    void on_sliderRotation_valueChanged(int value);
    void on_sliderThickness_valueChanged(int value);
    
    // New Tool Selection Slots
    void selectTool(int toolIndex);
    
    // Polygon / Fill Slots
    void handlePolygonCloseClicked();
    void handlePolygonClearClicked();
    void handleFillColorClicked();
    void handleBoundaryColorClicked();
    void handleClearCanvasClicked();
    void handleToggleNavbar();

    // Transformation Slots
    void handleTransformTranslate();
    void handleTransformRotate();
    void handleTransformScale();
    void handleTransformShear();
    void handleTransformReflectX();
    void handleTransformReflectY();
    void handleTransformReflectOrigin();
    void handleTransformArbitraryLine();
    void handleTransformArbitraryPoint();

    // Playback Slots
    void onStepBackClicked();
    void onStepForwardClicked();
    void onScrubSliderChanged(int value);

    // Bezier Slots
    void handleBezierAnimate();
    void handleBezierDrawInstantly();
    void handleBezierClear();
    void animateBezierStep();
    QVector<QPoint> calculateBezierCurve();
    QVector<QPoint> deCasteljauStep(const QVector<QPoint> &pts, double t);

    // Transformation Math
    void multiplyMatrix3x3(double m1[3][3], double m2[3][3], double result[3][3]);
    QPoint transformPoint(const QPoint& p, double T[3][3]);
    void applyTransformationMatrix(double T[3][3]);

    // Playback helpers
    void syncPlaybackState();
    int getActiveAnimationStep() const;
    void setActiveAnimationStep(int step);
    int getActiveAnimationPointCount() const;
    QVector<QPoint> getActiveAnimationPoints() const;

    // Scene / Undo / Zoom
    void pushUndo();
    void undo();
    void redo();
    void deleteSelectedShape();
    int hitTestShape(const QPoint &logical) const;
    void addShapeToScene(Shape &s);
    void drawSceneShapes(QPainter &painter);
    void rasterizeShapeToBuffer(Shape &s);
    void removeShapePixelsFromBuffer(const Shape &s);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

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

    void drawEllipseSymmetry(QPainter &painter, const QVector<QPoint> &points, const QColor &color, QPoint center, int rx, int ry, int rotation, int thickness, bool addToBuffer = false, bool regionHighlight = false);
    void removeShapeFromBuffer(const QVector<QPoint> &points, const QColor &color);
    QVector<QPoint> calculateEllipsePolar(QPoint center, int rx, int ry, qint64 &time);
    QVector<QPoint> calculateEllipseMidpoint(QPoint center, int rx, int ry, qint64 &time);
    QVector<QPoint> calculateEllipseCartesian(QPoint center, int rx, int ry, qint64 &time);

    // New Algorithms
    void floodFill(const QPoint &startNode, const QColor &targetColor, const QColor &replacementColor);
    void boundaryFill(const QPoint &startNode, const QColor &fillColor, const QColor &boundaryColor);
    void scanlineFillPolygon(const QVector<QPoint> &vertices, const QColor &fillColor, QVector<QPoint> *committed = nullptr);
    void drawPolygonEdges(QPainter &painter, const QVector<QPoint> &vertices, const QColor &color);

    QPoint screentological(const QPoint &pos) const;
    QPoint logicaltoscreen(const QPoint &pos) const;
    bool nearpoint(const QPoint &logical, const QPoint &target) const;

    QString formattime(qint64 nanoseconds) const;
    void calculatealgorithms();
    void calculateCircleAlgorithms();
    void calculateEllipseAlgorithms();

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
    bool showTrace;

    QTimer *lineAnimationTimer;
    int lineAnimationStep;
    QVector<QPoint> lineAnimationPoints;
    QVector<QString> lineAnimationLogs;
    QVector<QString> traceTags;

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

    QPoint ellipseCenter;
    int ellipseRx;
    int ellipseRy;
    bool hasEllipseCenter;
    bool hasEllipseRx;
    bool hasEllipseRy;
    bool ellipseVisible;
    int ellipseDraggingPoint;

    QVector<QPoint> ellipsePolarPoints;
    QVector<QPoint> ellipseMidpointPoints;
    QVector<QPoint> ellipseCartesianPoints;
    QVector<QString> ellipsePolarLogs;
    QVector<QString> ellipseMidpointLogs;
    QVector<QString> ellipseCartesianLogs;

    qint64 ellipsePolarTime;
    qint64 ellipseMidpointTime;
    qint64 ellipseCartesianTime;

    int selectedEllipseAlgorithm;
    
    QTimer *ellipseAnimationTimer;
    int ellipseAnimationStep;
    QVector<QPoint> ellipseAnimationPoints;
    QVector<QString> ellipseAnimationLogs;
    QColor ellipseAnimationColor;
    
    bool ellipseQuadColors;
    bool ellipseRegionSplit;
    
    int ellipseRotation;
    int ellipseThickness;
    
    struct PersistentEllipse {
        QPoint center;
        int rx;
        int ry;
        int rotation;
        int thickness;
        int algorithm;
        QColor color;
    };
    QVector<PersistentEllipse> persistentEllipses;

    QHash<QPoint, QList<QColor>> pixelBuffer;
    
    // Editor UI Architecture State
    enum ActiveTool { TOOL_LINE, TOOL_CIRCLE, TOOL_ELLIPSE, TOOL_POLYGON, TOOL_FLOOD_FILL, TOOL_BOUNDARY_FILL, TOOL_SCANLINE_FILL, TOOL_TRANSFORM, TOOL_CURVE };
    ActiveTool currentTool;
    QStackedWidget *settingsStack;
    QList<QToolButton*> sidebarButtons;
    QComboBox *comboPolygonMode;
    QFrame *navbar;
    QLabel *toolNameLabel;
    QPushButton *btnToggleNavbar;

    // Polygon State
    QVector<QPoint> activePolygonPoints;
    QVector<QPoint> committedPolygonPixels;
    QVector<QPoint> lastClosedPolygonVertices;
    bool polygonClosed;
    QColor polygonFillColor;

    // Fill Tool State
    QColor currentFillColor;
    QColor currentBoundaryColor;
    bool colorPickerActive;
    int colorPickerMode; // 0=fill, 1=edge, 2=boundary
    int fillConnectivity; // 4 or 8

    // Bezier Curve State
    QVector<QPoint> bezierControlPoints;  // P0, P1, P2, P3 (max 4)
    QVector<QPoint> bezierCurvePoints;    // evaluated curve at t=0..1
    QVector<QString> bezierLogs;
    QVector<QString> bezierTraceTags;
    int bezierAnimStep;
    QVector<QPoint> bezierAnimPoints;
    QTimer *bezierAnimTimer;
    QColor bezierColor;

    // Transform Overlay State (visible arb line / arb point)
    bool hasArbLine;
    double arbLineX1, arbLineY1, arbLineX2, arbLineY2;
    bool hasArbPoint;
    double arbPointX, arbPointY;
    bool keepOriginalOnTransform;

    // ---- PERSISTENT SCENE ----
    QVector<Shape> scene;
    int selectedShapeIndex;

    // ---- UNDO/REDO ----
    QVector<SceneSnapshot> undoStack;
    QVector<SceneSnapshot> redoStack;

    // ---- ZOOM / PAN ----
    double viewScale;
    double viewOffsetX;
    double viewOffsetY;
    bool isPanning;
    QPoint panStart;
    QPoint lastDragLogical;
    
    // Legacy Animation logic references...
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