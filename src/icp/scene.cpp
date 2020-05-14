#include "scene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

Scene::Scene(QObject *parent) : QGraphicsScene(parent)
{
    m_canceled_street = nullptr;
}

Scene::~Scene()
{
    this->clear_route();
    for (auto *i : this->m_streets) { delete i; }
    for (auto *i : this->m_connections) { delete i; }
    for (auto *i : this->m_stops) { delete i; }
    for (auto *i : this->m_street_labels) { delete i; }
    for (auto *i : this->m_stop_labels) { delete i; }
}

void Scene::add_connection(Connection *conn)
{
    /* Create and setup new Connection view*/
    ViewConnection *new_conn = new ViewConnection();
    this->m_connections.push_back(new_conn);
    new_conn->set_connection(conn);

    /* Connect the signals */
    connect(new_conn, &ViewConnection::display_route, this, &Scene::display_route);
    connect(new_conn, &ViewConnection::clear_route, this, &Scene::clear_route);
    this->addItem(new_conn);
    connect(new_conn, &ViewConnection::display_itinerary, this, &Scene::display_itinerary);
    connect(new_conn, &ViewConnection::clear_itinerary, this, &Scene::clear_itinerary);
}

void Scene::add_street(Street *street)
{
    /* Create and setup new Street view*/
    ViewStreet *new_street = new ViewStreet(street->start.x(), street->start.y(), street->end.x(), street->end.y());
    this->m_streets.push_back(new_street);
    new_street->set_street(street);

    /* Connect the signals */
    connect(new_street, &ViewStreet::street_selected, this, &Scene::street_selected_slot);
    connect(new_street, &ViewStreet::street_unselected, this, &Scene::street_unselected_slot);
    connect(new_street, &ViewStreet::notify_street_cancelled, this, &Scene::street_canceled_slot);
    this->addItem(new_street);

    /* Add a street label to the m_stop_labels vector */
    QGraphicsTextItem *label = new QGraphicsTextItem();
    label->setHtml("<div style='color: red; background-color:#DDDDDD;'>" + QString(street->name.c_str()) + "</div>");
    label->setVisible(false);
    this->addItem(label);
    label->setPos((street->start.x() + street->end.x()) / 2.0,
            (street->start.y() + street->end.y()) / 2.0);
    m_street_labels.push_back(label);
}

void Scene::add_stop(Stop *stop)
{
    /* Create and setup new Stop view*/
    ViewStop *new_stop = new ViewStop(stop);
    m_stops.push_back(new_stop);
    this->addItem(new_stop);

    /* Add a stop label to the m_stop_labels vector */
    QGraphicsTextItem *label = new QGraphicsTextItem();
    label->setHtml("<div style='background-color:#DDDDDD;'>" + QString(stop->name().c_str()) + "</div>");
    label->setVisible(false);
    this->addItem(label);
    label->setPos(stop->pos.x() + 2.0, stop->pos.y() + 2.0);
    m_stop_labels.push_back(label);
}

void Scene::redraw_connections()
{
    for (auto conn : this->m_connections) {
        conn->redraw();
    }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    /* street closing mode */
    if (this->m_canceled_street != nullptr) {
        for (QGraphicsItem *item : items(event->scenePos())) {

            ViewStreet *street = dynamic_cast<ViewStreet *>(item);
            if (street != nullptr && street != m_canceled_street) {

                /* Only select open streets */
                if (street->get_street()->closed()) continue;

                /* If not already present, add the street to the detour vector */
                if (std::find(m_detour.begin(), m_detour.end(), street) == m_detour.end()) {

                    m_detour.push_back(street);
                    /* Repaint the selected street */
                    street->setPen(QPen(Qt::green, 5, Qt::DashDotLine, Qt::RoundCap));
                    street->setAcceptHoverEvents(false);
                }
            }
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void Scene::display_route(Line *line)
{
    /* Clear the currently displayed path */
    if (!this->m_displayed_route.empty()) {
        this->clear_route();
    }

    /* Get the lines */
    this->m_displayed_route = get_lines_waypoints(line->get_route());

    /* Paint the route lines */
    for (auto item : this->m_displayed_route) {

        item->setPen(QPen(line->get_color(), 5, Qt::DashLine, Qt::RoundCap));
        item->setOpacity(0.6);
        this->addItem(item);
    }
}

void Scene::clear_route()
{
    /* Remove every item from display and delete it */
    for (auto item : this->m_displayed_route) {
        this->removeItem(item);
        delete item;
    }
    this->m_displayed_route.clear();
}

void Scene::street_selected_slot(Street *street)
{
    emit(street_selected(street));
}

void Scene::street_unselected_slot(Street *street)
{
    emit(street_unselected(street));
}

void Scene::street_canceled_slot(ViewStreet *street)
{
    this->m_canceled_street = street;
    emit(street_canceled(street));
}

void Scene::prepare_for_detour()
{
    /* Turn off mouse events for all the streets */
    for (auto street : m_streets) {
        street->setAcceptedMouseButtons(Qt::NoButton);
    }

    m_detour.clear();
}

void Scene::end_detour_selection(bool correct)
{
    /* Turn mouse events back on */
    for (auto street : m_streets) {
        if (!street->get_street()->closed()) {
            street->setAcceptedMouseButtons(Qt::AllButtons);
        }
    }

    if (!correct) {
        /* Bring the closed street back to normal */
        m_canceled_street->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
        m_canceled_street->setFlag(QGraphicsItem::ItemIsSelectable, true);
        m_canceled_street->setAcceptHoverEvents(true);
    }

    /* Bring the detour street look back to normal */
    for (auto street : m_detour) {
        street->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
        street->setAcceptHoverEvents(true);
    }


    m_canceled_street = nullptr;
    m_detour.clear();
}

void Scene::toggle_street_names(bool checked)
{
    /* Show or hide all the labels */
    for (auto *item : m_street_labels) {
        if (checked) {
            item->setVisible(true);
            item->setZValue(m_stop_labels.at(0)->zValue() + 1);
        } else {
            item->setVisible(false);
            item->setZValue(0);
        }
    }
}

void Scene::toggle_stop_names(bool checked)
{
    /* Show or hide all the labels */
    for (auto *item : m_stop_labels) {
        if (checked) {
            item->setVisible(true);
            item->setZValue(m_street_labels.at(0)->zValue() + 1);
        } else {
            item->setVisible(false);
            item->setZValue(0);
        }
    }
}

void Scene::itinerary_displayed_slot(Connection *conn)
{
    emit(display_itinerary(conn));
}

void Scene::itinerary_cleared_slot()
{
    emit(clear_itinerary());
}
