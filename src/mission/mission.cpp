#include "mission.h"

Mission::Mission()
{

}

void Mission::init(Table* itemData)
{
    itemCollection = MissionItemCollection::Parse(itemData);
}
