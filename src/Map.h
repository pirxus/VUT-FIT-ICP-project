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
    std::vector<Street *> m_streets; /**< A vector of all the streets on the map */
    std::vector<Street *> m_streets_v; /**< A vector of vertical streets */
    std::vector<Street *> m_streets_h; /**< A vector of horizontal streets */
    std::vector<Crossroads *> m_cross; /**< A vector of all crossroads on the map */
public:
	//load streets from file
	void load_streets(const char *filename);

    /**
     * @brief After loading the streets, this function goes through all of them and
     * find all the crossroads.
     */
    void find_crossroads();

    /**
     * @brief This function populates the m_streets_v and m_streets_h with horizontal
     * and vertical streets on the map
     *
     * @throw  TODO
     */
    void load_streets_vertical_horizontal();

    /**
     * @brief This method finds all horizontal streets and returns a vector of pointers
     * to their respective objects
     */
    std::vector<Street *> get_horizontal_streets();

    /**
     * @brief This method finds all vertical streets and returns a vector of pointers
     * to their respective objects
     */
    std::vector<Street *> get_vertical_streets();
};

#endif
