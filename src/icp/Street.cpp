/**
 * @file Street.cpp
 * @brief This module contains a class that represents a street on the map.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 */

#include "Street.h"

Street::Street(std::string name, int x1, int y1, int x2, int y2){
    this->name = name;
    this->start = QPoint(x1, y1);
    this->end = QPoint(x2, y2);
    this->m_traffic = 0;
    this->m_closed = false;
}
