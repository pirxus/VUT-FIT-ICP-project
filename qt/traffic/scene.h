﻿#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QPainterPath>
#include "viewconnection.h"
#include "viewstreet.h"
#include "viewstop.h"
#include "utilities.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT

private:
    std::vector<ViewConnection *> m_connections;
    std::vector<ViewStreet *> m_streets;
    std::vector<ViewStop *> m_stops;

    std::vector<QGraphicsLineItem *> m_displayed_route;

public:
    explicit Scene(QObject *parent = nullptr);
    ~Scene();

    void add_connection(Connection *conn);
    void add_street(Street *street);
    void add_stop(Stop *stop);
    void redraw_connections();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void street_selected(Street *street);
    void street_unselected(Street *street);

    void display_itinerary(Connection *conn);
    void clear_itinerary();

private slots:
    void display_route(Line *line);
    void clear_route();

    void street_selected_slot(Street *street);
    void street_unselected_slot(Street *street);

    void itinerary_displayed_slot(Connection *conn);
    void itinerary_cleared_slot();

};

#endif // SCENE_H
