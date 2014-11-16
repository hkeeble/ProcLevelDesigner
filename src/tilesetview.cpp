#include "tilesetview.h"

TilesetView::TilesetView(QObject *parent) :
    QGraphicsScene(parent)
{
    over = false;
    hasTileset = false;
    selectedTile = new QGraphicsRectItem(QRect(0, 0, 16, 16));
}

void TilesetView::setTileset(Tileset* tileset)
{
    this->tileset = tileset;
    hasTileset = true;
    QList<TilePattern*> patterns = tileset->getPatternList();
    for(TilePattern* pattern : patterns)
    {

    }
}

void TilesetView::drawBackground(QPainter *painter, const QRectF &rect)
{

}

void TilesetView::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    if(hasTileset)
    {
        currentMousePos = mouseEvent->scenePos();

        if(!currentMousePos.isNull())
        {
            currentTilePos = QPoint(currentMousePos.x() / tileset->getTileSize(), currentMousePos.y() / tileset->getTileSize());
            if(currentTilePos.x() < tileset->getWidth() && currentTilePos.y() < tileset->getHeight())
            {
                selectedTile->setRect(currentTilePos.x() * tileset->getTileSize(), currentTilePos.y() * tileset->getTileSize(),
                                      tileset->getTileSize(), tileset->getTileSize());
                selectedTile->setZValue(1);

                if(!items().contains(selectedTile))
                    addItem(selectedTile);
            }
        }
        else
        {
            removeItem(selectedTile);
        }
    }
    this->update();
}

void TilesetView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->update();
}

void TilesetView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    this->update();
}
