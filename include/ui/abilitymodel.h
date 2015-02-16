#ifndef ABILITYMODEL_H
#define ABILITYMODEL_H

#include <QAbstractTableModel>

#include "hero.h"

class AbilityModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    enum AbilityRoles {
        KeyRole = Qt::UserRole + 1,
        ValueRole
    };

    explicit AbilityModel(QObject *parent = 0);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    /*!
     * \brief Sets the map displayed by this model. Pass in the hero's ability map here.
     */
    inline void setMap(QMap<Hero::Ability,int>* map) { this->map = map; }

private:
    QMap<Hero::Ability,int>* map;
};

#endif // ABILITYMODEL_H
