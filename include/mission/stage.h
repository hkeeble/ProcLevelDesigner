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
    Stage(int id, QList<Key*> keys);
    Stage(int id, int previousID, int nextID, Gate* exitGate, QList<Key*> keys);
    Stage(int id, Stage* previous, Stage* next, Gate* exitGate, QList<Key*> keys);
    virtual ~Stage();

    void addKey(Key* key) { keys.append(key); }
    bool removeKey(Key* key) { keys.removeOne(key); }
    QList<Key*> getKeys() { return keys; }

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
    Stage* getNextStage()     { return next; }

    /*!
     * \brief Get the stage before this one.
     */
    Stage* getPreviousStage() { return previous; }

    int getPreviousStageID() { return prevID; }

    int getNextStageID() { return nextID; }

    /*!
     * \brief Get the gate that ends this stage.
     */
    Gate* getExitGate()     { return exitGate; }

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

    void clearKeys() { keys.clear(); }

    void setID(int id) { this->id = id; }

private:
    QList<Key*> keys;
    int id, nextID, prevID;
    Stage *previous, *next;
    Gate* exitGate;
};

#endif // STAGE_H
