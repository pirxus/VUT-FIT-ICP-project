/**
 * @file Coords.cpp
 * @brief
 * @author
 */

#include "Coords.h"

/* Coords operators */
bool operator ==(Coords c1, Coords c2) {
    return c1.x == c2.x && c1.y == c2.y;
}

bool operator !=(Coords c1, Coords c2) {
    return c1.x != c2.x || c1.y != c2.y;
}

Coords operator +(Coords c1, Coords c2) {
    return Coords(c1.x + c2.x, c1.y + c2.y);
}

Coords operator -(Coords c1, Coords c2) {
    return Coords(c1.x - c2.x, c1.y - c2.y);
}
