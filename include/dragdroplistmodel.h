#ifndef DRAGDROPLISTMODEL_H
#define DRAGDROPLISTMODEL_H

#include <QStringListModel>
#include <QMimeData>

class DragDropListModel : public QStringListModel
{
    Q_OBJECT
public:
    explicit DragDropListModel(QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QStringList mimeTypes() const override;
    QMimeData* mimeData(const QModelIndexList &indexes) const override;

signals:

public slots:

};

#endif // DRAGDROPLISTMODEL_H
