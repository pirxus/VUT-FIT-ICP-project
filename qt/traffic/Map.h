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
    std::vector<Crossroads *> m_cross; /**< A vector of all crossroads on the map */
    std::vector<Stop *> m_allstops;

public:
    std::vector<Street *> streets; /**< A vector of all the streets on the map */

    /**
	* @brief load streets from file
 	* @param filename
 	*/
	void load_streets(const char *filename);

    /**
     * @brief After loading the streets, this function goes through all of them and
     * find all the crossroads.
     */
    void find_crossroads();

	/**
	* @brief load allstops from file
 	* @param filename
 	*/
    void load_all_stops(const char *filename);
};

#endif
