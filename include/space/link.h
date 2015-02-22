#ifndef LINK_H
#define LINK_H

#include <QPoint>

#include "direction.h"
#include "filetools.h"

/*!
 * \brief Represents a link between two areas. The origin and target QPoint show the location of the two points that are linked.
 *        Also holds a gate, if the gate is null there is not gate attached to this link.
 */
class Link
{
public:
    Link();

    /*!
     * \brief Create a new link, given a destination.
     * \param origin          The origin of the area origin area of this link.
     * \param originRelative  The relative cell location in the given origin area that this link is in.
     * \param target         The origin of the area target area of this link.
     * \param direction     The direction of the link.
     */
    Link(const QPoint& origin, const QPoint& originRelative, const QPoint& target, const Direction& direction);

    virtual ~Link();

    bool operator ==(const Link& link);

    static Link Parse(Object* obj);
    void build(Object* obj);

    QPoint getOrigin() const { return origin; }
    QPoint getTarget() const { return target; }
    QPoint getOriginRelative() const { return originRelative; }
    Direction getDirection() const { return direction; }

private:
    Direction direction;
    QPoint originRelative;
    QPoint origin, target;
};

#endif // LINK_H
