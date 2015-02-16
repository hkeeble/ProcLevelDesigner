#include "abilitymodel.h"

AbilityModel::AbilityModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    map = nullptr;
}

int AbilityModel::rowCount(const QModelIndex &parent) const
{
    if(map)
        return map->size();
    return 0;
}

int AbilityModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant AbilityModel::data(const QModelIndex &index, int role) const
{
    if(!map)
        return QVariant();
    if(index.row() < 0 || index.row() >= map->size() || role != Qt::DisplayRole)
    {
        return QVariant();
    }
    if(index.column() == 0)
        return ABILITY_STRINGS_FORMATTED[static_cast<int>(map->keys().at(index.row()))];
    if(index.column() == 1)
        return map->values().at(index.row());
    return QVariant();
}

bool AbilityModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.column() == 1)
    {
        if(role == Qt::EditRole)
        {
            int& val = map->find(static_cast<Hero::Ability>(index.row())).value();
            val = value.toInt();
        }
    }
    return true;
}

Qt::ItemFlags AbilityModel::flags(const QModelIndex &index) const
{
    if(index.column() == 0)
        return QAbstractItemModel::flags(index);
    else
        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

QVariant AbilityModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case 0:
            return "Ability";
        case 1:
            return "Value";
        default:
            return "Unknown";
        }
    }

    return QVariant();
}
