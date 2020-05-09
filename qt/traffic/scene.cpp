#include "scene.h"
#include <QDebug>

Scene::Scene(QObject *parent) : QGraphicsScene(parent)
{
}

Scene::~Scene()
{
    this->clear_route();
    for (auto i : this->m_streets) { delete i; }
    for (auto i : this->m_connections) { delete i; }
    for (auto i : this->m_stops) { delete i; }
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
}

void Scene::add_street(Street *street)
{
    /* Create and setup new Street view*/
    ViewStreet *new_street = new ViewStreet(street->start.x(), street->start.y(), street->end.x(), street->end.y());
    this->m_streets.push_back(new_street);
    new_street->set_street(street);
    this->addItem(new_street);
}

void Scene::add_stop(Stop *stop)
{
    /* Create and setup new Stop view*/
    ViewStop *new_stop = new ViewStop(stop);
    this->m_stops.push_back(new_stop);
    this->addItem(new_stop);
}

void Scene::redraw_connections()
{
    for (auto conn : this->m_connections) {
        conn->redraw();
    }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

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
        item->setPen(QPen({Qt::red}, 5));
        item->setOpacity(0.3);
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
