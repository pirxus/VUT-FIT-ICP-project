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
    Connection *m_conn;
    int m_size;

public:
    explicit ViewConnection(qreal x = 0, qreal y = 0, qreal width = 8, qreal height = 8, QGraphicsItem *parent = nullptr);
    void set_connection(Connection *conn);
    void redraw();

protected:
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

signals:
    //void display_route(Line *line);
    //void clear_route();
};

#endif // VIEWCONNECTION_H
