#ifndef AREA_H
#define AREA_H

#include <QPoint>

#include "filetools.h"
#include "link.h"
#include "key.h"
#include "zone.h"
#include "map.h"

/*!
 * \brief An area is representative of an individual map in game. They are given a Zone pointer,
 *        which holds aesthetic data such as the tileset to be used by the area in game.
 *        Areas contain pointers to adjacent accessible areas, and their location on the grid
 *        contained by the Space object. They also contain a collection of the key events located
 *        in this area.
 */
class Area
{
public:
    Area();
    virtual ~Area();

    Area(Zone* zone, QPoint location, int height, int width, QList<Key*> keyEvents = QList<Key*>());

    Area(const Area& param);
    Area& operator=(const Area& param);

    static Area Parse(Object* obj, QList<Key*> keys);
    void build(Object* obj);

    QPoint getLocation() { return location; }

    QList<Key*> getKeyEvents() { return keyEvents; }

    QString getZoneName() { return zoneName; }

    Zone* getZone() { return zone; }
    void setZone(Zone* zone) { this->zone = zone; }

    int getWidth() { return width; }
    int getHeight() { return height; }

    Link* getLinkUp() { return up; }
    Link* getLinkLeft() { return left; }
    Link* getLinkRight() { return right; }
    Link* getLinkDown() { return down; }

    void setLinkUp(Link link) { up = new Link(link); }
    void setLinkLeft(Link link) { left = new Link(link); }
    void setLinkRight(Link link) { right = new Link(link); }
    void setLinkDown(Link link) { down = new Link(link); }

    void clearAllLinks();

    void removeRightLink();
    void removeLeftLink();
    void removeDownLink();
    void removeUpLink();

    Map buildMap();

private:
    void cpy(const Area& param); /*!< Internal copying function. */

    QString zoneName;                           /*!< Name of the zone this area should point to (only use this when parsing!) */
    Zone* zone;                                 /*!< The zone this area belongs to. */
    QPoint location;                            /*!< The grid location of this area. */
    int height, width;                          /*!< The height and width of this area. */
    QList<Key*> keyEvents;                      /*!< The key events that take place in this area. */
    Link *up, *left, *right, *down;             /*!< This area's links to other areas. */
};
#endif // AREA_H
