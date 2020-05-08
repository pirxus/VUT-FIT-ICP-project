/**
 * @file PublicTransport.h
 * @brief
 * @author
 */

#ifndef PUBLIC_TRANSPORT_H
#define PUBLIC_TRANSPORT_H

#include <vector>
#include "Map.h"
#include "Line.h"
#include "Stop.h"

class PublicTransport {
    std::vector<Line *> m_lines;
    int time;
public:
    Map map;

    PublicTransport(): map{Map()} {}

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
    //void load_lines(const char* filename, std::vector<Stop * > all_stops);
};

#endif
