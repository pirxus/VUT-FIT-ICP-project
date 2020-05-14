/**
 * @file Waypoint.h
 * @brief
 * @author
 */

#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <vector>
#include <QPointF>
#include <math.h>

/* Forward declarations */
class Street;
class Stop;

class Waypoint
{
public:
    QPointF pos;
    Street *street;
    Stop *stop;

    Waypoint();
    Waypoint(QPointF pos, Street *street): pos{pos}, street{street}, stop{nullptr} {}
    Waypoint(QPointF pos, Stop *stop): pos{pos}, street{nullptr}, stop{stop} {}

    /**
     * @brief Returns the indes of a stop in the vector of waypoints
     * @param points
     * @param stop
     */
    static int find_stop(std::vector<Waypoint> points, Stop *stop);

    /**
     * @brief Computes the total distance between two waypoints
     * @param points The vector of waypoints we take from
     * @param start The index of the first waypoint
     * @param end The index of the second waypoint
     * @return Return the total distance
     */
    static double segment_len(std::vector<Waypoint> points, unsigned start, unsigned end);

    /**
     * @brief Computes the distance between two waypoints
     * @param first Waypoint
     * @param second Waypoint
     * @return Eucleidean distance between the two waypoints
     */
    static double distance(Waypoint first, Waypoint second);
};

#endif // WAYPOINT_H
