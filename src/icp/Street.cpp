/**
 * @file Street.cpp
 * @brief
 * @author
 */

#include "Street.h"


bool Street::lies_on_street(QPoint point) {
    //TODO
    return false;
}

Street::Street(std::string name, int x1, int y1, int x2, int y2){
    this->name = name;
    this->start = QPoint(x1, y1);
    this->end = QPoint(x2, y2);
    this->m_traffic = 0;
    this->m_closed = false;
}
