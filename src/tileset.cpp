#include "tileset.h"

TilePattern::TilePattern()
{

}

TilePattern::~TilePattern()
{

}

TilePattern TilePattern::parse(Object object)
{
    TilePattern pattern;

    QString ground = object.data.find(ELE_GROUND).value();
    if(ground == "traversable")
        pattern.traversable = true;
    else
        pattern.traversable = false;

    pattern.id =              object.find(ELE_ID).toInt();
    pattern.defaultLayer =    object.find(ELE_DEFAULT_LAYER).toInt();
    pattern.x =               object.find(ELE_X).toInt();
    pattern.y =               object.find(ELE_Y).toInt();
    pattern.width =           object.find(ELE_WIDTH).toInt();
    pattern.height =          object.find(ELE_HEIGHT).toInt();

    return pattern;
}

Tileset::Tileset()
{
    data = nullptr;
}

Tileset::Tileset(Table* data)
{
    this->data = data;

    // Construct the list of patterns from the data
    QList<Object*> patternList = data->getObjectsOfName(OBJ_TILE_PATTERN);
    for(Object* obj : patternList)
        patterns.insert(obj->data.find(ELE_ID).value().toInt(), TilePattern::parse(*obj));
}

Tileset::~Tileset()
{

}

Tileset Tileset::create(QString name, Table* data, int tileSize)
{
    Tileset tileset;

    QImage image; // Load the image!

    int width = image.width()/tileSize;
    int height = image.height()/tileSize;

    int id = 0;

    // Construct the patterns used by this tileset
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            TilePattern pattern;
            pattern.x = x*tileSize;
            pattern.y = y*tileSize;
            pattern.height = pattern.width = tileSize;
            pattern.traversable = true;
            pattern.id = id;
            id++;

            tileset.addPattern(pattern);
        }
    }
}
