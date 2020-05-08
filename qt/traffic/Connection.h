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

/* Forward declarations.. */
class Stop;
class Crossroads;
class Line;

class Connection {
    Line *m_line; /* parent line of the connection */

    /* Stop - time pairs defining the base schedule for this connection */
    std::list<std::pair<Stop *, int>> m_schedule; //TODO time

    /* A list of waypoints for the connection - one of the two pair members is always
     * a nullptr */
    std::list<std::pair<Crossroads *, Stop *>> m_route;

    unsigned m_delay;

public:
    QPoint position;

    /** Connection constructors */
    Connection(Line *line): m_line{line}, m_delay{0} {}
    Connection(Line *line, std::list<std::pair<Stop *, int>> schedule):
        m_line{line}, m_schedule{schedule}, m_delay{0} {}

    void set_schedule(std::list<std::pair<Stop *, int>> schedule);
    void add_to_schedule(std::pair<Stop *, int> stop);
    void update_route();
    void update_position(int time);

    // refernece - open to alterations...
    std::list<std::pair<Crossroads *, Stop *>> *get_route() {
        return &this->m_route;
    }
    std::list<std::pair<Stop *, int>> get_schedule() { return this->m_schedule; }
    unsigned get_delay() { return this->m_delay; }
    QPoint get_position() { return this->position; }
};

#endif
