/**
 * @file Connection.cpp
 * @brief
 * @author
 */

#include "Connection.h"
#include "Line.h"

Connection::Connection(Line *line, std::vector<Stop *> stops, std::vector<QTime> times)
{
    m_line = line;
    /* Set the connection schedule */
    this->set_schedule(stops, times);
    active = 0;
    route_index = 0;
    m_delay = 0;
}

void Connection::update_route() {
    this->m_route = this->m_line->get_route();
}

void Connection::set_schedule(std::vector<Stop *> stops, std::vector<QTime> times) {
    /* Create the schedule by merging the stops and times */
    auto stop = stops.begin();
    auto time = times.begin();
    for (; stop != stops.end() && time != times.end(); stop++, time++) {
        m_schedule.push_back(std::pair<Stop *, QTime>(*stop, *time));
    }
}

void Connection::update_position(QTime time) {
}
