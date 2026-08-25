#ifndef MY_LABEL_H
#define MY_LABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QWheelEvent>

class my_label : public QLabel
{
    Q_OBJECT

public:
    explicit my_label(QWidget *parent = nullptr);

    int x;
    int y;

protected:

    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev);

signals:

    void sendMousePosition(QPoint &pos);

    void sendMouseDrag(QPoint &pos);

    void Mouse_Pos();
    void Mouse_Release();
    void sendMouseWheel(int delta);
};

#endif // MY_LABEL_H