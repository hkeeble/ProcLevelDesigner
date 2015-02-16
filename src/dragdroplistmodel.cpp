#include "dragdroplistmodel.h"

DragDropListModel::DragDropListModel(QObject *parent) :
    QStringListModel(parent)
{
}

Qt::ItemFlags DragDropListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QStringListModel::flags(index);

    if(index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return defaultFlags;
}

 QStringList DragDropListModel::mimeTypes() const
 {
     QStringList types;
     types << "application/vnd.text.list";
     return types;
 }

 QMimeData* DragDropListModel::mimeData(const QModelIndexList &indexes) const
 {
    QMimeData* mimeData = new QMimeData();

    QModelIndex index = indexes.at(0);
    mimeData->setText(data(index, Qt::DisplayRole).toString());

    return mimeData;
 }
