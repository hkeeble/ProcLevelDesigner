#ifndef SPACESCENE_H
#define SPACESCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QScopedPointer>
#include <QColor>
#include <QGraphicsSceneWheelEvent>

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
    const QColor wallColor;

    qreal areaCellWidth, areaCellHeight;
    qreal gridCellWidth, gridCellHeight;

    void clear();

    Space* space;
    QGraphicsRectItem* gridParent;
    QVector<QVector<QGraphicsRectItem*>> gridTiles;
    QVector<QGraphicsRectItem*> areaTiles;
    QVector<QGraphicsRectItem*> blockedTiles;
};

#endif // SPACESCENE_H
