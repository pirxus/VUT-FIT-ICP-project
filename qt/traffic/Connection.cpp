/**
 * @file Connection.cpp
 * @brief
 * @author
 */

#include "Connection.h"

Connection::Connection(Line *line, std::list<Stop *> stops, std::list<QTime> times)
{
    m_line = line;
    /* Create the schedule by merging the stops and times */
    auto stop = stops.begin();
    auto time = times.begin();
    for (; stop != stops.end() && time != times.end(); stop++, time++) {
        m_schedule.push_back(std::pair<Stop *, QTime>(*stop, *time));
    }

    active = 0;
    route_index = 0;
    m_delay = 0;
}

void Connection::set_schedule(std::list<std::pair<Stop *, int>> schedule) {
}
void Connection::add_to_schedule(std::pair<Stop *, int> stop) {
}
void Connection::update_route() {
}
void Connection::update_position(int time) {
}
