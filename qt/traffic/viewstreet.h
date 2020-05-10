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
    void closing_cancelled();
protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;


private slots:

signals:
    void street_selected(Street *street);
    void street_unselected(Street *street);
    void notify_street_cancelled(ViewStreet *street);
};

#endif // VIEWSTREET_H
