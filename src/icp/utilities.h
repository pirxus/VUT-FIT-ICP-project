/**
 * @file utilities.h
 * @brief
 * @author
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <QVector>
#include <string>
#include "scene.h"
#include "Line.h"

static QColor line_colors[10] = {QColor("lightGreen"), QColor("magenta"), QColor("yellow"),
                      QColor("blue"), QColor("darkCyan"), QColor("darkMagenta"),
                      QColor("cyan"), QColor("darkGreen"), QColor("red"),
                      QColor("darkRed")};

/**
 * @brief strip_whitespace Strips the initial and trailing whitespaces from a string
 * @param str the input string
 * @return modified string
 */
std::string strip_whitespace(std::string str);


/**
 * @brief get_lines_waypoints For a list of waypoints, this function returns a series
 * of QGraphicsLineItems that represent the line route.
 * @param points The set of waypoints
 * @return Returns a QVector of pointers to the created lines
 */
std::vector<QGraphicsLineItem *> get_lines_waypoints(std::vector<Waypoint> points);

/**
 * @brief This function returns a color from our color pallete
*/
QColor get_next_color();

#endif // UTILITIES_H
