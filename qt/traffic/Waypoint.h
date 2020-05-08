#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QPointF>

/* Forward declarations */
class Street;
class Stop;

class Waypoint
{
public:
    QPointF pos;
    Street *street;
    Stop *stop;

    Waypoint(QPointF pos, Street *street): pos{pos}, street{street}, stop{nullptr} {}
    Waypoint(QPointF pos, Stop *stop): pos{pos}, street{nullptr}, stop{stop} {}
};

#endif // WAYPOINT_H
