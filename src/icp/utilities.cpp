#include "utilities.h"
#include <regex>

/**
 * @brief This function strips a string of the initial and trailing whitespace characters
 * @param str
 * @return string
 */
std::string strip_whitespace(std::string str) {
    str = std::regex_replace(str, std::regex("^\\s+"), std::string(""));
    str = std::regex_replace(str, std::regex("\\s+$"), std::string(""));
    return str;
}

std::vector<QGraphicsLineItem *> get_lines_waypoints(std::vector<Waypoint> points)
{
    std::vector<QGraphicsLineItem *> lines;
    unsigned points_len = points.size();
    for (unsigned i = 0; i < points_len - 1; i++) {
        QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(points.at(i).pos, points.at(i + 1).pos));
        lines.push_back(line);
    }
    return lines;
}

QColor get_next_color()
{
    /* This is an index to the color pallete for our lines. */
    static unsigned color_index = 0;
    return line_colors[color_index++ % 10];
}
