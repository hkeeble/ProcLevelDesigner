#ifndef MISSION_H
#define MISSION_H

#include <QList>
#include <QDebug>
#include <QObject>

#include "stage.h"
#include "randomengine.h"
#include "missiongenerationoptions.h"

class MissionObserver : public QObject
{
    Q_OBJECT;

public:
    void emitUpdate() { emit updated(); }

signals:
    void updated();
};

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
     * \brief Parse a mission item collection from a table.
     */
    static Mission Parse(Table* data);

    /*!
     * \brief build the mission item collection into the given table.
     */
    void build(Table* table);

    /*!
     * \brief Add a new gate to this collection. Returns true if addition was successful (returns false if name of gate exists).
     */
    bool addGate(QString name, Gate gate);

    /*!
     * \brief Add a new key event to this collection. Returns true if addition was successful (returns false if name of key event exists).
     */
    bool addKeyEvent(QString name, Key key);

    /*!
     * \brief Remove a key event from this collection. Returns false if the specified key was not found.
     */
    bool removeKeyEvent(QString name);

    /*!
     * \brief Removes a gate from this collection. Returns false if the specified gate was not found.
     */
    bool removeGate(QString name);

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
     * \brief getKeys Get the map of keys in this mission.
     */
    QMap<QString,Key>* getKeys();

    /*!
     * \brief Retrieves a list of all names of key events.
     */
    QStringList getKeyEventNameList();

    /*!
     * \brief Retrieves a list of all names of gates.
     * \return
     */
    QStringList getGateNameList();

    /*!
     * \brief Get the list of stages in this mission.
     */
    QList<Stage*> getStages();

    /*!
     * \brief Emit an update signal to ensure any slots connected to the mission are updated.
     */
    void emitUpdate() { observer->emitUpdate(); }

    /*!
     * \brief Validates this mission, checking whether or not the mission is valid. Returns false if the mission has become invalid. The validation
     * function will only validate the mission for key events that are IN USE in the mission. If a key is not in use, then it is assumed that it's placement
     * will be validated elsewhere.
     */
    bool validate();

    /*!
     * \brief Retrieves all keys that are currently in an invalid location. If the list is empty, the mission is valid.
     */
    QList<Key*> getInvalidKeys();

    /*!
     * \brief Validates the potential addition of the given key to the given stage, and returns whether or not this would invalidate the mission
     *        structure for any reason.
     * \param stage The stage we want to add the key to.
     * \param key The key we want to add.
     * \return True if the mission structure would still be valid.
     */
    bool validateAddition(Stage* stage, Key* key);

    /*!
     * \brief Checks to see if this key already exists in the mission structure.
     * \param key The key to check for.
     */
    bool doesKeyExist(Key* key);

    MissionObserver* getObserver() { return observer; }

    /*!
     * \brief Returns a list of all the keys that are currently in use, that is they are placed into a stage.
     */
    QList<Key*> getUsedKeys();

    /*!
     * \brief Clear all placed keys.
     */
    void clearKeys();

    /*!
     * \brief Toggle lock in of a key in a stage. If the given stage does not contain the given key, nothing happens.
     * \param stage The stage containing the key to lock in.
     * \param key The key to lock in.
     */
    void toggleLockInKey(Stage* stage, Key* key);

    /*!
     * \brief Removes a key from a stage. If the given stage does not contain the given key, nothing happens.
     * \param stage The stage to remove the key from.
     * \param key The key to remove.
     */
    void removeKey(Stage* stage, Key* key);

    /*!
     * \brief Toggles lock in state for the given gate.
     * \param gate The gate to lock in.
     */
    void toggleLockInGate(Gate* gate);

    MissionGenerationOptions& getOptions() { return options; }

private:
    /*!
     * \brief Internal function to call whenever stage links have been updated (updates IDs for parsing/building)
     */
    void updateStageIDs();

    MissionObserver* observer;   /*!< This observer is a QObject used to emit an update signal when the mission is modified. */
    QMap<QString,Gate> gates;    /*!< The gates contained in this collection. */
    QMap<QString,Key> keyEvents; /*!< The key events contained in this collection. */
    QList<Stage> stages;

    MissionGenerationOptions options; /*!< Contains user-defined options for generation constraints and preferences. */
};

#endif // MISSION_H
