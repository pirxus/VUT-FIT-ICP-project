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
        Stop *stop;
        try {
            stop = this->m_stops.at(i);
        } catch (const std::out_of_range& e) {
            stop = nullptr;
        }

        Street *street = this->m_streets.at(j);
        Street *street_next;
        try {
            street_next = this->m_streets.at(j + 1);

        } catch (const std::out_of_range& e) {
            street_next = nullptr;
        }
        if (stop == nullptr) {
            if (j == street_len - 1) {
                return; /* The job is done */
            } else {
                std::cerr<<"Error: The specified line did not end correctly\n";
                return;
            }
        }

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

void Line::implement_detour(Street *closed, std::vector<Street *> detour, std::vector<Waypoint> new_route)
{
    auto it = std::find(m_streets.begin(), m_streets.end(), closed);
    if (it == m_streets.end()) {
        /* Detour does not affect this line... */
        return;
    }

    /* Find the index of the cancelled street */
    unsigned index = std::distance(m_streets.begin(), it);

    /* Edit the original street vector with the detour vector */
    m_streets_current = std::vector<Street *>(m_streets.begin(), m_streets.begin() + index);
    m_streets_current.insert(m_streets_current.end(), detour.begin(), detour.end());
    m_streets_current.insert(m_streets_current.end(), m_streets.begin() + index + 1, m_streets.end());

    /* Edit the m_stops vector - delete all the stops that lie on the closed street */
    std::vector<Stop *>::iterator iter;
    for (iter = m_stops.begin(); iter != m_stops.end(); iter++) {
        if ((*iter)->street == closed) continue;
        m_stops_current.push_back(*iter);
    }


    //TODO waypoints
}

void Line::set_color(QColor color) {
    this->m_color = color;
}
