#ifndef QPOINTEXT_H
#define QPOINTEXT_H

#include <QPoint>

// Define a '<' operator for QPoint (so we can store it in a QMap)
static bool operator<(const QPoint& lhs, const QPoint& rhs)
{
    if(lhs.x() < rhs.x())
        return true;
    else if (lhs.x() == rhs.x())
        return lhs.y() < rhs.y();
    else
        return false;
}


#endif // QPOINTEXT_H
