/**
 * @file Map.h
 * @brief This module represents the the map.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 *
 * This module represents the the map. The map consists of sets of streets and stops.
 * All streets are stored in two map containers one of which allows searching by street
 * name (we are assuming that no two streets are named the same way) and the other one
 * by the set of street endpoints that define it.
 *
 * The stops are also searchable by their respective names.
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

/**
 * @brief This class represents the loaded map.
 */
class Map {
public:
    std::map<std::string, Street *> streets; /**< A hash table of all the streets on the map */
    std::map<std::tuple<int, int, int, int>, Street *> streets_by_endpoints; /**< A hash table of all the streets on the map, keys are street endpoints */
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
    * @brief Loads streets from a file
    * @param filename The source file
 	*/
	void load_streets(const char *filename);

    /**
     * @brief After loading the streets, this function goes through all of them and
     * find all the crossroads.
     */
    void find_crossroads();

	/**
    * @brief Loads stops from a file
    * @param filename The source file
 	*/
    void load_stops(const char *filename);
};

#endif
