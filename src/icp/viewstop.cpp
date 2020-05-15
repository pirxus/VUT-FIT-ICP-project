/**
 * @file viewstop.cpp
 * @brief A graphical representation of the Stop class.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 */

#include "viewstop.h"
#include <QDebug>

ViewStop::ViewStop(Stop *stop, qreal width, qreal height, QGraphicsItem *parent) :
    QGraphicsEllipseItem(stop->pos.x()-6, stop->pos.y()-6, width, height, parent)
{
    this->setBrush(QBrush(QColor{Qt::red}, Qt::SolidPattern));
    this->setPen(QPen({Qt::black}, 2));
    this->m_stop = stop;
}
