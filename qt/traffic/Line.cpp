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

    /* Now we need to replace the waypoint sequence that is now unaccessible */
    QPointF w1 = new_route.front().pos;
    QPointF w2 = new_route.back().pos;
    /* The bool in this vector indicates whether the waypoint sequence should be inserted reversed */
    std::vector<std::tuple<int, int, bool>> index_tuples;
    int i1 = -1;
    int i2 = -1;
    unsigned len = m_route.size();

    for (unsigned i = 0; i < len; i++) {
        /* First, find one of the starting points of the closed street */
        if (m_route.at(i).pos == w1) {
            i1 = i;

            /* Now check for the matching end */
            for (unsigned j = i; j < len; j++) {
                /* If the sequence of waypoints is to be removed, only stops can be between the two endpoints */
                if (m_route.at(j).stop != nullptr) {
                    continue;

                } else if (m_route.at(j).pos == w2) {
                    i2 = j;
                    i = j; // skip a few indexes
                    break;

                } else {
                    i1 = -1;
                    i2 = -1;
                    break;
                }
            }

            if (i2 != -1) /* Save the found coordinates */
                index_tuples.push_back(std::tuple<int, int, bool>(i1, i2, false));


        /* Now check for the other direction */
        } else if (m_route.at(i).pos == w2) {
            i1 = i;

            /* Now check for the matching end */
            for (unsigned j = i; j < len; j++) {
                /* If the sequence of waypoints is to be removed, only stops can be between the two endpoints */
                if (m_route.at(j).stop != nullptr) {
                    continue;

                } else if (m_route.at(j).pos == w1) {
                    i2 = j;
                    i = j; // skip a few indexes
                    break;

                } else {
                    i1 = -1;
                    i2 = -1;
                    break;
                }
            }

            if (i2 != -1) /* Save the found coordinates *reversed* */
                index_tuples.push_back(std::tuple<int, int, bool>(i1, i2, true));
        }

        i1 = i2 = -1;
    }

    /* Create a reversed copy of new_route */
    std::vector<Waypoint> new_route_reversed(new_route);
    std::reverse(new_route_reversed.begin(), new_route_reversed.end());
    int last_original = 0;
    /* Now replace the waypoints in between the indexes */
    for (auto ii : index_tuples) {
        m_route_current.insert(m_route_current.end(), m_route.begin() + last_original, m_route.begin() + std::get<0>(ii));

        if (std::get<2>(ii) == false)
            m_route_current.insert(m_route_current.end(), new_route.begin(), new_route.end());
        else
            m_route_current.insert(m_route_current.end(), new_route_reversed.begin(), new_route_reversed.end());

        last_original = std::get<1>(ii) + 1;
    }

    /* Tie up the vector */
    m_route_current.insert(m_route_current.end(), m_route.begin() + last_original, m_route.end());
}

void Line::set_color(QColor color) {
    this->m_color = color;
}
