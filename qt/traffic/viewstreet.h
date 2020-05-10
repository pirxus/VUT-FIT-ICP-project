#ifndef VIEWSTREET_H
#define VIEWSTREET_H

#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QPaintEngine>
#include "Street.h"

class ViewStreet : public QGraphicsLineItem
{
private:
    Street *m_street;

public:
    explicit ViewStreet(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent = nullptr);
    void set_street(Street *street);
protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
};

#endif // VIEWSTREET_H
