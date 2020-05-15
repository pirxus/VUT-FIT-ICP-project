/**
 * @file Connection.cpp
 * @brief This module represents one connection in the system.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 */

#include "Connection.h"
#include "Line.h"
#include "PublicTransport.h"

Connection::Connection(Line *line, std::vector<Stop *> stops, std::vector<unsigned> times)
{
    m_line = line;
    /* Set the connection schedule */
    this->set_schedule(stops, times);
    active = 0;
    m_delay = 0;
}

void Connection::update_route() {
    this->m_route = this->m_line->get_route();
}

void Connection::set_schedule(std::vector<Stop *> stops, std::vector<unsigned> times) {
    /* Create the schedule by merging the stops and times */
    auto stop = stops.begin();
    auto time = times.begin();
    for (; stop != stops.end() && time != times.end(); stop++, time++) {
        m_schedule.push_back(std::tuple<Stop *, unsigned, unsigned>(*stop, *time, 0));
    }
}

int Connection::get_delay(int sch_index)
{
    unsigned stop_delay = 0;
    if (sch_index >= 0) {
        if ((unsigned)sch_index < m_schedule.size() - 1) {
            stop_delay = std::get<2>(m_schedule.at(sch_index + 1));
        }
    }
    return m_delay + stop_delay;
}

void Connection::delete_from_schedule(Street *street)
{
    std::vector<std::tuple<Stop *, unsigned, unsigned>> new_sch;
    for (auto it = m_schedule.begin(); it != m_schedule.end(); it++) {
        if (std::get<0>(*it)->street != street) {
            new_sch.push_back(*it);
        }
    }

    m_schedule = new_sch;
}

void Connection::add_delay_on_stops(std::vector<Stop *> stops, std::vector<unsigned> occurences, unsigned detour_len)
{
    for (unsigned i = 0; i < stops.size(); i++) {
        for (unsigned j = 0; j < m_schedule.size(); j++) {
            if (std::get<0>(m_schedule.at(j)) == stops.at(i))
                occurences.at(i)--;

            /* Add a delay burst on all stops that come afther this one */
            if (occurences.at(i) == 0) {
                std::get<2>(m_schedule.at(j)) += DETOUR_DELAY*detour_len;
            }
        }
    }
}

void Connection::update_position(unsigned time) {
    /* First check whether the current time is within the duty window */

    unsigned start = std::get<1>(this->m_schedule.front());
    unsigned end = std::get<1>(this->m_schedule.back()) + std::get<2>(this->m_schedule.back());

    if (time < start || time - m_delay >= end) {
        this->active = false;
        this->m_delay = 0;

    } else { /* Now compute the actual position */

        this->active = true;
        unsigned sch_index = this->find_schedule_index(time);

        /* Check whether the vehicle should be right at the stop */
        if (std::get<1>(this->m_schedule.at(sch_index)) + std::get<2>(this->m_schedule.at(sch_index)) == time - this->m_delay) {
            this->m_position = std::get<0>(this->m_schedule.at(sch_index))->pos;

        } else { /* Otherwise compute the position */
            /* Find the stops in the m_route vector */
            unsigned s1 = Waypoint::find_stop(this->m_route, std::get<0>(this->m_schedule.at(sch_index)));
            unsigned s2 = Waypoint::find_stop(this->m_route, std::get<0>(this->m_schedule.at(sch_index + 1)));

            /* Compute the current segment total length */
            double segment_len = Waypoint::segment_len(this->m_route, s1, s2);

            /* Compute the 'time' length of this segment including the delay differences */
            unsigned time_dist = (std::get<1>(this->m_schedule.at(sch_index + 1)) + std::get<2>(this->m_schedule.at(sch_index + 1))) -
                    (std::get<1>(this->m_schedule.at(sch_index)) + std::get<2>(this->m_schedule.at(sch_index)));
            unsigned time_pos = time - (std::get<1>(this->m_schedule.at(sch_index)) + std::get<2>(this->m_schedule.at(sch_index)));

            /* Compute the time progress between the stops */
            double progress = ((double)time_pos - m_delay) / ((double)time_dist);
            double dist_progress = segment_len * progress; /* Actual distance progress between stops */

            /* Find the last passed waypoint */
            Waypoint wlast = this->m_route.at(s1);
            unsigned next = s1 + 1;
            unsigned dist_progress_segment = dist_progress;
            for (; next <= s2; next++) {
                double curr_seg = Waypoint::distance(wlast, this->m_route.at(next));
                segment_len = curr_seg;
                if (dist_progress_segment - curr_seg <= 0) {
                    break;
                } else {
                    dist_progress_segment -= curr_seg;
                    wlast = this->m_route.at(next);
                }
            }
            Waypoint wnext = this->m_route.at(next);
            double traffic = this->determine_traffic_situation(wlast, wnext) / 10.0;

            /* Set the vehicle position */
            if (segment_len < 0.01) {
                this->m_position = wnext.pos;
            } else {
                double frac = dist_progress_segment / segment_len;
                if (frac > 1.0) frac = 1.0;

                /* compute the delay */
                this->m_delay += TIME_INCREMENT * traffic;
                this->m_position = (wnext.pos - wlast.pos) * frac + wlast.pos;
            }
        }
    }
}

unsigned Connection::find_schedule_index(unsigned time)
{
    unsigned len = this->m_schedule.size();
    for (unsigned i = 0; i < len; i++) {
        if (std::get<1>(this->m_schedule.at(i)) + std::get<2>(this->m_schedule.at(i)) <= time - this->m_delay) {
            continue;
        } else { /* We have already passed the desired stop */
            return i - 1; /* in this state, i will always be at least 1 */
        }
    }

    return 0; /* Should not happen... TODO exception */
}

unsigned Connection::determine_traffic_situation(Waypoint w1, Waypoint w2)
{
    /* If one of the waypoints is a stop, then finding the current street
     * is easy. */
    if (w1.street == nullptr) {
        return w1.stop->street->get_traffic();

    } else if (w2.street == nullptr) {
        return w2.stop->street->get_traffic();

    } else { /* Both waypoints are street endpoints */
        auto street = m_line->streets_by_endpoints_ref.find(
                    std::tuple<int, int, int, int>(w1.pos.x(), w1.pos.y(), w2.pos.x(), w2.pos.y()));

        if (street == m_line->streets_by_endpoints_ref.end()) {
            /* try switching the begining and end */
            street = m_line->streets_by_endpoints_ref.find(
                    std::tuple<int, int, int, int>(w2.pos.x(), w2.pos.y(), w1.pos.x(), w1.pos.y()));
            if (street != m_line->streets_by_endpoints_ref.end())
                return street->second->get_traffic();
        }
    }
    return 0;
}
