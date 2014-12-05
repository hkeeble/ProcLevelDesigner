#ifndef ZONE_H
#define ZONE_H

#include <QPoint>
#include <QList>

#include "gate.h"
#include "key.h"
#include "tileset.h"
#include "link.h"

class Zone
{
public:
    Zone();
    Zone(QString name, int areaCount, Tileset* tileset);
    virtual ~Zone();

    void setTileset(Tileset* tileset) { this->tileset = tileset; }

    inline Tileset* getTileset() { return tileset; }
    inline QString getName() { return name; }
    inline int getAreaCount() { return areaCount; }

    static Zone Parse(Object* object, QList<Tileset*> tilesets);
    void build(Object* object);

private:
    QString name;             /*!< The name of this zone. */
    int areaCount;            /*!< The maximum number of areas that can be within this zone. */
    Tileset* tileset;         /*!< The tileset used by this zone. */
};

#endif // ZONE_H
