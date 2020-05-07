/**
 * @file Street.h
 * @brief
 * @author
 */

#ifndef STREET_H
#define STREET_H

#include <vector>
#include <string>

#include "Coords.h"

/* Forward declarations */
class Stop;

class Street {
    Coords m_start;
    Coords m_end;
    std::string m_name;

    /* A vector of stops that lie on the Street */
    std::vector<Stop *> m_stops;

    /* The current traffic state on the street */
    int m_traffic;

public:
    Street(): m_traffic{0} {}
    bool lies_on_street(Coords point);
    void add_stop(Stop *stop);
    
    /**
    * @brief Street constructor
    * @param name name of the street
    * @param x1 the street start x coordinate street start
    * @param y1 the street start y coordinate
    * @param x2 the street end x coordinate street start
    * @param y2 the street end y coordinate
    */
    Street(std::string name, int x1, int y1, int x2, int y2);
};

#endif
