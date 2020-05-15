/**
 * @file viewconnection.h
 * @brief A graphical representation of the Connection class.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 *
 * A graphical representation of the Connection class, used by our implementation of the
 * graphics scene. This class extends the QGraphicsEllipseItem class (as the connections
 * are represented by circles..) and stores a pointer to the respective connection
 * instance. This stored instance is only accessed for retreiving information, never
 * to write any.
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


/**
 * @brief The ViewConnection class is the interactive graphical representation of the
 * Connection class.
 */
class ViewConnection : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
private:
    Connection *m_conn; /**< A pointer to the represented connection */
    int m_size; /**< The base size of the drawn ellipse */

public:
    explicit ViewConnection(qreal x = 0, qreal y = 0, qreal width = 15, qreal height = 15, QGraphicsItem *parent = nullptr);


    /**
     * @brief Sets the m_conn attribute
     * @param conn The connection
     */
    void set_connection(Connection *conn);

    /**
     * @brief Redraws the connection
     */
    void redraw();

protected:
    virtual void focusInEvent(QFocusEvent *event) override; /**< focusIn handler */
    virtual void focusOutEvent(QFocusEvent *event) override; /**< focusOut handler */

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override; /**< hoverEnter handler */
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override; /**< hoverLeave handler */

signals:
    /**
     * @brief Signal emitted when one connection is selected, displays the connection route
     * @param line Line object
     */
    void display_route(Line *line);

    /**
     * @brief Signal emitted when connection is unselected, clear the displayed route
     */
    void clear_route();

    /**
     * @brief Signal emitted when one connection is selected, notifies the
     * main window to display the itinerary
     * @param conn Connection object
     */
    void display_itinerary(Connection *conn);

    /**
     * @brief Signal emitted when one connection is unselected, clears the displayed itinerary
     */
    void clear_itinerary();
};

#endif // VIEWCONNECTION_H
