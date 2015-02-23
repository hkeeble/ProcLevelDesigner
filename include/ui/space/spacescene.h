#ifndef SPACESCENE_H
#define SPACESCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QScopedPointer>
#include <QColor>
#include <QGraphicsSceneWheelEvent>
#include <QStatusBar>

#include "space.h"

class RectContainer
{
public:
    RectContainer() { rect = nullptr; }
    QGraphicsRectItem* getRect() const { return rect; }

    void resetColor() { rect->setBrush(initialBrush); }

protected:
    RectContainer(QGraphicsRectItem* rect) : rect(rect), initialBrush(rect->brush()) { }

private:
    void copy(const RectContainer& param);

    QBrush initialBrush;
    QGraphicsRectItem* rect;
};

class AreaTile : public RectContainer
{
public:
    AreaTile() : RectContainer(), area(nullptr) { }
    AreaTile(Area* area, QGraphicsRectItem* rect) : area(area), RectContainer(rect) { }

    Area* getArea() { return area; }
private:
    Area* area;
};

class GateTile : public RectContainer
{
public:
    GateTile() : RectContainer(), gate(nullptr) { }
    GateTile(Gate* gate, QGraphicsRectItem* rect) : gate(gate), RectContainer(rect) { }

    Gate* getGate() { return gate; }
private:
    Gate* gate;
};

class KeyTile : public RectContainer
{
public:
    KeyTile() : RectContainer(), key(nullptr) { }
    KeyTile(Key* key, QGraphicsRectItem* rect) : key(key), RectContainer(rect) { }

    Key* getKey() { return key; }

private:
    Key* key;
};

class SpaceScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SpaceScene(QStatusBar* statusBar, QObject *parent = 0);

    virtual ~SpaceScene();

    void setSpace(Space* space);

public slots:
    void spaceUpdated();

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

private:

    QStatusBar* statusBar;

    const QColor wallColor;
    const QColor gateColor;
    const QColor keyColor;

    AreaTile* highlightedArea;

    qreal areaCellWidth, areaCellHeight;
    qreal gridCellWidth, gridCellHeight;

    void clear();

    QPoint mousePos;

    Space* space;
    QGraphicsRectItem* gridParent;
    QVector<QVector<QGraphicsRectItem*>> gridTiles;
    QVector<QGraphicsRectItem*> blockedTiles;

    QVector<AreaTile> areaTiles;
    QVector<GateTile> gates;
    QVector<KeyTile> keys;
};

#endif // SPACESCENE_H
