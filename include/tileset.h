#ifndef TILESET_H
#define TILESET_H

#include <QPixmap>
#include <QSet>

#include "filetools.h"

struct TilePattern
{
    TilePattern();
    virtual ~TilePattern();

    static TilePattern parse(Object object);
    Object build();

    int id, x, y, width, height, defaultLayer;
    bool traversable;
};

class Tileset
{
public:
    Tileset();
    Tileset(QString name, Table* data);
    virtual ~Tileset();

    static Tileset create(QString name, QString filePath, Table* data, int tileSize);
    static Tileset parse(QString name, Table* data);

    inline TilePattern getPattern(int id) { return patterns.find(id).value(); }
    inline void addPattern(TilePattern pattern) { patterns.insert(pattern.id, pattern); }

    inline QMap<int,TilePattern>* getPatterns() { return &patterns; }
    QList<TilePattern*> getPatternList();

    inline QString getName() { return name; }
    inline void setName(QString name) { this->name = name; }

    inline int getTileSize() { return tileSize; }
    inline int getWidth() { return width; }
    inline int getHeight() { return height; }

    inline QPixmap getImage() { return image; }

    inline void saveToDisk() { data->saveToDisk(); }

private:
    Table* data;
    QString name;
    QPixmap image; /*!< The image representing the tileset. */
    int tileSize; /*!< The size of each individual tile in pixels. */
    int width, height; /*!< Width and height (in tile count) of the tileset. */
    QMap<int,TilePattern> patterns; /*!< Map containing all patterns for this tileset. */
};

#endif // TILESET_H
