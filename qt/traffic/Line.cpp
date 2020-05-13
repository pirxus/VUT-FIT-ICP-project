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

    std::vector<unsigned> indices;
    for (unsigned i = 0; i < m_streets.size(); i++) {
        if (m_streets.at(i) == closed) {
            indices.push_back(i);
        }
    }

    if (indices.empty()) return; /* Detour does not affect this line... */

    unsigned last = 0;
    for (unsigned i = 0; i < indices.size(); i++) {
        m_streets_current.insert(m_streets_current.end(), m_streets.begin() + last, m_streets.begin() + indices.at(i));
        /* Do not replace the first and the last street in the original vector*/
        if (indices.at(i) != 0 && indices.at(i) != m_streets.size() -1) {
            m_streets_current.insert(m_streets_current.end(), detour.begin(), detour.end());
        }
        last = indices.at(i) + 1;
    }
    /* Tie up the detour insertion */
    m_streets_current.insert(m_streets_current.end(), m_streets.begin() + last, m_streets.end());

    /* Edit the m_stops vector - delete all the stops that lie on the closed street */
    for (auto *iter : m_stops) {
        if (iter->street == closed) continue;
        m_stops_current.push_back(iter);
    }

    /* If the ending or begining stop lies on the closed street, trim the total route */
    if (m_route.front().stop->street == closed) {
        for (unsigned i = 0; i < m_route.size(); i++) {
            if (m_route.at(i).street != nullptr) {
                m_route = std::vector<Waypoint>(m_route.begin() + i + 1, m_route.end());
                break;
            }
        }
    }

    if (m_route.back().stop->street == closed) {
        for (int i = m_route.size() - 1; i >= 0; i--) {
            if (m_route.at(i).street != nullptr) {
                std::cerr<<m_route.size()<<std::endl;
                m_route = std::vector<Waypoint>(m_route.begin(), m_route.begin() + i);
                std::cerr<<m_route.size()<<std::endl;
                break;
            }
        }
    }

    QPointF w1 = new_route.front().pos;
    QPointF w2 = new_route.back().pos;
    unsigned len = m_route.size();

    /* In the route vector, first detect and delete sequences that make the bus
     * go halfway across affected the street and then come back */
    for (unsigned i = 0; i < len; i++) {
        if (m_route.at(i).pos == w1) {
            for (unsigned j = i + 1; j < len; j++) {
                if (m_route.at(j).pos == w1) {
                    /* Delete the loop */
                    m_route.erase(m_route.begin() + i, m_route.begin() + j);
                    i--;
                    break;

                } else if (m_route.at(j).stop != nullptr) {
                    if (m_route.at(j).stop->street == closed)
                        continue;
                    else
                        break;
                } else {
                    break;
                }
            }
        } else if (m_route.at(i).pos == w2) {
            for (unsigned j = i + 1; j < len; j++) {
                if (m_route.at(j).pos == w2) {
                    /* Delete the loop */
                    m_route.erase(m_route.begin() + i, m_route.begin() + j);
                    i--;
                    break;

                } else if (m_route.at(j).stop != nullptr) {
                    if (m_route.at(j).stop->street == closed)
                        continue;
                    else
                        break;
                } else {
                    break;
                }
            }
        }
    }


    /* Now we need to replace the waypoint sequence that is now unaccessible */
    /* The bool in this vector indicates whether the waypoint sequence should be inserted reversed */
    std::vector<std::tuple<int, int, bool>> index_tuples;
    int i1 = -1;
    int i2 = -1;

    len = m_route.size();
    for (unsigned i = 0; i < len; i++) {
        /* First, find one of the starting points of the closed street */
        if (m_route.at(i).pos == w1) {
            i1 = i;

            /* Now check for the matching end */
            for (unsigned j = i + 1; j < len; j++) {
                /* If the sequence of waypoints is to be removed, only stops can be between the two endpoints */
                if (m_route.at(j).stop != nullptr) {
                    if (m_route.at(j).stop->street == closed) {
                        i2 = j;
                        continue;
                    } else {
                        i1 = -1;
                        i2 = -1;
                        break;
                    }

                } else if (m_route.at(j).pos == w2) {
                    i2 = j;
                    //i = j; // skip a few indexes
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
            for (unsigned j = i + 1; j < len; j++) {
                /* If the sequence of waypoints is to be removed, only stops can be between the two endpoints */
                if (m_route.at(j).stop != nullptr) {
                    if (m_route.at(j).stop->street == closed) {
                        i2 = j;
                        continue;
                    } else {
                        i1 = -1;
                        i2 = -1;
                        break;
                    }

                } else if (m_route.at(j).pos == w1) {
                    i2 = j;
                    //i = j; // skip a few indexes
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

        //std::cerr<<std::get<0>(ii)<<" "<<std::get<1>(ii)<<std::endl;
    }

    /* Tie up the vector */
    m_route_current.insert(m_route_current.end(), m_route.begin() + last_original, m_route.end());


   // for (auto point : m_route) {
   //     std::cerr<<point.pos.x()<<" "<<point.pos.y()<<" ";
   //     if (point.stop == nullptr) {
   //         std::cerr<<"street: "<<point.street->name<<std::endl;
   //     } else {
   //         std::cerr<<"stop: "<<point.stop->name()<<std::endl;
   //     }
   // }

   // std::cerr << std::endl;

   // for (auto point : m_route_current) {
   //     std::cerr<<point.pos.x()<<" "<<point.pos.y()<<" ";
   //     if (point.stop == nullptr) {
   //         std::cerr<<"street: "<<point.street->name<<std::endl;
   //     } else {
   //         std::cerr<<"stop: "<<point.stop->name()<<std::endl;
   //     }
   // }

    m_route = m_route_current;
    /* Set the new route for the connections */
    for (auto *conn : this->connections) {
        /* First delete the schedule entries that contain the affected stops*/
        conn->delete_from_schedule(closed);
        /* Update the waypoints */
        conn->update_route();
    }

}

void Line::set_color(QColor color) {
    this->m_color = color;
}
