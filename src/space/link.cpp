#include "link.h"

Link::Link()
{

}

Link::Link(const QPoint& origin, const QPoint& originRelative, const QPoint& target, const Direction& direction)
{
    this->origin = origin;
    this->target = target;
    this->originRelative = originRelative;
    this->direction = direction;
}

Link::~Link()
{

}

bool Link::operator==(const Link& link)
{
    return this->origin == link.origin && this->target == link.target && this->originRelative == link.originRelative;
}

Link Link::Parse(Object* obj)
{
    Link link;

    int originX = obj->find(ELE_ORIGIN_X, "0").toInt();
    int originY = obj->find(ELE_ORIGIN_Y, "0").toInt();

    int targetX = obj->find(ELE_TARGET_X, "0").toInt();
    int targetY = obj->find(ELE_TARGET_Y, "0").toInt();

    int originRelX = obj->find(ELE_ORIGIN_REL_X, "0").toInt();
    int originRelY = obj->find(ELE_ORIGIN_REL_Y, "0").toInt();

    link.origin = QPoint(originX, originY);
    link.target = QPoint(targetX, targetY);
    link.originRelative = QPoint(originRelX, originRelY);

    link.direction = static_cast<Direction>(obj->find(ELE_DIRECTION, "0").toInt());

    return link;
}

void Link::build(Object* obj)
{
    obj->data.clear();

    obj->insert(ELE_ORIGIN_X, QString::number(origin.x()));
    obj->insert(ELE_ORIGIN_Y, QString::number(origin.y()));

    obj->insert(ELE_TARGET_X, QString::number(target.x()));
    obj->insert(ELE_TARGET_Y, QString::number(target.y()));

    obj->insert(ELE_ORIGIN_REL_X, QString::number(originRelative.x()));
    obj->insert(ELE_ORIGIN_REL_Y, QString::number(originRelative.y()));

    obj->insert(ELE_DIRECTION, QString::number(static_cast<int>(direction)));
}
