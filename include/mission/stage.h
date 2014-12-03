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
    Stage(int id, Stage* previousStage, Stage* nextStage, Gate* previousGate, Gate* nextGate, QList<Key*> keys);
    virtual ~Stage();

    void addKey(Key* key) { keys.append(key); }
    bool removeKey(Key* key) { keys.removeOne(key); }
    QList<Key*> getKeys() { return keys; }

    /*!
     * \brief Get this stage's ID.
     */
    int getID() { return id; }

    /*!
     * \brief Get the stage that follows this one.
     */
    Stage* getNextStage()     { return nextStage; }

    /*!
     * \brief Get the stage before this one.
     */
    Stage* getPreviousStage() { return previousStage; }

    /*!
     * \brief Get the gate that must be passed to get to this stage.
     */
    Gate* getPreviousGate()    { return previousGate; }

    /*!
     * \brief Get the gate that ends this stage.
     */
    Gate* getNextGate()     { return nextGate; }

    /*!
     * \brief Set the stage that follows this one.
     */
    void setNextStage(Stage* next) { this->nextStage = next; }

    /*!
     * \brief Set the stage before this one.
     */
    void setPreviousStage(Stage* previous) { this->previousStage = previous; }

    /*!
     * \brief Set the gate that must be passed to get to this stage.
     */
    void setPreviousGate(Gate* previous) { this->previousGate = previous; }

    /*!
     * \brief Set the gate that ends this stage.
     */
    void setNextGate(Gate* next) { this->nextGate = next; }

private:
    QList<Key*> keys;
    int id;
    Stage *previousStage, *nextStage;
    Gate *previousGate, *nextGate;
};

#endif // STAGE_H
