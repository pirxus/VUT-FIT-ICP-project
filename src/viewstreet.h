/**
 * @file viewstreet.h
 * @brief A graphical representation of the Street class.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 *
 * A graphical representation of the Street class, used by our implementation of the
 * graphics scene. This class extends the QGraphicsLineItem class (as the stops
 * are represented by lines..) and stores a pointer to the respective street instance.
 * This stored instance is only accessed for retreiving information, never to write any.
 *
 * This class also handles are user interaction events and allows the user for example
 * to close a street by double-clicking it and to specify a detour afterwards.
 */

#ifndef VIEWSTREET_H
#define VIEWSTREET_H

#include <QVariant>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QPaintEngine>
#include "Street.h"

/**
 * @brief The ViewStreet class is the interactive graphical representation of the
 * Street class.
 */
class ViewStreet : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
private:
    Street *m_street; /**< A pointer to the represented street */

public:
    /**
     * @brief ViewStreet constructor
     */
    explicit ViewStreet(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent = nullptr);

    /**
     * @brief m_street attribute setter
     * @param street The street we want to set
     */
    void set_street(Street *street);

    /**
     * @brief m_street attribute getter
     */
    Street *get_street() { return m_street; }

protected:

    /**
     * @brief Specifies the reaction to hoverEnter
     */
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    /**
     * @brief Specifies reaction to hoverLeave
     */
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    /**
     * @brief Specifies the reaction to focusIn
     */
    virtual void focusInEvent(QFocusEvent *event) override;

    /**
     * @brief Specifies the reaction to focusOut
     */
    virtual void focusOutEvent(QFocusEvent *event) override;

    /**
     * @brief Double click handler - selects the street to be closed and initiates the
     * detour selection
     */
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

    /**
     * @brief Street selecting handler
     */
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

signals:

    /**
     * @brief Signal emitted when street is selected
     * @param street Street object
     */
    void street_selected(Street *street);

    /**
     * @brief Signal emitted when street is unselected
     * @param street Street object
     */
    void street_unselected(Street *street);

    /**
     * @brief Notifies the scene that the street has been chosen to be closed
     * @param street
     */
    void notify_street_cancelled(ViewStreet *street);
};

#endif // VIEWSTREET_H
