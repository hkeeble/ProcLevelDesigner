#include "tilesetview.h"

TilesetView::TilesetView(QObject *parent) :
    QGraphicsScene(parent)
{
    over = false;
    hasTileset = false;
    leftMouseDown = false;
    rightMouseDown = false;

    selectedTile = new QGraphicsRectItem(QRect(0, 0, 16, 16));
    blockedMarkers = QVector<QVector<QGraphicsRectItem*>>();

    blockedBrush = QBrush(QColor(255, 0, 0));
}

TilesetView::~TilesetView()
{
    if(!items().contains(selectedTile) && selectedTile)
        delete selectedTile;
    blockedMarkers.clear();
}

QPoint TilesetView::getMouseTilePos(QPointF scenePos)
{
    return QPoint(scenePos.x() / tileset->getTileSize(), scenePos.y() / tileset->getTileSize());
}

void TilesetView::enableBlocked(int x, int y)
{
    if(patterns[x][y]->traversable)
    {
        TilePattern* pattern = patterns[x][y];
        pattern->traversable = false;
        blockedMarkers[x][y] = new QGraphicsRectItem(QRect(pattern->x + pattern->width/4, pattern->y + pattern->height/4,
                                                           pattern->width/2, pattern->height/2));
        blockedMarkers[x][y]->setBrush(blockedBrush);
        addItem(blockedMarkers[x][y]);
    }
}

void TilesetView::disableBlocked(int x, int y)
{
    if(!patterns[x][y]->traversable)
    {
        patterns[x][y]->traversable = true;
        removeItem(blockedMarkers[x][y]);
        delete blockedMarkers[x][y];
        blockedMarkers[x][y] = nullptr;
    }
}

bool TilesetView::isInBounds(QPoint pos)
{
    return pos.x() >= 0 && pos.y() >= 0 && pos.x() < tileset->getWidth() && pos.y() < tileset->getHeight();
}

void TilesetView::mouseOutOfBounds()
{
    over = false;
    leftMouseDown = false;
    rightMouseDown = false;
    if(items().contains(selectedTile))
        removeItem(selectedTile);
}

void TilesetView::setTileset(Tileset* tileset)
{
    if(items().contains(selectedTile))
    {
        removeItem(selectedTile);
        delete selectedTile;
        selectedTile = new QGraphicsRectItem(QRect(0, 0, 16, 16));
    }

    clear();
    blockedMarkers.clear();

    this->tileset = tileset;
    this->setSceneRect(QRect(0, 0, tileset->getImage().width(), tileset->getImage().height()));
    addPixmap(tileset->getImage());

    hasTileset = true;
    patterns = tileset->getPatternGrid();

    blockedMarkers = QVector<QVector<QGraphicsRectItem*>>(tileset->getWidth(), QVector<QGraphicsRectItem*>(tileset->getHeight()));

    // Add initial blocked markers
    for(int x = 0; x < tileset->getWidth(); x++)
    {
        for(int y = 0; y < tileset->getHeight(); y++)
        {
            blockedMarkers[x][y] = nullptr;
            TilePattern* pattern = patterns[x][y];
            if(!patterns[x][y]->traversable)
            {
                blockedMarkers[x][y] = new QGraphicsRectItem(QRect(pattern->x + pattern->width/4, pattern->y + pattern->height/4,
                                                                   pattern->width/2, pattern->height/2));
                blockedMarkers[x][y]->setBrush(blockedBrush);
                addItem(blockedMarkers[x][y]);
            }
        }
    }
}

void TilesetView::drawBackground(QPainter *painter, const QRectF &rect)
{

}

void TilesetView::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    // Check for mouse movements on the scene
    if(hasTileset)
    {
        QPoint tilePos = getMouseTilePos(mouseEvent->scenePos());

        if(isInBounds(tilePos))
        {
            over = true;
            selectedTile->setRect(tilePos.x() * tileset->getTileSize(), tilePos.y() * tileset->getTileSize(),
                                      tileset->getTileSize(), tileset->getTileSize());
            selectedTile->setZValue(1);

            if(!items().contains(selectedTile))
                addItem(selectedTile);
        }
        else
            mouseOutOfBounds();
    }

    // Check for click and drag
    if(over && (leftMouseDown || rightMouseDown) && mouseEvent->lastScenePos() != mouseEvent->scenePos())
    {
        QPoint tilePos = getMouseTilePos(mouseEvent->scenePos());
        if(tilePos != currentTilePos)
        {
            currentTilePos = tilePos;
            if(isInBounds(tilePos))
            {
                if(rightMouseDown)
                    disableBlocked(currentTilePos.x(), currentTilePos.y());
                else if(leftMouseDown)
                    enableBlocked(currentTilePos.x(), currentTilePos.y());
            }
            else
                mouseOutOfBounds();
        }
    }

    this->update();
}

void TilesetView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(over)
    {
        if(event->button() == Qt::LeftButton)
            leftMouseDown = true;
        if(event->button() == Qt::RightButton)
            rightMouseDown = true;

        currentTilePos = getMouseTilePos(event->scenePos());
    }

    if(over && leftMouseDown)
        enableBlocked(currentTilePos.x(), currentTilePos.y());
    else if(over && rightMouseDown)
        disableBlocked(currentTilePos.x(), currentTilePos.y());

    this->update();
}

void TilesetView::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{

}

void TilesetView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        leftMouseDown = false;
    if(event->button() == Qt::RightButton)
        rightMouseDown = false;

    this->update();
}


