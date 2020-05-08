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
#include <map>
#include "Street.h"
#include "Stop.h"

class Map {
public:
    std::map<std::string, Street *> streets; /**< A hash table of all the streets on the map */
    std::map<std::string, Stop *> stops; /**< A vector of all the stops on the map */

    /**
    * @brief Map destructor - clears all the vector attributes
    */
    ~Map();

    /**
    * @brief Clears the streets vector
    */
    void delete_streets();

    /**
    * @brief Clears the stops vector
    */
    void delete_stops();

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
    void load_stops(const char *filename);
};

#endif
