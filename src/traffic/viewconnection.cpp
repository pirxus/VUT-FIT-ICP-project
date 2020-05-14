#include "viewconnection.h"

ViewConnection::ViewConnection(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent) :
    QObject(), QGraphicsEllipseItem(x, y, width, height, parent)
{
    this->m_size = 15;
    this->setVisible(false);
    this->setPen(QPen({Qt::black}, 2));
    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsFocusable);
}

void ViewConnection::set_connection(Connection *conn)
{
    this->m_conn = conn;
    this->setBrush(QBrush(conn->get_line()->get_color(), Qt::SolidPattern));
}

void ViewConnection::redraw()
{
    this->setPos(this->m_conn->get_pos().x() - m_size/2.0, this->m_conn->get_pos().y() - m_size/2.0);
    if (this->m_conn->active) {
        this->setVisible(true);

    } else {
        this->clearFocus(); /* Clear the displayed info about the connection */
        this->setVisible(false);
    }
}

void ViewConnection::focusInEvent(QFocusEvent *event)
{
    this->m_size = 20;
    QRectF rect =  this->rect();

    /* resize */
    rect.setWidth(m_size);
    rect.setHeight(m_size);
    this->setRect(rect);

    /* Correct the position offeset */
    this->setPos(this->m_conn->get_pos().x() - m_size/2.0, this->m_conn->get_pos().y() - m_size/2.0);
    QGraphicsEllipseItem::focusInEvent(event);

    /* Notify the scene to display the line route and itinerary*/
    emit(this->display_route(this->m_conn->get_line()));
    emit(this->display_itinerary(this->m_conn));
}

void ViewConnection::focusOutEvent(QFocusEvent *event)
{
    this->m_size = 15;
    QRectF rect =  this->rect();

    /* resize back */
    rect.setWidth(m_size);
    rect.setHeight(m_size);
    this->setRect(rect);

    /* Correct the position offeset */
    this->setPos(this->m_conn->get_pos().x() - m_size/2.0, this->m_conn->get_pos().y() - m_size/2.0);
    QGraphicsEllipseItem::focusOutEvent(event);

    /* Notify the scene to clear the displayed route and itinerary */
    emit(this->clear_route());
}

void ViewConnection::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    this->setPen(QPen({Qt::cyan}, 3));
    QGraphicsEllipseItem::hoverEnterEvent(event);
}

void ViewConnection::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setPen(QPen({Qt::black}, 2));
    QGraphicsEllipseItem::hoverLeaveEvent(event);
}
