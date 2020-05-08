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

class PublicTransport {
    Map m_map;
    std::vector<Line *> m_lines;
    int time;
public:

	/**
    * @brief load streets and load allstops functions
    * @param filename
    */
	void load_map(const char *filename);

    /**
    * @brief load lines and connections from file
    * @param filename
    * @param all_stops vector of all_stops
    */
	void load_alllines(const char *filename, std::vector<Stop *> all_stops);
};

#endif
