#ifndef SPACEVIEW_H
#define SPACEVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

class SpaceView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SpaceView(QWidget *parent = 0);

protected:
    virtual void wheelEvent(QWheelEvent * event) override;
public slots:

};

#endif // SPACEVIEW_H
