#include "link.h"

Link::Link()
{
    gate = nullptr;
}

Link::Link(Gate *gate)
{
    this->gate = gate;
}

Link::~Link()
{

}

bool Link::operator==(const Link& link)
{
    return this->first == link.first && this->second == link.second;
}

Link Link::Parse(Object* obj, QList<Gate*> gates)
{
    Link link;

    int firstX = obj->find(ELE_FIRST_X, "0").toInt();
    int firstY = obj->find(ELE_FIRST_Y, "0").toInt();
    int secondX = obj->find(ELE_SECOND_X, "0").toInt();
    int secondY = obj->find(ELE_SECOND_Y, "0").toInt();

    link.first = QPoint(firstX, firstY);
    link.second = QPoint(secondX, secondY);

    QString gateName = obj->find(ELE_GATE_NAME, NULL_ELEMENT);

    if(gateName != NULL_ELEMENT)
    {
        for(Gate* gate : gates)
        {
            if(gate->getName() == gateName)
            {
                link.gate = gate;
                break;
            }
        }
    }
    else
        link.gate = nullptr;

    return link;
}

void Link::build(Object* obj)
{
    obj->data.clear();

    obj->insert(ELE_FIRST_X, QString::number(first.x()));
    obj->insert(ELE_FIRST_Y, QString::number(first.y()));
    obj->insert(ELE_SECOND_X, QString::number(second.x()));
    obj->insert(ELE_SECOND_Y, QString::number(second.y()));

    if(gate)
        obj->insert(ELE_GATE_NAME, gate->getName());
    else
        obj->insert(ELE_GATE_NAME, NULL_ELEMENT);
}
