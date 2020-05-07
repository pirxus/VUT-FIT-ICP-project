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
    //constructor declaration 
    Street(string name, int x1, int y1, int x2, int y2);

    //bool is_correct_street();

    bool is_vertical();
    bool is_horizontal();
};

#endif
