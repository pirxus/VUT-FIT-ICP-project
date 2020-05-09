#include "scene.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent)
{

}

Scene::~Scene()
{
    for (auto i : this->m_streets) { delete i; }
    for (auto i : this->m_connections) { delete i; }
    for (auto i : this->m_stops) { delete i; }
}

void Scene::add_connection(Connection *conn)
{
    ViewConnection *new_conn = new ViewConnection();
    this->m_connections.push_back(new_conn);
    new_conn->set_connection(conn);
    this->addItem(new_conn);
}

void Scene::add_street(Street *street)
{
    ViewStreet *new_street = new ViewStreet(street->start.x(), street->start.y(), street->end.x(), street->end.y());
    this->m_streets.push_back(new_street);
    new_street->set_street(street);
    this->addItem(new_street);
}

void Scene::add_stop(Stop *stop)
{
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
