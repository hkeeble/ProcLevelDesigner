#include "link.h"

Link::Link()
{

}

Link::Link(QPoint origin, QPoint target)
{
    this->first = origin;
    this->second = target;
}

Link::~Link()
{

}

bool Link::operator==(const Link& link)
{
    return this->first == link.first && this->second == link.second;
}

Link Link::Parse(Object* obj)
{
    Link link;

    int firstX = obj->find(ELE_FIRST_X, "0").toInt();
    int firstY = obj->find(ELE_FIRST_Y, "0").toInt();
    int secondX = obj->find(ELE_SECOND_X, "0").toInt();
    int secondY = obj->find(ELE_SECOND_Y, "0").toInt();

    link.first = QPoint(firstX, firstY);
    link.second = QPoint(secondX, secondY);

    return link;
}

void Link::build(Object* obj)
{
    obj->data.clear();

    obj->insert(ELE_FIRST_X, QString::number(first.x()));
    obj->insert(ELE_FIRST_Y, QString::number(first.y()));
    obj->insert(ELE_SECOND_X, QString::number(second.x()));
    obj->insert(ELE_SECOND_Y, QString::number(second.y()));
}
