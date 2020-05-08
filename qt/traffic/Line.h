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
#include "Crossroads.h"

/* Forward declarations.. */
class Connection;

class Line {
    unsigned m_number; /* Line number */
    QColor m_color; /* Line color */

    std::list<Stop *> m_stops; /**< An ordered list of stops along the line */
    //std::list<Crossroads *, Stop *> m_route;

public:
    std::vector<Connection *> connections; /**< A vector of all the connections */
    /**
     * @brief Line object constructor
     * @param line_number The line number/id
     * @param color The line color - this will be displayed on the map
     */
    Line(unsigned line_number, QColor color, std::list<Stop *> stops):
        m_number{line_number}, m_color{color}, m_stops{stops} {}

    void add_stop(Stop *stop);
    void add_connection(Connection *conn);
    void set_color(QColor color);
    QColor get_color() { return this->m_color; }
};

#endif
