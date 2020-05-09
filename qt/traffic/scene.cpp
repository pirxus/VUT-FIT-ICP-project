#include "scene.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent)
{

}

void Scene::add_connection(Connection *conn)
{
    ViewConnection *connection = new ViewConnection();
    this->m_connections.push_back(connection);
    connection->set_connection(conn);
    this->addItem(connection);
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
