/**
 * @file Line.cpp
 * @brief
 * @author
 */

#include "Line.h"
#include "Stop.h"

void Line::compute_route()
{
    unsigned i = 0, j = 0;
    unsigned stop_len = this->m_stops.size();
    unsigned street_len = this->m_streets.size();

    while(i < stop_len || j < street_len) {
        Stop *stop = this->m_stops.at(i);
        Street *street = this->m_streets.at(j);
        Street *street_next = this->m_streets.at(j + 1);

        /* Check if the currently processed stop lies on the current street */
        if (stop->street == street) {

            /* The current stop lies on the current street, try next stop */
            this->m_route.push_back(Waypoint(stop->pos, stop));
            i++;

        } else {
            /* The current stop is not present on the current street,
             * skip to the next street */
            if (j < street_len - 1) {

                /* Find out which street point is the next in the sequence */
                if ((street->start == street_next->start)
                        || (street->start == street_next->end)) {

                    this->m_route.push_back(Waypoint(street->start, street));

                } else if ((street->end == street_next->start)
                        || (street->end == street_next->end)) {

                    this->m_route.push_back(Waypoint(street->end, street));

                } else {
                    //TODO ERROR CODE
                    std::cerr<<"Error: The specified line did not have all the streets connected\n";
                    return;
                }

            } else if (i != stop_len) {
                /* There was a trailing street in the line specification... */
                //TODO ERROR CODE
                std::cerr<<"Error: The specified line did not end correctly\n";
                return;
            }
            j++;
        }
    }
}

void Line::set_color(QColor color) {
    this->m_color = color;
}
