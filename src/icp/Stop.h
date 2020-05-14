/**
 * @file Stop.h
 * @brief
 * @author
 */

#ifndef STOP_H
#define STOP_H

#include <string>
#include <map>
#include <QPointF>

/* Forward declarations */
class Street;

class Stop {
    std::string m_name;
public:
    QPointF pos;
    Street *street;

	/**
    * @brief Stop constructor
    * @param street The pointer to the parent street
 	* @param name name of the stop
    * @param pos The position on the street relative to the street start
 	*/
    Stop(Street *street, std::string name, float pos);

    /**
    * @brief name returns name of the stop
    */
    std::string name() {return m_name;}
};

#endif
