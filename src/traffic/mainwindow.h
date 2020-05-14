/**
 * @file mainwindow.h
 * @brief
 * @author
 */

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
    void accept_street_cancel();

    void restore_after_cancel();

    void display_itinerary(Connection *conn);
    void clear_itinerary();

    void display_street_name();
    void display_stop_name();
    //void display_street_name(Street *streets);
    //void display_stop_name(Stop *stops);

private:
    void initTraffic(); /**< Creates a public transport object */
    void initScene(); /** Initialize scenes */

    PublicTransport *transit; /**< The controller of the program logic */
    Ui::MainWindow *ui;
    Scene *scene;
    QGraphicsScene *itineraryScene;
    Street *currently_edited_street;
    ViewStreet *currently_cancelled_street;

    bool street_closing_mode;
    std::vector<ViewStreet *> detour;
};
#endif // MAINWINDOW_H
