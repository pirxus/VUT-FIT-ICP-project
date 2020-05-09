#ifndef VIEWCONNECTION_H
#define VIEWCONNECTION_H

#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QPen>
#include "Connection.h"
#include "Line.h"


class ViewConnection : public QGraphicsEllipseItem
{
private:
    Connection *conn;

public:
    explicit ViewConnection(qreal x = 0, qreal y = 0, qreal width = 8, qreal height = 8, QGraphicsItem *parent = nullptr);
    void set_connection(Connection *conn);
    void redraw();

signals:

};

#endif // VIEWCONNECTION_H
