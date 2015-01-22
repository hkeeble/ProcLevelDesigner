#ifndef MAPENTITY_H
#define MAPENTITY_H

#include "filetools.h"

/*!
 * \brief Abstract class, represents a map entity. Derive for custom map entities.
 */
class MapEntity
{
protected:
    MapEntity() : entityName("NULL"), layer(0), x(0), y(0) { }
    MapEntity(int layer, int x, int y) : entityName("NULL"), name("NULL"), layer(layer), x(x), y(y) { }
    MapEntity(int layer, int x, int y, QString name) : name(name), layer(layer), x(x), y(y) { }

    QString name;
    QString entityName;
    int layer, x, y;

public:
    static void Parse(MapEntity* mapEntity, Object* object);
    virtual void build(Object* object);

    /*!
     * \brief Clones an entity, for use in copying polymorphic collections.
     */
    virtual MapEntity* clone() = 0;

    /**
     * @brief Retrieve the name given to this entity.
     * @return
     */
    QString getName() const { return name; }

    /**
     * @brief Retrieve the entity name.
     * @return
     */
    QString getEntityName() { return entityName; }
};

#endif // MAPENTITY_H
