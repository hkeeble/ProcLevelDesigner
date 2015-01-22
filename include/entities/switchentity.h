#ifndef SWITCHENTITY_H
#define SWITCHENTITY_H

#include "mapentity.h"

class SwitchEntity : public MapEntity
{
public:
    enum class SubType
    {
        WALKABLE,
        SOLID,
        ARROW_TARGET
    };

    SwitchEntity() : MapEntity(0, 0, 0, "NULL") { }

    SwitchEntity(int layer, int x, int y, QString name, SubType subType, QString sprite, bool needsBlock, bool inactiveWhenLeaving)
        : SwitchEntity(layer, x, y, name, subType, sprite, "NULL", needsBlock, inactiveWhenLeaving) { entityName = OBJ_SWITCH; }

    SwitchEntity(int layer, int x, int y, QString name, SubType subType, QString sprite)
        : SwitchEntity(layer, x, y, name, subType, sprite, "NULL", false, false) { entityName = OBJ_SWITCH; }

    SwitchEntity(int layer, int x, int y, QString name, SubType subType, QString sprite, QString sound)
        : MapEntity(layer, x, y, name), subType(subType), sprite(sprite), sound(sound), needsBlock(false), inactiveWhenLeaving(false) { entityName = OBJ_SWITCH; }

    SwitchEntity(int layer, int x, int y, QString name, SubType subType, QString sprite, QString sound, bool needsBlock, bool inactiveWhenLeaving)
        : MapEntity(layer, x, y, name), subType(subType), sprite(sprite), sound(sound), needsBlock(needsBlock), inactiveWhenLeaving(inactiveWhenLeaving) { entityName = OBJ_SWITCH; }

    virtual ~SwitchEntity() { }


    virtual SwitchEntity* clone() override final;

    /*!
     * \brief Parses a switch object from a data object.
     * \param Object containing the switch.
     */
    static SwitchEntity Parse(Object* object);

    /*!
     * \brief Builds the switch into a given object.
     * \param Object to build the switch into.
     */
    virtual void build(Object* object) override final;

private:
    // Used to check when parsing opening method, and assign the correct enum.
    static const QString WALKABLE;
    static const QString SOLID;
    static const QString ARROW_TARGET;

    SubType subType;
    QString sprite;
    QString sound;
    bool needsBlock;
    bool inactiveWhenLeaving;
};

#endif // SWITCHENTITY_H
