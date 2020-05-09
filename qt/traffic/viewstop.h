#ifndef VIEWSTOP_H
#define VIEWSTOP_H

#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QPen>
#include "Stop.h"

class ViewStop : public QGraphicsEllipseItem
{
private:
    Stop *m_stop;

public:
    explicit ViewStop(Stop *stop, qreal width = 10, qreal height = 10, QGraphicsItem *parent = nullptr);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // VIEWSTOP_H
