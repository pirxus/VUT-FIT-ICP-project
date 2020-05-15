/**
 * @file Stop.h
 * @brief This module represents a stop.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 *
 * This module represents a stop. Each stop is defined by its name, the street that it
 * lies on and its position.
 */

#ifndef STOP_H
#define STOP_H

#include <string>
#include <map>
#include <QPointF>

/* Forward declarations */
class Street;

/**
 * @brief The Stop class represents one stop.
 */
class Stop {
    std::string m_name; /**< The name of the street */
public:
    QPointF pos; /**< The position of the stop */
    Street *street; /**< A pointer to the street the stop lies on */

	/**
    * @brief Stop constructor
    * @param street The pointer to the parent street
 	* @param name name of the stop
    * @param pos The position on the street relative to the street start
 	*/
    Stop(Street *street, std::string name, float pos);

    /**
    * @brief Stop name getter
    */
    std::string name() {return m_name;}
};

#endif
