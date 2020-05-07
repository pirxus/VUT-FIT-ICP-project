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
    * @brief calls load lines and connections from file
    * @param filename
    */
	void load_lines(const char *filename);
};

#endif
