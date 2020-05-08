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
#include <QColor>
#include "Street.h"
#include "Stop.h"
#include "Waypoint.h"

/* Forward declarations.. */
class Connection;

class Line {
    unsigned m_number; /* Line number */
    QColor m_color; /* Line color */

    std::vector<Stop *> m_stops; /**< An ordered list of stops along the line */
    std::vector<Street *> m_streets; /**< A vector of streets along the line route */
    std::vector<Waypoint> m_route; /**< A vector of waypoints for the line */

public:
    std::vector<Connection *> connections; /**< A vector of all the connections */
    /**
     * @brief Line object constructor
     * @param line_number The line number/id
     * @param color The line color - this will be displayed on the map
     */
    Line(unsigned line_number, QColor color, std::vector<Stop *> stops, std::vector<Street *> streets):
        m_number{line_number}, m_color{color}, m_stops{stops}, m_streets{streets} {}

    /**
     * @brief compute_route This method takes the stops and the streets defining the line
     * and creates a list of points that define the travel route for the connections.
     */
    void compute_route();


    void add_stop(Stop *stop);
    void add_connection(Connection *conn);
    void set_color(QColor color);
    QColor get_color() { return this->m_color; }
    std::vector<Waypoint> get_route() { return this->m_route; }
};

#endif
