/**
 * @file Line.h
 * @brief
 * @author
 */

#ifndef LINE_H
#define LINE_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <algorithm>
#include <tuple>
#include <QColor>
#include "Street.h"
#include "Stop.h"
#include "Waypoint.h"
#include "Connection.h"

/* Forward declarations.. */
class Connection;

class Line {
    unsigned m_number; /* Line number */
    QColor m_color; /* Line color */

    std::vector<Stop *> m_stops; /**< An ordered list of stops along the line */
    std::vector<Street *> m_streets; /**< A vector of streets along the line route */
    std::vector<Waypoint> m_route; /**< A vector of waypoints for the line */

    std::vector<Stop *> m_stops_current; /**< A working copy of m_stops */
    std::vector<Street *> m_streets_current; /**< A working copy of m_streets */
    std::vector<Waypoint> m_route_current; /**< A working copy of m_route */


public:
    std::vector<Connection *> connections; /**< A vector of all the connections */

    /** References the hash table in the Map class */
    const std::map<std::tuple<int, int, int, int>, Street *> &streets_by_endpoints_ref;
    /**
     * @brief Line object constructor
     * @param line_number The line number/id
     * @param color The line color - this will be displayed on the map
     */
    Line(unsigned line_number, QColor color, std::vector<Stop *> stops,
         std::vector<Street *> streets, const std::map<std::tuple<int, int, int, int>, Street *> &street_map):
        m_number{line_number}, m_color{color}, m_stops{stops}, m_streets{streets}, streets_by_endpoints_ref{street_map} {}

    /**
     * @brief compute_route This method takes the stops and the streets defining the line
     * and creates a list of points that define the travel route for the connections.
     */
    void compute_route();

    void implement_detour(Street *closed, std::vector<Street *> detour, std::vector<Waypoint> new_route);

    /**
     * @brief add_stops Add a new stop to the
     * @param stop Stop object
     */
    void add_stop(Stop *stop);

    /**
     * @brief add_connection Add new connection to the
     * @param conn Connection object
     */
    void add_connection(Connection *conn);

    /**
     * @brief set_color Sets color the the line
     * @param color QColor color
     */
    void set_color(QColor color);

    /**
     * @brief get_line_number Sets color the the line
     * @return m_number Number of this line
     */
    unsigned get_line_number() { return this->m_number; }

    /**
     * @brief get_color Gets color the line
     * @return m_color Color of this line
     */
    QColor get_color() { return this->m_color; }

    /**
     * @brief get_route Gets r the line
     * @return m_route Vector of route Waypoints
     */
    std::vector<Waypoint> get_route() { return this->m_route; }
};

#endif
