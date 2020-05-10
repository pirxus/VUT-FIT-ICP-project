#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "PublicTransport.h"
#include "scene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void zoomIn();
    void zoomOut();
    void sliderZoom(int z);
    void load_map();
    void load_stops();
    void load_lines();
    void positions_updated();
    void update_time(unsigned time); /**< Sets the time on the clock */
    void toggle_play_pause(); /**< Play/Pause event handler */

    void street_selected(Street *street);
    void street_unselected(Street *street);

    void traffic_situation_changed(int level);

    void street_cancelled(ViewStreet *street);
    void cancel_street_cancel();

private:
    void initTraffic(); /**< Creates a public transport object */
    void initScene();
    PublicTransport *transit; /**< The controller of the program logic */
    Ui::MainWindow *ui;
    Scene *scene;

    Street *currently_edited_street;
    ViewStreet *currently_cancelled_street;
};
#endif // MAINWINDOW_H
