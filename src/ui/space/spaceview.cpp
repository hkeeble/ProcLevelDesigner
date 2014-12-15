#include "spaceview.h"

SpaceView::SpaceView(QWidget *parent) :
    QGraphicsView(parent)
{

}

void SpaceView::wheelEvent(QWheelEvent * event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    double scaleFactor = 1.5f;

    if(event->delta() > 0)
        scale(scaleFactor, scaleFactor);
    else
        scale(1.0/scaleFactor, 1.0/scaleFactor);
}
