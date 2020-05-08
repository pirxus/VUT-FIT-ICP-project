/**
 * @file PublicTransport.h
 * @brief
 * @author
 */

#ifndef PUBLIC_TRANSPORT_H
#define PUBLIC_TRANSPORT_H

#include <vector>
#include <QTimer>
#include <QTime>
#include "Map.h"
#include "Line.h"
#include "Stop.h"

class PublicTransport {
    QTime m_time;
    QTimer *m_timer;
public:
    Map map;
    std::vector<Line *> lines;

    PublicTransport(QObject *parent);
    ~PublicTransport();

	/**
    * @brief loads streets from a csv file
    * @param filename
    */
	void load_map(const char *filename);

    /**
    * @brief loads all stops from a file
    * @param filename
    */
    void load_stops(const char *filename);

    /**
    * @brief load lines and connections from file
    * @param filename
    * @param all_stops vector of all_stops
    */
    void load_lines(const char* filename);

    QTime get_time() { return this->m_time; }
    void set_time(QTime time) { this->m_time = time; } //TODO
    void set_timeout(unsigned timeout); /**< Sets the clock speed */ //TODO

    void delete_lines();

    /**
     * @brief Makes all the vehicles move a notch towards their
     * respective destinations
     */
    void update_vehicles();

    /**
     * @brief prepare_connections Assigns routes for each connection from each line
     */
    void prepare_connections();
};

#endif
