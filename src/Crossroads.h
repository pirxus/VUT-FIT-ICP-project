/**
 * @file Crossroads.h
 * @brief
 * @author
 */

#ifndef CROSSROADS_H
#define CROSSROADS_H

#include <utility>
#include "Coords.h"

/* Forward declarations.. */
class Street;

class Crossroads {
    Coords m_pos;
    std::pair<Street *, Street *> m_streets; /* A pair of the two crossing streets */

public:

    Crossroads(): m_pos{Coords()} {}
    Crossroads(Coords pos, Street *ver, Street *hor):
        m_pos{pos}, m_streets{std::pair<Street *, Street *>(ver, hor)} {}

    Coords get_pos() { return this->m_pos; }
    std::pair<Street *, Street *> get_streets() { return this->m_streets; }
    Street *get_vertical() { return m_streets.first; }
    Street *get_horizontal() { return m_streets.second; }

};

#endif
