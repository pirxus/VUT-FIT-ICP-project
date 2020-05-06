/**
 * @file Coords.h
 * @brief
 * @author
 */

#ifndef COORDS_H
#define COORDS_H

class Coords {
public:
    int x, y;
    Coords(): x{0}, y{0} {}
    Coords(const int x, const int y): x{x}, y{y} {}

    friend bool operator ==(Coords c1, Coords c2);
    friend bool operator !=(Coords c1, Coords c2);
    friend Coords operator +(Coords c1, Coords c2);
    friend Coords operator -(Coords c1, Coords c2);
};

#endif
