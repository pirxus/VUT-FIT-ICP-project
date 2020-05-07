/**
 * @file Street.cpp
 * @brief
 * @author
 */

#include "Street.h"


bool Street::lies_on_street(Coords point) {
    //TODO
    return false;
}

void Street::add_stop(Stop *stop) {
    //TODO

}


Street::Street(std::string name, int x1, int y1, int x2, int y2){
	m_name = name;
	Coords m_start(x1, y1);
	Coords m_end(x2, y2);
}
