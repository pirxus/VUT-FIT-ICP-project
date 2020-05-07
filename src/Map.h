/**
 * @file Map.h
 * @brief
 * @author
 */

#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Street.h"
#include "Crossroads.h"

class Map {
    std::vector<Street *> m_streets;
    std::vector<Crossroads *> m_cross;
    std::vector<Stops *> m_allstops;

public:
	/**
	* @brief load streets from file
 	* @param filename
 	*/
	void load_streets(const char *filename);

	/**
	* @brief load allstops from file
 	* @param filename
 	*/
	void load_allstops(const char *filename);

};

#endif
