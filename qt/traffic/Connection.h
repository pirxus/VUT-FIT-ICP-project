/**
 * @file Connection.h
 * @brief
 * @author
 */

#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <QPoint>
#include <QTime>

/* Forward declarations.. */
class Stop;
class Crossroads;
class Line;

class Connection {
    Line *m_line; /* parent line of the connection */

    /* Stop - time pairs defining the base schedule for this connection */
    std::list<std::pair<Stop *, QTime>> m_schedule; //TODO time

    /* A list of waypoints for the connection - one of the two pair members is always
     * a nullptr */
    std::list<std::pair<Crossroads *, Stop *>> m_route;

    unsigned route_index; /**< The upcoming stop/crossroads in the m_route list */
    unsigned m_delay; /**< The current delay of the connection */

public:
    QPoint position; /**< The current position of the vehicle on the map */
    bool active; /**< Indicates, whether the vehicle is on duty */

    /** Connection constructors */
    Connection(Line *line, std::list<Stop *> stops, std::list<QTime> times);

    std::list<std::pair<Stop *, QTime>> get_schedule() { return this->m_schedule; }
    void set_schedule(std::list<std::pair<Stop *, int>> schedule);
    void add_to_schedule(std::pair<Stop *, int> stop);
    void update_route();
    void update_position(int time);

    // refernece - open to alterations...
    std::list<std::pair<Crossroads *, Stop *>> *get_route() {
        return &this->m_route;
    }
    unsigned get_delay() { return this->m_delay; }
    QPoint get_position() { return this->position; }
};

#endif
