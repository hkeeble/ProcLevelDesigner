#ifndef DOOR_H
#define DOOR_H

#include <QString>

#include "filetools.h"
#include "mapentity.h"
#include "direction.h"

/*!
 * \brief Represents a door map entity.
 */
class Door : public MapEntity
{
public:
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

    Door() : MapEntity(0, 0, 0, "NULL"), direction(Direction::EAST), openingMethod(OpeningMethod::None), sprite("NULL") { entityName = OBJ_DOOR; }
    Door(int layer, int x, int y, QString name, Direction direction, OpeningMethod openingMethod, QString sprite) : MapEntity(layer, x, y, name),
        direction(direction), openingMethod(openingMethod), sprite(sprite) { entityName = OBJ_DOOR; }

    virtual Door* clone() override final;

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
