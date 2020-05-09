#include "viewconnection.h"

ViewConnection::ViewConnection(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent) :
    QGraphicsEllipseItem(x, y, width, height, parent)
{
    this->setVisible(false);
    this->setPen(QPen({Qt::black}, 1));
}

void ViewConnection::set_connection(Connection *conn)
{
    this->conn = conn;
    this->setBrush(QBrush(QColor{Qt::yellow}, Qt::SolidPattern));
}

void ViewConnection::redraw()
{
    this->setPos(this->conn->get_pos().x() - 4, this->conn->get_pos().y() - 4);
    if (this->conn->active) {
        this->setVisible(true);
    } else {
        this->setVisible(false);
    }
}
