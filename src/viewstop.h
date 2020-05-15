/**
 * @file viewstop.h
 * @brief A graphical representation of the Stop class.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 *
 * A graphical representation of the Stop class, used by our implementation of the
 * graphics scene. This class extends the QGraphicsEllipseItem class (as the stops
 * are represented by circles..) and stores a pointer to the respective stop instance.
 * This stored instance is only accessed for retreiving information, never to write any.
 */

#ifndef VIEWSTOP_H
#define VIEWSTOP_H

#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QPen>
#include "Stop.h"

/**
 * @brief The ViewStop class is the graphical representation of the Stop class.
 */
class ViewStop : public QGraphicsEllipseItem
{
private:
    Stop *m_stop; /**< A pointer to the represented stop */

public:
    /**
     * @brief ViewStop constructor
     */
    explicit ViewStop(Stop *stop, qreal width = 12, qreal height = 12, QGraphicsItem *parent = nullptr);
};

#endif // VIEWSTOP_H
