#include "my_label.h"

my_label::my_label(QWidget *parent)
    : QLabel(parent)
{
    setMouseTracking(true);

    x = 0;
    y = 0;
}

void my_label::mouseMoveEvent(QMouseEvent *ev)
{
    x = ev->pos().x();
    y = ev->pos().y();

    QPoint pos = ev->pos();

    emit sendMousePosition(pos);

    // If left mouse button is being held
    // we are dragging.
    if (ev->buttons() & Qt::LeftButton)
    {
        emit sendMouseDrag(pos);
    }
}

void my_label::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        x = ev->pos().x();
        y = ev->pos().y();

        emit Mouse_Pos();
    }
}

void my_label::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        emit Mouse_Release();
    }
}

void my_label::wheelEvent(QWheelEvent *ev)
{
    emit sendMouseWheel(ev->angleDelta().y());
}