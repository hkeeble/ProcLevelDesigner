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
    Zone(QString name, int areaCount, Tileset* tileset, QColor color);
    virtual ~Zone();

    inline void setTileset(Tileset* tileset) { this->tileset = tileset; }
    inline void setAreaCount(int count) { this->areaCount = count; }
    inline void setName(QString name) { this->name = name; }
    inline void setColor(QColor color) { this->color = color; }

    inline Tileset* getTileset() { return tileset; }
    inline QString getName() { return name; }
    inline int getAreaCount() { return areaCount; }
    inline QColor getColor() { return color; }

    static Zone Parse(Object* object, QList<Tileset*> tilesets);
    void build(Object* object);

private:
    QColor color;             /*!< Color used to represent this zone. */
    QString name;             /*!< The name of this zone. */
    int areaCount;            /*!< The maximum number of areas that can be within this zone. */
    Tileset* tileset;         /*!< The tileset used by this zone. */
};

#endif // ZONE_H
