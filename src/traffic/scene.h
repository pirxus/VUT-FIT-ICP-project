/**
 * @file scene.h
 * @brief
 * @author
 */

#ifndef SCENE_H
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

    std::vector<QGraphicsTextItem *> m_street_labels;
    std::vector<QGraphicsTextItem *> m_stop_labels;

    std::vector<QGraphicsLineItem *> m_displayed_route;

    ViewStreet *m_canceled_street;
    std::vector<ViewStreet *> m_detour;

public:
    explicit Scene(QObject *parent = nullptr);
    ~Scene();

    void add_connection(Connection *conn);
    void add_street(Street *street);
    void add_stop(Stop *stop);
    void redraw_connections();
    void prepare_for_detour();
    void end_detour_selection(bool correct);
    std::vector<ViewStreet *> get_detour() { return m_detour; }

    void toggle_street_names(bool checked);
    void toggle_stop_names(bool checked);

    void delete_streets();
    void delete_stops();
    void delete_connections();
    void delete_street_labels();
    void delete_stop_labels();
    void delete_labels();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void street_selected(Street *street);
    void street_unselected(Street *street);

    /**
     * @brief street_canceled Propagates the notification about a street
     * being cancelled further to main window
     * @param street
     */
    void street_canceled(ViewStreet *street);
    void display_itinerary(Connection *conn);
    void clear_itinerary();

private slots:
    void display_route(Line *line);
    void clear_route();

    void street_selected_slot(Street *street);
    void street_unselected_slot(Street *street);

    /**
     * @brief street_canceled_slot Notifies the main window that a street has
     * been marked for cancellation using the street_canceled signal.
     * @param street
     */
    void street_canceled_slot(ViewStreet *street);
    void itinerary_displayed_slot(Connection *conn);
    void itinerary_cleared_slot();

};

#endif // SCENE_H
