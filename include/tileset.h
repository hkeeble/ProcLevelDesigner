#ifndef TILESET_H
#define TILESET_H

#include <QImage>
#include <QSet>

#include "filetools.h"

struct Color { int r, g, b; };

struct TilePattern
{
    TilePattern();
    virtual ~TilePattern();

    static TilePattern parse(Object object);

    int id, x, y, width, height, defaultLayer;
    bool traversable;
};

class Tileset
{
public:
    Tileset();
    Tileset(Table* data);
    virtual ~Tileset();

    static Tileset create(QString name, Table* data, int tileSize);

    inline TilePattern getPattern(int id) { return patterns.find(id).value(); }
    inline void addPattern(TilePattern pattern) { patterns.insert(pattern.id, pattern); }

private:
    Table* data;
    QImage image; /*!< The image representing the tileset. */
    QMap<int,TilePattern> patterns; /*!< Map containing all patterns for this tileset. */
    Color color;
};

#endif // TILESET_H
