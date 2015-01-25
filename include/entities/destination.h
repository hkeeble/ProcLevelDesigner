#ifndef DESTINATION_H
#define DESTINATION_H

#include "mapentity.h"
#include "direction.h"

/**
 * @brief Represents a destination on a map. A destination can be the target of a teletransporter, or can be used when setting the game starting
 *        location.
 */
class Destination : public MapEntity
{
public:
    Destination() : Destination(0, 0, 0, NULL_ELEMENT, Direction::EAST) { }

    Destination(int layer, int x, int y, QString name, Direction direction, bool defaultDestination = true) : MapEntity(layer, x, y, name),
        direction(direction), defaultDestination(defaultDestination) { entityName = OBJ_DESTINATION; }

    virtual ~Destination() { }

    virtual Destination* clone() override final;

    /*!
     * \brief Parses a destination object from a data object.
     * \param Object containing the door.
     */
    static Destination Parse(Object* object);

    /*!
     * \brief Builds the destination into a given object.
     * \param Object to build the destination into.
     */
    virtual void build(Object* object) override final;

private:
    Direction direction;
    bool defaultDestination;
};

#endif // DESTINATION_H
