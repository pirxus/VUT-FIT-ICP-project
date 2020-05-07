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

/**
 * @brief Checks whether the Street object is vertically oriented
 */
bool Street::is_vertical() {
    this->m_start.first == this->m_end.first ? return true : return false;
}

/**
 * @brief Checks whether the Street object is horizontally oriented
 */
bool Street::is_horizontal() {
    this->m_start.second == this->m_end.second ? return true : return false;
}
