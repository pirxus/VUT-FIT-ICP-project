/**
 * @file Stop.cpp
 * @brief This module represents a stop.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 */

#include "Stop.h"
#include "Street.h"

Stop::Stop(Street *street, std::string name, float pos)
{
    m_name = name;
    this->street = street;
    float x = (street->end.x() - street->start.x()) * pos + street->start.x();
    float y = (street->end.y() - street->start.y()) * pos + street->start.y();
    this->pos = QPointF(x, y);
}
