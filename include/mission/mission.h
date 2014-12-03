#ifndef MISSION_H
#define MISSION_H

#include <QList>
#include <QDebug>

#include "missionitemcollection.h"
#include "stage.h"

class Mission
{
public:
    Mission();
    virtual ~Mission();

    /*!
     * \brief generate Generates this mission.
     */
    void generate();

    /*!
     * \brief init Initialize this mission.
     * \param itemData The table containing all item data to use for this mission.
     */
    void init(Table* itemData);

    MissionItemCollection* getItems() { return &itemCollection; }

private:
    MissionItemCollection itemCollection;
    QList<Stage*> stages;
};

#endif // MISSION_H
