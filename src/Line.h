/**
 * @file Line.h
 * @brief This module represents a line of connections.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 *
 * This module represents a line of connections. Aside of being defined by a set
 * streets and stops, this class also computes the waypoint vectors for its connections
 * and is able to implement specified detours in these routes.
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

/**
 * @brief This class represents a line of public transport
 */
class Line {
    unsigned m_number; /**< Line number */
    QColor m_color; /**< Line color */

    std::vector<Stop *> m_stops; /**< An ordered list of stops along the line */
    std::vector<Street *> m_streets; /**< A vector of streets along the line route */
    std::vector<Waypoint> m_route; /**< A vector of waypoints for the line */

    /* The reason for these following vectors is that we wanted to implement a 'restore default'
     * button that would reopen all the closed streets and cancel detours. In that case, we would
     * need to store the default configuration in order to be able to restore it. This feature, however,
     * we didn't manage to implement on time, so these vectors are basically redundant for the time being. */
    std::vector<Stop *> m_stops_current; /**< A working copy of m_stops */
    std::vector<Street *> m_streets_current; /**< A working copy of m_streets */
    std::vector<Waypoint> m_route_current; /**< A working copy of m_route */

public:
    std::vector<Connection *> connections; /**< A vector of all the connections */

    /**
     * @brief References the hash table in the Map class - used for determining
     * the traffic situation on a particular street.*/
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
     * @brief This method takes the stops and the streets defining the line
     * and creates a list of points that define the travel route for the connections.
     */
    void compute_route();

    /**
     * @brief This method implements a detour specified by the user
     * @param closed A pointer to the closed street
     * @param detour The street sequence that specifies the detour
     * @param new_route The sequence of waypoints that define the detour
     */
    void implement_detour(Street *closed, std::vector<Street *> detour, std::vector<Waypoint> new_route);

    /**
     * @brief Add a new stop to the line
     * @param stop Stop object
     */
    void add_stop(Stop *stop);

    /**
     * @brief Add new connection to the line
     * @param conn Connection object
     */
    void add_connection(Connection *conn);

    /**
     * @brief Sets the color the line
     * @param color QColor color
     */
    void set_color(QColor color);

    /**
     * @brief Line number getter
     * @return m_number Returns the number of this line
     */
    unsigned get_line_number() { return this->m_number; }

    /**
     * @brief Line color getter
     * @return m_color Color of this line
     */
    QColor get_color() { return this->m_color; }

    /**
     * @brief Line route getter
     * @return m_route Vector of route Waypoints
     */
    std::vector<Waypoint> get_route() { return this->m_route; }
};

#endif
