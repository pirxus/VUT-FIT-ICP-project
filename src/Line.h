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

/* Forward declarations.. */
class Stop;
class Connection;

class Line {
    unsigned m_line_no; /* Line number */
    char m_color; /* Line color */

    /* An ordered list of stops along the line */
    std::list<Stop *> m_stops;
    std::vector<Connection *> m_connections; /* A vector of all connections on the line */

public:
    Line(): m_line_no{0}, m_color{0} {}
    Line(unsigned line_number, char color): m_line_no{line_number}, m_color{color} {}

    void add_stop(Stop *stop);
    void add_connection(Connection *conn);
    void set_color(char color);
    
    /**
    * @brief load lines and connections from file
    * @param filename
    */
    void load_lines(const char* filename);
};

#endif
