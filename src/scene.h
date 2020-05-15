/**
 * @file scene.h
 * @brief An extension of QGraphicsScene with some additional attributes and methods.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 *
 * An extension of QGraphicsScene with some additional attributes and methods. This
 * class allows us to store and manipulate references to our displayed 'view' objects.
 * These 'view' objects are stored in vectors and allow us to graphically represent
 * their purely logical counterparts.
 */

#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QPainterPath>
#include "viewconnection.h"
#include "viewstreet.h"
#include "viewstop.h"
#include "utilities.h"

/**
 * @brief The Scene class extends the QGraphicsScene class and is responsible
 * for graphically representing the simulated public transport system.
 */
class Scene : public QGraphicsScene
{
    Q_OBJECT

private:
    std::vector<ViewConnection *> m_connections; /**< A vector of displayed connections */
    std::vector<ViewStreet *> m_streets; /**< A vector of displayed streets */
    std::vector<ViewStop *> m_stops; /**< A vector of displayed stops */

    std::vector<QGraphicsTextItem *> m_street_labels; /**< A vector of displayed street labels */
    std::vector<QGraphicsTextItem *> m_stop_labels; /**< A vector of displayed stop labels */

    std::vector<QGraphicsLineItem *> m_displayed_route; /**< The currently displayed connection route */

    ViewStreet *m_canceled_street; /**< The street that is currently being canceled */
    std::vector<ViewStreet *> m_detour; /**< A set of pointers to streets marked as a detour */

public:
    explicit Scene(QObject *parent = nullptr);
    ~Scene();

    void add_connection(Connection *conn); /**< Creates a new ViewConnection object and adds it to the scene */
    void add_street(Street *street); /**< Creates a new ViewStreet object and adds it to the scene */
    void add_stop(Stop *stop); /**< Creates a new ViewStop object and adds it to the scene */

    void redraw_connections(); /**< Redraws all connections in the scene */
    void prepare_for_detour(); /**< Turns off mouse interaction for streets */
    void end_detour_selection(bool correct); /**< Restores mouse interaction for streets */

    std::vector<ViewStreet *> get_detour() { return m_detour; } /**< m_detour getter */

    void toggle_street_names(bool checked); /**< Toggles street name labels */
    void toggle_stop_names(bool checked); /**< Toggles stop name labels */

    void delete_streets(); /**< Clears the m_streets vector */
    void delete_stops(); /**< Clears the m_stops vector */
    void delete_connections(); /**< Clears the m_connections vector */
    void delete_street_labels(); /**< Clears the m_street_labels vector */
    void delete_stop_labels(); /**< Clears the m_stop_labels vector */
    void delete_labels(); /**< Clears the both label vectors */

protected:
    /**
     * @brief Handler used when selecting detours
     * @param event
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void street_selected(Street *street); /**< A street has been selected */
    void street_unselected(Street *street); /**< A street has been unselected */

    /**
     * @brief Propagates the notification about a street
     * being closed further to main window
     * @param street ViewStreet object
     */
    void street_canceled(ViewStreet *street);

    void display_itinerary(Connection *conn); /**< Tells the main window to display the itinerary of a connection */
    void clear_itinerary(); /**< Tells the main window to clear the itinerary scene */

private slots:
    void display_route(Line *line); /**< Displays the route of a line */
    void clear_route(); /**< Clears the displayed route */

    void street_selected_slot(Street *street); /**< Handles the street selection - propagate to main window */
    void street_unselected_slot(Street *street); /**< Handles the street unselection - propagate to main window */

    /**
     * @brief Notifies the main window that a street has
     * been marked for cancellation using the street_canceled signal.
     * @param street
     */
    void street_canceled_slot(ViewStreet *street);

    /**
     * @brief Propagates the information that an itinerary should be displayed to the
     * main window
     * @param conn
     */
    void itinerary_displayed_slot(Connection *conn);

    /**
     * @brief Propagates the information that an itinerary should be closed to the
     * main window
     * @param conn
     */
    void itinerary_cleared_slot();

};

#endif // SCENE_H
