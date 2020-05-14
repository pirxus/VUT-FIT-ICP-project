/**
 * @file viewstreet.h
 * @brief
 * @author
 */

#ifndef VIEWSTREET_H
#define VIEWSTREET_H

#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QPaintEngine>
#include "Street.h"

class ViewStreet : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
private:
    Street *m_street;

public:
    explicit ViewStreet(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent = nullptr);
    void set_street(Street *street);
    Street *get_street() { return m_street; }

protected:

    /**
     * @brief hoverEnterEvent Specifies the hover consequences
     */
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    /**
     * @brief hoverLeaveEvent Specifies the end of hovering
     */
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    /**
     * @brief focusInEvent Specifies the change of focused item
     */
    virtual void focusInEvent(QFocusEvent *event) override;
    /**
     * @brief focusOutEvent Specifies the change of item that lost focus
     */
    virtual void focusOutEvent(QFocusEvent *event) override;

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;


private slots:

signals:
    /**
     * @brief street_selected Signal emitted when street is selected
     * @param street Street object
     */
    void street_selected(Street *street);

    /**
     * @brief street_selected Signal emitted when street is unselected
     * @param street Street object
     */
    void street_unselected(Street *street);

    void notify_street_cancelled(ViewStreet *street);
};

#endif // VIEWSTREET_H
