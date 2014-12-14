#ifndef SPACESCENE_H
#define SPACESCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QScopedPointer>

#include "space.h"

class SpaceScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SpaceScene(QObject *parent = 0);

    virtual ~SpaceScene();

    void setSpace(Space* space);

public slots:
    void spaceUpdated();

private:
    qreal cellWidth, cellHeight;

    void clear();

    Space* space;
    QVector<QGraphicsRectItem*> tiles;
};

#endif // SPACESCENE_H
