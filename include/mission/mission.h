#ifndef MISSION_H
#define MISSION_H

#include "missionitemcollection.h"

class Mission
{
public:
    Mission();
    virtual ~Mission() { }

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
};

#endif // MISSION_H
