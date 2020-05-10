/**
 * @file Street.h
 * @brief
 * @author
 */

#ifndef STREET_H
#define STREET_H

#include <vector>
#include <string>

#include <QPoint>

/* Forward declarations */
class Stop;

class Street {

    /* A vector of stops that lie on the Street */
    std::vector<Stop *> m_stops;

    /* The current traffic situation on the street */
    int m_traffic;

    bool m_closed;

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

    bool lies_on_street(QPoint point);

    int get_traffic() { return this->m_traffic; }
    void set_traffic(int traffic) { this->m_traffic = traffic; }
    void close_street() { m_closed = true; }
    void open_street() { m_closed = false; }
};

#endif
