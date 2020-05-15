/**
 * @file Waypoint.h
 * @brief This module represents a single waypoint on the route of a connection.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 *
 * This module represents a single waypoint on the route of a connection. These
 * waypoints are chained into a vector and define the current route for a connection or
 * line. The Waypoint can either refer to a stop or an endpoint of a street.
 */

#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <vector>
#include <QPointF>
#include <math.h>

/* Forward declarations */
class Street;
class Stop;

/**
 * @brief The Waypoint class represents one waypoint (a stop or a street endpoint) from which
 * the Line class then constructs routes for the connections.
 */
class Waypoint
{
public:
    QPointF pos; /**< Waypoint position */
    Street *street; /**< Is this a street endpoint? */
    Stop *stop; /**< Is the waypoint a stop? */

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
