#ifndef TILESETVIEW_H
#define TILESETVIEW_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QGraphicsItemGroup>
#include <QPainter>

#include "tileset.h"

/*!
 * \brief Graphics view used to display a tileset in the editor. Contains functionality for adding/removing and displaying blocked tiles.
 */
class TilesetView : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit TilesetView(QObject *parent = 0);

    ~TilesetView();

    void setTileset(Tileset* tileset);

signals:

public slots:
    void drawBackground(QPainter *painter, const QRectF &rect) override final;
    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent) override final;
    void mousePressEvent(QGraphicsSceneMouseEvent *event)      override final;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)    override final;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event)     override final;


private:
    void enableBlocked(int x, int y);
    void disableBlocked(int x, int y);
    void mouseOutOfBounds();
    bool isInBounds(QPoint pos);

    QPoint getMouseTilePos(QPointF scenePos);

    QPointF currentMousePos;
    QPoint prevTilePos;
    QPoint currentTilePos;

    bool over;
    bool rightMouseDown;
    bool leftMouseDown;
    bool hasTileset;

    QBrush blockedBrush;
    QVector<QVector<QGraphicsRectItem*>> blockedMarkers;
    QGraphicsRectItem* selectedTile;

    Tileset* tileset;
    QVector<QVector<TilePattern*>> patterns;
};

#endif // TILESETVIEW_H
