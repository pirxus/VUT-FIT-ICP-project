#include "viewstop.h"

ViewStop::ViewStop(Stop *stop, qreal width, qreal height, QGraphicsItem *parent) :
    QGraphicsEllipseItem(stop->pos.x(), stop->pos.y(), width, height, parent)
{
    this->setBrush(QBrush(QColor{Qt::red}, Qt::SolidPattern));
    this->setPen(QPen({Qt::black}, 3));
    this->m_stop = stop;
}
