/**
 * @file Connection.h
 * @brief This module represents one connection in the system.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 *
 * This module represents one connection in the system. Each connection stores a pointer
 * to its parent line and has its own schedule, which allows us to compute the position
 * of the particular connection given a time value.
 */

#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <QPoint>
#include <QTime>
#include "Street.h"
#include "Waypoint.h"

#define DETOUR_DELAY 240 /**< Base detour delay increment */

/* Forward declarations.. */
class Stop;
class Crossroads;
class Line;

/**
 * @brief The Connection class, represents one connection in the public
 * transport system
 */
class Connection {
    Line *m_line; /**< The parent line of the connection */

    /**
     * @brief Stop - time - delay tuples defining the base schedule for this connection */
    std::vector<std::tuple<Stop *, unsigned, unsigned>> m_schedule;

    /** @brief A list of waypoints for the connection */
    std::vector<Waypoint> m_route;

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
     * @brief Returns an index to the schedule
     * of the connection which points to the stop the vehicle has passed last
     * @param time The time the index should correspond to
     * @return Returns an index to the connection schedule
     */
     unsigned find_schedule_index(unsigned time);

     /**
      * @brief This function takes two waypoints in between
      * which the vehicle is present and determines the current street the vehicle is on.
      * @param w1 The first waypoint
      * @param w2 The second waypoint
      * @return Returns the traffic situation coefficient on the street
      */
     unsigned determine_traffic_situation(Waypoint w1, Waypoint w2);

    /**
     * @brief Sets the schedule for this connection. This method
     * is only used by the constructor and the schedules should not be altered
     * @param stops The list of stops on the route
     * @param times The timetable corresponding to the stops
     */
    void set_schedule(std::vector<Stop *> stops, std::vector<unsigned> times);

    /**
     * @brief Returns the schedule of this connection.
     */
    std::vector<std::tuple<Stop *, unsigned, unsigned>> get_schedule() { return this->m_schedule; }

    /**
     * @brief Returns current delay of this connection.
     */
    int get_delay(int sch_index);

    /**
     * @brief Returns the current position of the connection
     */
    QPointF get_pos() { return this->m_position; }

    /**
     * @brief Returns a pointer to the parent line of the connection
     */
    Line *get_line() { return this->m_line; }

    /**
     * @brief Deletes all schedule entries for stops that lie on the
     * specified street
     * @param street The street that was closed and its stops need to be eliminated
     * from the connection schedule
     */
    void delete_from_schedule(Street *street);

    /**
     * @brief When specifying a detour, this function adds delay on all stops after a specified occurence
     * of a stop in order to compensate for the detour. The amount of delay added corresponds to the length
     * of the detour segment
     * @param stops A vector of stops that are located right after a detour segment
     * @param occurences Specifies how many stops, whose name is specified by the stops vector should be skipped
     * before adding delay
     * @param detour_len Specifies the length of the detour segment.
     */
    void add_delay_on_stops(std::vector<Stop *> stops, std::vector<unsigned> occurences, unsigned detour_len);
};

#endif
