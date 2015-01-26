#ifndef MAPSCRIPT_H
#define MAPSCRIPT_H

#include <QString>
#include <QMap>

#include "script.h"
#include "door.h"
#include "switchentity.h"
#include "key.h"

/**
 * @brief Represents an individual map script. Use member functions to add to the script.
 */
class MapScript : public Script
{
public:
    MapScript();
    virtual ~MapScript() override { }

    /**
     * @brief Adds the ability for this script to handle a switch of the given name.
     * @param keyName The name of the key containing the switch.
     */
    void addSwitch(SwitchEntity switchEntity);

    /**
     * @brief Adds the ability for this script to handle a door of a given name.
     * @param gateName The name of the gate containing the door.
     * @param keys The keys required to open the gate.
     */
    void addDoor(Door door, QList<Key*> keys);
};

#endif // MAPSCRIPT_H
