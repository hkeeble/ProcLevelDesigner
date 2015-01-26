#ifndef LINK_H
#define LINK_H

#include <QPoint>

#include "filetools.h"

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

    /*!
     * \brief Create a new link, given a destination.
     * \param origin      The origin of the area this link is leading from.
     * \param destination The origin of the area that this link leads to.
     */
    Link(QPoint origin, QPoint target);

    virtual ~Link();

    bool operator ==(const Link& link);

    static Link Parse(Object* obj);
    void build(Object* obj);

    QPoint getOrigin() { return first; }
    QPoint getTarget() { return second; }

    void setOrigin(QPoint origin) { this->first = origin; }
    void setTarget(QPoint target) { this->second = target; }

private:
    QPoint first, second;
};

#endif // LINK_H
