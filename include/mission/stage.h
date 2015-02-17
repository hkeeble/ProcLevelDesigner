#ifndef STAGE_H
#define STAGE_H

#include <QList>

#include "key.h"
#include "gate.h"

/*!
 * \brief Represents an individual stage of a mission.
 */
class Stage
{
public:
    Stage(int id);
    Stage(int id, QList<Key*> keys, QList<Key*> lockedKeys, bool gateLocked);
    Stage(int id, int previousID, int nextID, Gate* exitGate, QList<Key*> keys, QList<Key*> lockedKeys, bool gateLocked);
    Stage(int id, Stage* previous, Stage* next, Gate* exitGate, QList<Key*> keys, QList<Key*> lockedKeys, bool gateLocked);
    virtual ~Stage();

    void addKey(Key* key) { keys.append(key); }
    bool removeKey(Key* key) { return keys.removeOne(key); }
    QList<Key*> getKeys() const { return keys; }

    /*!
     * \brief Parse a stage from the data passed in.
     */
    static Stage Parse(QList<Gate*> gates, QList<Key*> keys, Object* data);

    /*!
     * \brief Build the stage into the data passed in.
     */
    void build(Object* data);

    /*!
     * \brief Get this stage's ID.
     */
    int getID() { return id; }

    /*!
     * \brief Get the stage that follows this one.
     */
    Stage* getNextStage() const     { return next; }

    /*!
     * \brief Get the stage before this one.
     */
    Stage* getPreviousStage() { return previous; }

    int getPreviousStageID() { return prevID; }

    int getNextStageID() { return nextID; }

    /*!
     * \brief Get the gate that ends this stage.
     */
    Gate* getExitGate() const     { return exitGate; }

    /*!
     * \brief Set the stage that follows this one.
     */
    void setNextStage(Stage* next) { this->next = next; }

    /*!
     * \brief Set the stage before this one.
     */
    void setPreviousStage(Stage* previous) { this->previous = previous; }

    /*!
     * \brief Set the stage ID that follows this one.
     */
    void setNextStageID(int id) { this->nextID = id; }

    /*!
     * \brief Set the stage ID before this one.
     */
    void setPreviousStageID(int id) { this->prevID = id; }

    /*!
     * \brief Set the gate that ends this stage.
     */
    void setExitGate(Gate* gate) { this->exitGate = gate; }

    /*!
     * \brief Clears all keys.
     */
    void clearKeys() { keys.clear(); lockedKeys.clear(); }

    /*!
     * \brief Clears all keys that are not locked into this stage.
     */
    void clearUnlockedKeys();

    void setID(int id) { this->id = id; }

    /*!
     * \brief Get pointer to the key with the given name.
     * \param name The name of the key to retrieve.
     * \return The pointer to the key contained in this stage. If the key does not exist in this stage, returns nullptr.
     */
    Key* getKey(QString name);

    /*!
     * \brief Locks the given key into this stage. If the stage does not contain the given key, then this function does nothing.
     * \param key The key to lock into the stage.
     */
    void lockKey(Key* key);

    /*!
     * \brief Unlocks the given key from this stage. If the stage does not contain the given key, then this functions does nothing.
     * \param key
     */
    void unlockKey(Key* key);

    /*!
     * \brief Gets a list of all the keys locked into this stage.
     */
    QList<Key*> getLockedKeys() { return lockedKeys; }

    /*!
     * \brief Checks if the given key is locked into this stage.
     * \param key The key to check for locking.
     * \return If the key is locked into this stage, returns true. If the key is not locked, or does not exist in this stage, then returns false.
     */
    bool isKeyLocked(Key* key);

    /*!
     * \brief Set whether or not the gate for this stage is locked in place.
     */
    void setGateLocked(const bool& locked) { gateLocked = locked; }

    /*!
     * \brief Checks if this stage contains a locked gate (it should not move upon regeneration of the mission).
     */
    bool isGateLocked() { return gateLocked; }

private:
    QList<Key*> keys;
    QList<Key*> lockedKeys; /*!< List of keys locked in this stage. Locked keys are not removed when generating space. */
    bool gateLocked; /*!< Whether or not the gate paired to this stage is locked in place. */
    int id, nextID, prevID;
    Stage *previous, *next;
    Gate* exitGate;
};

#endif // STAGE_H
