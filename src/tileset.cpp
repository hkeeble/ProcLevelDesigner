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

    pattern.id =              object.find(ELE_ID).toInt();
    pattern.defaultLayer =    object.find(ELE_DEFAULT_LAYER).toInt();
    pattern.x =               object.find(ELE_X).toInt();
    pattern.y =               object.find(ELE_Y).toInt();
    pattern.width =           object.find(ELE_WIDTH).toInt();
    pattern.height =          object.find(ELE_HEIGHT).toInt();
    pattern.traversable =     object.find(ELE_GROUND, "traversable") == "traversable" ? true : false;

    return pattern;
}

Object TilePattern::build()
{
    Object obj = Object();

    obj.insert(ELE_ID, QString::number(id));
    obj.insert(ELE_DEFAULT_LAYER, QString::number(defaultLayer));
    obj.insert(ELE_X, QString::number(x));
    obj.insert(ELE_Y, QString::number(y));
    obj.insert(ELE_WIDTH, QString::number(width));
    obj.insert(ELE_HEIGHT, QString::number(height));

    if(traversable)
        obj.insert(ELE_GROUND, traversable ? "traversable" : "wall");

    return obj;
}

Tileset::Tileset()
{
    data = nullptr;
}

Tileset::Tileset(QString name, Table* data)
{
    this->data = data;
    this->name = name;

    QString imgPath = QFileInfo(data->getFilePath()).dir().absolutePath() + QDir::separator() + name + ".tiles.png";
    this->image = QPixmap(imgPath);

    // Construct the list of patterns from the data
    QList<Object*> patternList = data->getObjectsOfName(OBJ_TILE_PATTERN);
    for(Object* obj : patternList)
        patterns.insert(obj->data.find(ELE_ID).value().toInt(), TilePattern::parse(*obj));
}

Tileset::~Tileset()
{

}

Tileset Tileset::create(QString name, QString filePath, Table* data, int tileSize)
{
    Tileset tileset;
    tileset.setName(name);

    QPixmap image = QPixmap(filePath); // Load the image!

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
            data->addObject(OBJ_TILE_PATTERN, pattern.build());
        }
    }

    data->saveToDisk();
    return tileset;
}
