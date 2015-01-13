#ifndef GRID_H
#define GRID_H

#include "area.h"


/*!
 * \brief Each cell contains a reference to the area contained within it. Null cells contain no area.
 */
class GridCell
{
public:
    GridCell() : area(nullptr) { }
    GridCell(Area* area) : area(area) { }

    inline Area* getArea() { return area; }

    inline void removeArea() { area = nullptr; }

private:
    Area* area;
};

/*!
 * \brief Represents the grid held by space. Each cell contains a reference to an area.
 */
class Grid
{
public:
    Grid();

    /*!
     * \brief Places a new area at the given location in the grid.
     * \param x The X location of the top-left corner of the area.
     * \param y The Y location of the top-left corner of the area.
     */
    void placeArea(Area area);

    /*!
     * \brief Removes an area from the location given. If no area is found, returns false.
     * \param x The X location of the top-left corner of the area to remove.
     * \param y The Y location of the top-left corner of the area to remove.
     */
    bool removeArea(int x, int y);

    /*!
     * \brief Removes an area equivalent to the area given. If no area is removed, returns false.
     * \param The area to remove.
     */
    bool removeArea(Area area);

    /*!
     * \brief Retrieves the cell at the given location.
     */
    GridCell* getCell(int x, int y);

    inline QList<Area>* getAreas() { return &areas; }

private:
    QVector<QVector<GridCell>> cells;
    QList<Area> areas;
};

#endif // GRID_H
