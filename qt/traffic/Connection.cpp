/**
 * @file Connection.cpp
 * @brief
 * @author
 */

#include "Connection.h"
#include "Line.h"

Connection::Connection(Line *line, std::vector<Stop *> stops, std::vector<unsigned> times)
{
    m_line = line;
    /* Set the connection schedule */
    this->set_schedule(stops, times);
    active = 0;
    m_route_index = 0;
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
        m_schedule.push_back(std::pair<Stop *, unsigned>(*stop, *time));
    }
}

void Connection::update_position(unsigned time) {
    /* First check whether the current time is within the duty window */
    if (time < this->m_schedule.front().second || time > this->m_schedule.back().second - m_delay) {
        this->active = false;
        this->m_delay = 0;
        this->m_route_index = 0;

    } else { /* Now compute the actual position */

        this->active = true;
        unsigned sch_index = this->find_schedule_index(time);

        /* Check whether the vehicle should be right at the stop */
        if (this->m_schedule.at(sch_index).second == time - this->m_delay) {
            this->m_position = this->m_schedule.at(sch_index).first->pos; //TODO delay???

        } else { /* Otherwise compute the position */
            /* Find the stops in the m_route vector */
            unsigned s1 = Waypoint::find_stop(this->m_route, this->m_schedule.at(sch_index).first);
            unsigned s2 = Waypoint::find_stop(this->m_route, this->m_schedule.at(sch_index + 1).first);

            /* Compute the current segment total length */
            double segment_len = Waypoint::segment_len(this->m_route, s1, s2);

            /* Compute the 'time' length of this segment */
            unsigned time_dist = this->m_schedule.at(sch_index + 1).second -
                    this->m_schedule.at(sch_index).second;
            unsigned time_pos = time - this->m_schedule.at(sch_index).second; /* normalize current time */

            /* Compute the time progress between the stops */
            double progress = ((double)time_pos - m_delay) / ((double)time_dist);
            double dist_progress = segment_len * progress; /* Actual distance progress between stops */

            /* Find the last passed waypoint */
            Waypoint last = this->m_route.at(s1);
            unsigned next = s1 + 1;
            for (unsigned i = next; i < s2; i++, next++) {
                double curr_seg = Waypoint::distance(last, this->m_route.at(i));
                if (dist_progress - curr_seg <= 0) {
                    break;
                } else {
                    dist_progress -= curr_seg;
                    segment_len -= curr_seg;
                    last = this->m_route.at(i);
                }
            }

            /* TODO, traffic */
            /* Set the vehicle position */
            double frac = dist_progress / segment_len;
            this->m_position = (this->m_route.at(next).pos - last.pos) * frac + last.pos;
            std::cerr << this->m_position.x() <<" "<<this->m_position.y()<<std::endl;
        }
    }
}

unsigned Connection::find_schedule_index(unsigned time)
{
    auto sch = this->m_schedule;
    unsigned len = sch.size();
    for (unsigned i = 0; i < len; i++) {
        if (sch.at(i).second <= time - this->m_delay) {
            continue;
        } else { /* We have already passed the desired stop */
            return i - 1; /* in this state, it will always be at least 1 */
        }
    }

    return 0; /* Should not happen... TODO exception */
}
