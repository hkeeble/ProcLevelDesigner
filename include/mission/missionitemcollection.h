#ifndef MISSIONITEMCOLLECTION_H
#define MISSIONITEMCOLLECTION_H

#include <QList>
#include "key.h"
#include "gate.h"
#include "filetools.h"

/*!
 * \brief Represents a collection of mission items, specifically key events and gates, that can be used to form a mission structure.
 */
class MissionItemCollection
{
public:
    MissionItemCollection();
    virtual ~MissionItemCollection();

    /*!
     * \brief Parse a mission item collection from a table.
     */
    static MissionItemCollection Parse(Table* data);

    /*!
     * \brief Build the mission item collection into the given table.
     */
    void Build(Table* table);

    /*!
     * \brief Add a new gate to this collection. Returns true if addition was successful (returns false if name of gate exists).
     */
    bool AddGate(QString name, Gate gate);

    /*!
     * \brief Add a new key event to this collection. Returns true if addition was successful (returns false if name of key event exists).
     */
    bool AddKeyEvent(QString name, Key key);

    /*!
     * \brief Remove a key event from this collection. Returns false if the specified key was not found.
     */
    bool RemoveKeyEvent(QString name);

    /*!
     * \brief Removes a gate from this collection. Returns false if the specified gate was not found.
     */
    bool RemoveGate(QString name);

    /*!
     * \brief Retrieves a pointer to the key event with the given name. Returns null pointer of key event was not found.
     */
    Key* getKeyEvent(QString name);

    /*!
     * \brief Retrieves a pointer to the gate with the given name. Returns null pointer of gate was not found.
     */
    Gate* getGate(QString name);


    /*!
     * \brief Retrieves a list of pointers to all gates in this collection.
     */
    QList<Gate*> getGateList();

    /*!
     * \brief Retrieves a list of pointers to all key events in this collection.
     */
    QList<Key*> getKeyEventList();

    /*!
     * \brief Retrieves a list of all names of key events.
     */
    QStringList getKeyEventNameList();

    /*!
     * \brief Retrieves a list of all names of gates.
     * \return
     */
    QStringList getGateNameList();

private:
    QMap<QString,Gate> gates; /*!< The gates contained in this collection. */
    QMap<QString,Key> keyEvents; /*!< The key events contained in this collection. */
};

#endif // MISSIONITEMCOLLECTION_H
