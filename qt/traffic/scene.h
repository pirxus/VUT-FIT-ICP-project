#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include "viewconnection.h"
#include "viewstreet.h"
#include "viewstop.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT

private:
    std::vector<ViewConnection *> m_connections;
    std::vector<ViewStreet *> m_streets;
    std::vector<ViewStop *> m_stops;


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
};

#endif // SCENE_H
