#ifndef DOOR_H
#define DOOR_H

#include <QString>

#include "filetools.h"
#include "mapentity.h"

/*!
 * \brief Represents a door map entity.
 */
class Door : public MapEntity
{
public:
    Door() : direction(Direction::EAST), openingMethod(OpeningMethod::None), sprite("NULL") { entityName = OBJ_DOOR; }

    virtual Door* clone() override final;

    /*!
     * \brief The direction a door is facing.
     */
    enum Direction
    {
        EAST,
        NORTH,
        WEST,
        SOUTH
    };

    /*!
     * \brief The opening method for a door.
     */
    enum class OpeningMethod
    {
        None,
        Interaction,
        InteractionIfSavegameVariable,
        InteractionIfItem,
        Explosion
    };

    /*!
     * \brief Parses a door object from a data object.
     * \param Object containing the door.
     */
    static Door Parse(Object* object);

    /*!
     * \brief Builds the door into a given object.
     * \param Object to build the door into.
     */
    virtual void build(Object* object) override final;

private:
    // Used to check when parsing opening method, and assign the correct enum.
    static const QString NONE;
    static const QString INTERACTION;
    static const QString INTERACTION_IF_SAVEGAME_VARIABLE;
    static const QString INTERACTION_IF_ITEM;
    static const QString EXPLOSION;

    Direction direction;
    OpeningMethod openingMethod;
    QString sprite;
};

#endif // DOOR_H
