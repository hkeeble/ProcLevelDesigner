#ifndef MISSION_H
#define MISSION_H

#include <QList>
#include <QDebug>
#include <QObject>

#include "stage.h"

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
    QList<Stage*> getStages() { return stages; }

    MissionObserver* getObserver() { return observer; }

private:
    MissionObserver* observer; /*!< This observer is a QObject used to emit an update signal when the mission is modified. */
    QMap<QString,Gate> gates; /*!< The gates contained in this collection. */
    QMap<QString,Key> keyEvents; /*!< The key events contained in this collection. */
    QList<Stage*> stages;
};

#endif // MISSION_H
