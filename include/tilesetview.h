#ifndef TILESETVIEW_H
#define TILESETVIEW_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QGraphicsItemGroup>
#include <QPainter>

#include "tileset.h"

class TilesetView : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit TilesetView(QObject *parent = 0);

    void setTileset(Tileset* tileset);

signals:

public slots:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event)      override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)    override;

private:
    QPointF currentMousePos;
    QPoint currentTilePos;
    bool over;
    bool hasTileset;

    QGraphicsRectItem* selectedTile;

    Tileset* tileset;
};

#endif // TILESETVIEW_H
