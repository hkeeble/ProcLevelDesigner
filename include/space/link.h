#ifndef LINK_H
#define LINK_H

#include <QPoint>

#include "filetools.h"
#include "gate.h"

/*!< Used to represent the direction of a link. */
enum class LinkDirection
{
    Up,
    Down,
    Left,
    Right
};

/*!
 * \brief Represents a link between two areas. The first and second QPoint show the location of the two points that are linked.
 *        Also holds a gate, if the gate is null there is not gate attached to this link.
 */
class Link
{
public:
    Link();
    virtual ~Link();

    bool operator ==(const Link& link);

    static Link Parse(Object* obj, QList<Gate*> gates);
    void build(Object* obj);

    QPoint first, second;

    Gate* getGate() { return gate; }

private:
    Gate* gate;     /*!< The gate attached to this link. Null pointer means there is no gate. */
};

#endif // LINK_H
