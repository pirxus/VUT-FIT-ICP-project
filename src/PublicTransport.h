/**
 * @file PublicTransport.h
 * @brief This is the central module of the program logic.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 *
 * This is the central module of the program logic. The PublicTransport class stores
 * a Map object as well as a vector of pointers to the Line objects that define the
 * traffic. The 'simulation' process is synchronized by an instance of QTimer which
 * emits signals that allow the PublicTransport class to coordinate the progress of
 * each active connection in the system.
 */

#ifndef PUBLIC_TRANSPORT_H
#define PUBLIC_TRANSPORT_H

#include <QObject>
#include <vector>
#include <QTimer>
#include <QTime>
#include "Map.h"
#include "utilities.h"
#include "Line.h"
#include "Stop.h"

#define TIME_INCREMENT 2 /**< Indicates how many seconds pass in each frame */
#define DEFAULT_TIME 12*3600 /**< Default program time - 12:00 */
#define DEFAULT_TIMEOUT 1000 /**< Default timer timeout value in ms */
#define DEFAULT_CLOCK_RATE 1 /**< Default timeout rate */


/**
 * @brief The PublicTransport class represents the whole simulated public tranport system.
 */
class PublicTransport : public QObject {
    Q_OBJECT

    unsigned m_time; /**< Current system time */
    QTimer *m_timer; /**< A pointer to a timer object coordinating the 'simulation' */
    double m_clock_rate; /**< A base refresh rate of the timer */

signals:
    /**
     * @brief This signal notifies the subscribers that
     * the positions of all vehicles have been updated.
     */
    void vehicles_updated();

    /**
     * @brief After the timer triggers the timer_triggered slot,
     * this signal then releases the information about what the current time is.
     * @param time Current simulation time
     */
    void time_updated(unsigned time);

public slots:

    /**
     * @brief This slot is triggered every time the timer
     * has a timeout. Increments the internal clock by TIME_INCREMENT and
     * fires the time_updated signal.
     */
    void timer_triggered();

    /**
     * @brief This slot receives the current time from the
     * clock in the UI and sets it as the internal time. This Allows us to
     * immediately react to user generated time changes a the small cost
     * of some wasted energy...
     * @param time The received time value
     */
    void time_changed(const QTime &time);

public:
    Map map; /**< An instance of the Map class holding the information about all streets and stops */
    std::vector<Line *> lines; /**< A vector of simulated lines */

    explicit PublicTransport(QObject *parent = nullptr); /**< Constructor */
    ~PublicTransport(); /**< Destructor */

	/**
    * @brief Loads streets from a csv file
    * @param filename The source file
    */
	void load_map(const char *filename);

    /**
    * @brief Loads all stops from a csv file
    * @param filename The source file
    */
    void load_stops(const char *filename);

    /**
    * @brief Load lines and connections from a csv file
    * @param filename The source file
    * @param all_stops vector of all_stops
    */
    void load_lines(const char* filename);

    /**
    * @brief Current simulation time getter
    * @return m_time The current time
    */
    unsigned get_time() { return this->m_time; }

    /**
     * @brief Current simulation time setter
     * @param time New time value
     */
    void set_time(unsigned time) { this->m_time = time; }

    /**
     * @brief Clock rate setter
     * @param clock_rate New simulation clock rate
     */
    void set_clock_rate(unsigned clock_rate);

    /**
     * @brief Deletes all the line objects
     */
    void delete_lines();

    /**
     * @brief Makes all the vehicles move a notch towards their
     * respective destinations
     */
    void update_vehicles();

    /**
     * @brief Assigns routes for each connection from each line
     */
    void prepare_connections();

    /**
     * @brief Starts the simulation
     */
    void start_timer();

    /**
     * @brief Pauses the simulation
     */
    void stop_timer();

    /**
     * @brief Tries to create a detour around a closed street. If the creation succeeds, the detour
     * is then implemented to all lines
     * @param closed The closed street
     * @param detour The user-selected detour
     * @return Returns 0 if the detour creation succeeds, otherwise 1
     */
    int create_detour(Street *closed, std::vector<Street *> detour);

    /**
     * @brief Computes a set of waypoints for a specified detour
     * @param closed The closed street
     * @param detour The user-selected detour
     * @return Returns the detour set of waypoints
     */
    std::vector<Waypoint> compute_waypoints_detour(Street *closed, std::vector<Street *> detour);
};

#endif
