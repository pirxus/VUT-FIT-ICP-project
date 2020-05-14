#include "viewstop.h"
#include <QDebug>

ViewStop::ViewStop(Stop *stop, qreal width, qreal height, QGraphicsItem *parent) :
    QGraphicsEllipseItem(stop->pos.x()-10, stop->pos.y()-10, width*2, height*2, parent)
{
    this->setBrush(QBrush(QColor{Qt::red}, Qt::SolidPattern));
    this->setPen(QPen({Qt::black}, 3));
    this->m_stop = stop;
}

void ViewStop::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mousePressEvent(event);
}
