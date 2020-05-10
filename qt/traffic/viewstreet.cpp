#include "viewstreet.h"

ViewStreet::ViewStreet(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent) :
    QGraphicsLineItem(x1, y1, x2, y2, parent)
{
    this->setPen(QPen({Qt::black}, 2));
    this->setAcceptHoverEvents(true);
}

void ViewStreet::set_street(Street *street)
{
    this->m_street = street;
}

void ViewStreet::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    this->setPen(QPen({Qt::black}, 4));
    QGraphicsLineItem::hoverEnterEvent(event);
}

void ViewStreet::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setPen(QPen({Qt::black}, 2));
    QGraphicsLineItem::hoverLeaveEvent(event);
}
