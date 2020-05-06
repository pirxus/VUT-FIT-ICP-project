/**
 * @file Map.h
 * @brief
 * @author
 */

#ifndef MAP_H
#define MAP_H

#include <vector>
#include "Street.h"
#include "Crossroads.h"

class Map {
    std::vector<Street *> m_streets;
    std::vector<Crossroads *> m_cross;
};

#endif
