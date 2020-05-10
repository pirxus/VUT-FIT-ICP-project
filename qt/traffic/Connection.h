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
#include "Street.h"
#include "Waypoint.h"

/* Forward declarations.. */
class Stop;
class Crossroads;
class Line;

class Connection {
    Line *m_line; /**< The parent line of the connection */

    /**
     * @brief Stop - time pairs defining the base schedule for this connection */
    std::vector<std::pair<Stop *, unsigned>> m_schedule;

    /** @brief A list of waypoints for the connection - the first element is the next coordinate
     * on the route and the second and the third point to a stop/street or a nullptr */
    std::vector<Waypoint> m_route;

    unsigned m_route_index; /**< The upcoming stop/crossroads in the m_route list */
    double m_delay; /**< The current delay of the connection */
    QPointF m_position; /**< The current position of the vehicle on the map */

public:
    bool active; /**< Indicates, whether the vehicle is on duty */

    /**< Connection constructors */
    Connection(Line *line, std::vector<Stop *> stops, std::vector<unsigned> times);

    /**
     * @brief Fetches the route from the parent line and updates m_route */
    void update_route();

    /**
     * @brief Updates the position of the connection relative to the current time and
     * connection delay
     * @param time The time point to jump to
     */
    void update_position(unsigned time);

    /**
     * @brief find_route_segment Returns an index to the schedule
     * of the connection which points to the stop the vehicle has passed last
     * @param time The time the index should correspond to
     */
     unsigned find_schedule_index(unsigned time);

     /**
      * @brief determine_traffic_situation This function takes two waypoints in between
      * which the vehicle is present and determines the current street the vehicle is on.
      * Then the traffic situation on that street is retruned.
      * @param w1
      * @param w2
      */
     unsigned determine_traffic_situation(Waypoint w1, Waypoint w2);


    /**
     * @brief set_schedule Sets the schedule for this connection. This method
     * is only used by the constructor and the schedules should not be altered
     * @param stops The list of stops on the route
     * @param times The timetable corresponding to the stops
     */
    void set_schedule(std::vector<Stop *> stops, std::vector<unsigned> times);
    std::vector<std::pair<Stop *, unsigned>> get_schedule() { return this->m_schedule; }

    // refernece - open to alterations...
    std::vector<Waypoint> *get_route() { return &this->m_route; }
    int get_delay() { return this->m_delay; }
    QPointF get_pos() { return this->m_position; }
    Line *get_line() { return this->m_line; }
};

#endif
