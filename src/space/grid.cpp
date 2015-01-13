#include "grid.h"

Grid::Grid()
{
    cells = QVector<QVector<GridCell>>();
    areas = QList<Area>();
}

void Grid::placeArea(Area area)
{
    // Only allow this area to exist once within the grid (remove any existing instances)
    if(areas.contains(area))
        removeArea(area);

    // Ensure there is enough room in the grid for the new area.
    while(cells.length() < area.getLocation().x() + area.getWidth())
    {
        cells.append(QVector<GridCell>());
        while(cells.last().length() < area.getLocation().y() + area.getHeight())
            cells.last().append(GridCell());
    }

    for(int i = area.getLocation().x(); i < area.getWidth(); i++)
    {
        for(int j = area.getLocation().y(); j < area.getHeight(); j++)
        {
            // Remove any areas in the way of this one
            if(cells[i][j].getArea() != nullptr)
                removeArea(i, j);

            cells[i][j] = GridCell(&area);
        }
    }

    areas.append(area);
}

bool Grid::removeArea(int x, int y)
{
    if(cells[x][y].getArea() != nullptr)
    {
        Area* area = cells[x][y].getArea();

        for(int i = x; i < area->getWidth(); i++)
        {
            for(int j = y; j < area->getHeight(); j++)
            {
                cells[i][j].removeArea();
            }
        }

        areas.removeOne(*area);

        return true;
    }

    return false;
}

bool Grid::removeArea(Area area)
{
    if(areas.contains(area))
    {
        if(*cells[area.getLocation().x()][area.getLocation().y()].getArea() == area) // Failsafe to check if the area is in the correct location
        {
            for(int x = area.getLocation().x(); x < area.getWidth(); x++)
            {
                for(int y = area.getLocation().y(); y < area.getHeight(); y++)
                {
                    cells[x][y].removeArea();
                }
            }

            areas.removeOne(area);
        }
        else
            return false;
    }
    else
        return false;
}

GridCell* Grid::getCell(int x, int y)
{
    if(x < cells.length() && y < cells[x].length())
        return &cells[x][y];
}
