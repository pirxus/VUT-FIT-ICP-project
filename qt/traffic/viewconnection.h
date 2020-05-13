/**
 * @file viewconnection.h
 * @brief
 * @author
 */

#ifndef VIEWCONNECTION_H
#define VIEWCONNECTION_H

#include <QGraphicsItem>
#include <QObject>
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QPen>
#include "Connection.h"
#include "Line.h"


class ViewConnection : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
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
    /**
     * @brief display_route Signal emitted when one connection is selected
     * @param line Line object
     */
    void display_route(Line *line);

    /**
     * @brief clear_route Signal emitted when connection is unselected
     */
    void clear_route();

    /**
     * @brief display_itinerary Signal emitted when one connection is selected
     * @param conn Connection object
     */
    void display_itinerary(Connection *conn);

    /**
     * @brief clear_itinerary Signal emitted when one connection is unselected
     */
    void clear_itinerary();
};

#endif // VIEWCONNECTION_H
