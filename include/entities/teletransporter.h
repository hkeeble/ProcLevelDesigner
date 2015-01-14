#ifndef TELETRANSPORTER_H
#define TELETRANSPORTER_H

#include "mapentity.h"

/*!
 * \brief Represents a teletransporter, used to move a player from one location to another.
 */
class Teletransporter : public MapEntity
{
public:
    /*!
     * \brief The type of transition to use for a teletransporter.
     */
    enum class Transition
    {
        Fade,
        Scroll,
        Immediate
    };

    Teletransporter() : width(32), height(32),
        destinationMap(""), destination(""), transition(Transition::Immediate), MapEntity() { entityName = OBJ_TELETRANSPORTER; }

    Teletransporter(int x, int y, int width, int height, QString destinationMap, QString destination, Transition transition);
    Teletransporter(int x, int y, int width, int height, QString destinationMap, QString destination, Transition transition, QString sprite);

    virtual Teletransporter* clone() override final;

    static Teletransporter Parse(Object* object);
    virtual void build(Object* object) override final;

private:
    static const QString FADE;
    static const QString SCROLL;
    static const QString IMMEDIATE;

    int width, height;
    Transition transition;
    QString destinationMap;
    QString destination;
    QString sprite;
};

#endif // TELETRANSPORTER_H
