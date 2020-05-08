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

/* Forward declarations.. */
class Stop;
class Connection;

class Line {
    unsigned m_number; /* Line number */
    QColor m_color; /* Line color */

    std::list<Stop *> m_stops; /**< An ordered list of stops along the line */
    std::vector<Connection *> m_connections; /**< A vector of all the connections */
    std::vector<Street *> m_route;

public:
    /**
     * @brief Line object constructor
     * @param line_number The line number/id
     * @param color The line color - this will be displayed on the map
     */
    Line(unsigned line_number, QColor color): m_number{line_number}, m_color{color} {}

    void add_stop(Stop *stop);
    void add_connection(Connection *conn);
    void set_color(char color);
};

#endif
