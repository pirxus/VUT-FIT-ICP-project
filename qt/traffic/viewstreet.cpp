#include "viewstreet.h"

ViewStreet::ViewStreet(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent) :
    QObject(), QGraphicsLineItem(x1, y1, x2, y2, parent)
{
    this->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    //this->setFlag(QGraphicsItem::ItemIsFocusable);
}

void ViewStreet::set_street(Street *street)
{
    this->m_street = street;
}

void ViewStreet::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    this->setPen(QPen(Qt::black, 3, Qt::DashDotLine, Qt::RoundCap));
    QGraphicsLineItem::hoverEnterEvent(event);
}

void ViewStreet::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
    QGraphicsLineItem::hoverLeaveEvent(event);
}

void ViewStreet::focusInEvent(QFocusEvent *event)
{
    this->setAcceptHoverEvents(false);
    this->setPen(QPen(Qt::gray, 5, Qt::SolidLine, Qt::RoundCap));
    QGraphicsLineItem::focusInEvent(event);
}

void ViewStreet::focusOutEvent(QFocusEvent *event)
{
    this->setAcceptHoverEvents(true);
    this->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
    QGraphicsLineItem::focusOutEvent(event);
}

QVariant ViewStreet::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedChange) {
        if (value == true) {
            this->setPen(QPen(Qt::gray, 5, Qt::SolidLine, Qt::RoundCap));
            this->setAcceptHoverEvents(false);

            /* Set the traffic intensity counter on the UI to the current value of the
             * street by emitting a signal */
            emit(street_selected(this->m_street));

        } else {
            this->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
            this->setAcceptHoverEvents(true);
            emit(street_unselected(this->m_street));
        }
    }

    return QGraphicsLineItem::itemChange(change, value);
}
