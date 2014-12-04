#include "spacescene.h"

SpaceScene::SpaceScene(QObject *parent) :
    QGraphicsScene(parent)
{
}


void SpaceScene::setSpace(Space* space)
{
    this->space = space;
    QObject::connect(space->getObserver(), SIGNAL(updated()), this, SLOT(spaceUpdated()));
    spaceUpdated();
}


void SpaceScene::spaceUpdated()
{

}
