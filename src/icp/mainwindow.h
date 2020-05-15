/**
 * @file mainwindow.h
 * @brief This is the main window module for the application.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 *
 * This is the main window module for the application. The main role of this module is
 * to interconnect the user interaction on the gui widgets and the graphics scene with
 * the logical backend of the program.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "PublicTransport.h"
#include "scene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The main window class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void zoomIn(); /**< Zooms in the map */
    void zoomOut(); /**< Zooms out the map */
    void sliderZoom(int z); /**< Zooms the map in/out based on the slider value */

    void load_map(); /**< Lets the user load the map from a file */
    void load_stops(); /**< Lets the user load the stops from a file */
    void load_lines(); /**< Lets the user load the lines from a file */

    void positions_updated(); /**< Lets the scene know that it can redraw its content */
    void update_time(unsigned time); /**< Sets the time on the clock */
    void slider_speed(); /**< Time speed slider handler */
    void toggle_play_pause(); /**< Play/Pause event handler */

    void street_selected(Street *street); /**< Enables some gui options when selecting a street */
    void street_unselected(Street *street); /**< Reverts things back to normal after a street is unscelected */

    void traffic_situation_changed(int level); /**< When modifying the traffic situation, notify the street */

    void street_cancelled(ViewStreet *street); /**< When a street is closed, prepares the ui for detour selection */
    void cancel_street_cancel(); /**< Cancels the closing of a street */
    void accept_street_cancel(); /**< Accepts the selected detour and applies it */
    void restore_after_cancel(); /**< Restores the ui elements after closing a street */

    void display_itinerary(Connection *conn); /**< Displays the itinerary of a connection */
    void clear_itinerary(); /**< Clears the currently displayed itinerary */

    void display_street_name(); /**< Displays street name labels on the map */
    void display_stop_name(); /**< Displays stop name labels on the map */

private:
    void initTraffic(); /**< Creates a public transport object and initializes it */
    void initScene(); /** Initialize scenes */

    PublicTransport *m_transit; /**< The controller of the program logic */
    Ui::MainWindow *m_ui; /**< A pointer to the ui object */
    Scene *m_scene; /**< A pointer to the main scene object */
    QGraphicsScene *m_itinerary_scene; /**< A pointer to the itinerary scene object */
    Street *m_currently_edited_street; /**< A pointer to a street whose traffic state is being edited */
    ViewStreet *m_currently_cancelled_street; /**< A pointer to a street which is currently marked to be closed */

    bool m_street_closing_mode; /**< Indicates whether the program is currently in the street closing state */
    std::vector<ViewStreet *> m_detour; /**< A temporary vector of streets that have been selected as a detour */
    Connection *m_itinerary; /**< A pointer to a connection object whose itinerary is currently being displayed */
};

#endif // MAINWINDOW_H
