#ifndef SPACESCENE_H
#define SPACESCENE_H

#include <QGraphicsScene>

#include "space.h"

class SpaceScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SpaceScene(QObject *parent = 0);

    void setSpace(Space* space);

public slots:
    void spaceUpdated();

private:
    Space* space;
};

#endif // SPACESCENE_H
