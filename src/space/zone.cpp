#include "zone.h"

Zone::Zone()
{
    tileset = nullptr;
}

Zone::Zone(int areaCount)
    : Zone()
{
    this->areaCount = areaCount;
}

Zone::Zone(Tileset* tileset)
    : Zone()
{
    this->tileset = tileset;
}

Zone::Zone(Tileset* tileset, int areaCount)
    : Zone()
{
    this->tileset = tileset;
    this->areaCount = areaCount;
}

Zone::Zone(Tileset* tileset, int areaCount, QList<Area> areas)
    : Zone(tileset, areaCount)
{

}

Zone Zone::Parse(Table* data)
{

}

void Zone::build(Table* data)
{

}

Zone::~Zone()
{

}
