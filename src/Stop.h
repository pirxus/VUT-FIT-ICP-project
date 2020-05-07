/**
 * @file Stop.h
 * @brief
 * @author
 */

#ifndef STOP_H
#define STOP_H

#include <string>
#include "Coords.h"

/* Forward declarations */
class Street;

class Stop {
    std::string m_name;
public:
    Coords pos;
    Street *street;

    Stop(Coords pos, Street *street): pos{pos}, street{street} {}
    Stop(std::string name_street, std::string name, int x1, int y1);
};

#endif
