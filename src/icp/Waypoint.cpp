#include "Waypoint.h"


int Waypoint::find_stop(std::vector<Waypoint> points, Stop *stop)
{
    unsigned len = points.size();
    for (unsigned i = 0; i < len; i++) {
        if (points.at(i).stop == stop) {
            return i;
        }
    }
    return -1; /* not found, Should not happen... */
}

double Waypoint::segment_len(std::vector<Waypoint> points, unsigned start, unsigned end)
{
    double total_distance = 0;
    for (unsigned i = start; i < end; i++) {
        total_distance += distance(points.at(i), points.at(i + 1));
    }

    return total_distance;
}

double Waypoint::distance(Waypoint first, Waypoint second)
{
    if (first.pos == second.pos) return 0;
    return sqrt(pow(first.pos.x() - second.pos.x(), 2.0) + pow(first.pos.y() - second.pos.y(), 2.0));
}
