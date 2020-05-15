/**
 * @file Street.h
 * @brief This module contains a class that represents a street on the map.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 *
 * This module contains a class that represents a street on the map. Each street is
 * defined by its unique name and a set of endpoints. The class also holds information
 * about the current traffic situation on the street and a vector of pointer to stops
 * that lie on this particular street.
 */

#ifndef STREET_H
#define STREET_H

#include <vector>
#include <string>

#include <QPoint>

/* Forward declarations */
class Stop;

/**
 * @brief The Street class represents a street in the system.
 */
class Street {

    std::vector<Stop *> m_stops; /**< A vector of stops that lie on the Street */

    int m_traffic; /**< The current traffic situation on the street */

    bool m_closed; /**< Open/closed indicator */

public:
    QPoint start; /**< The first coordinate */
    QPoint end; /**< The second coordinate */
    std::string name; /**< Street name */

    /**
    * @brief Street constructor
    * @param name name of the street
    * @param x1 the street start x coordinate street start
    * @param y1 the street start y coordinate
    * @param x2 the street end x coordinate street start
    * @param y2 the street end y coordinate
    */
    Street(std::string name, int x1, int y1, int x2, int y2);

    /**
    * @brief Gets the traffic situation.
    * @return m_traffic Current traffic situation the this street.
    */
    int get_traffic() { return this->m_traffic; }

    /**
    * @brief Sets the traffic situation.
    * @param traffic
    */
    void set_traffic(int traffic) { this->m_traffic = traffic; }

    /**
     * @brief Closes the street
     */

    void close_street() { m_closed = true; }

    /**
     * @brief Opens the street
     */
    void open_street() { m_closed = false; }

    /**
     * @brief Checks if the street is closed
     * @return True if closed, otherwise false
     */
    bool closed() { return m_closed; }
};

#endif
