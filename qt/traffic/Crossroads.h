/**
 * @file Crossroads.h
 * @brief
 * @author
 */

#ifndef CROSSROADS_H
#define CROSSROADS_H

#include <utility>
#include <QPoint>

/* Forward declarations.. */
class Street;

class Crossroads {
    QPoint m_pos;
    std::pair<Street *, Street *> m_streets; /* A pair of the two crossing streets */

public:

    Crossroads(): m_pos{QPoint()} {}
    Crossroads(QPoint pos, Street *ver, Street *hor):
        m_pos{pos}, m_streets{std::pair<Street *, Street *>(ver, hor)} {}

    QPoint get_pos() { return this->m_pos; }
    std::pair<Street *, Street *> get_streets() { return this->m_streets; }
};

#endif
