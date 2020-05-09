#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include "viewconnection.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT

private:
    std::vector<ViewConnection *> m_connections;

public:
    explicit Scene(QObject *parent = nullptr);

    void add_connection(Connection *conn);
    void redraw_connections();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
};

#endif // SCENE_H
