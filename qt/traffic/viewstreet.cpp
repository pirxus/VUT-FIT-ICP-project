#include "viewstreet.h"

ViewStreet::ViewStreet(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent) :
    QGraphicsLineItem(x1, y1, x2, y2, parent)
{
    this->setPen(QPen({Qt::black}, 2));
}

void ViewStreet::set_street(Street *street)
{
    this->m_street = street;
}
