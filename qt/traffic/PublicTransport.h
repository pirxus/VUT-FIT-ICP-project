/**
 * @file PublicTransport.h
 * @brief
 * @author
 */

#ifndef PUBLIC_TRANSPORT_H
#define PUBLIC_TRANSPORT_H

#include <QObject>
#include <vector>
#include <QTimer>
#include <QTime>
#include "Map.h"
#include "Line.h"
#include "Stop.h"

#define TIME_INCREMENT 2
#define DEFAULT_TIME 14*3600
#define DEFAULT_TIMEOUT 1000
#define DEFAULT_CLOCK_RATE 10.0f

class PublicTransport : public QObject {
    Q_OBJECT

    unsigned m_time;
    QTimer *m_timer;
    double m_clock_rate;

signals:
    /**
     * @brief vehicles_updated This signal notifies the subscribers that
     * the positions of all vehicles have been updated.
     */
    void vehicles_updated();

    /**
     * @brief time_updated After the timer triggers the timer_triggered slot,
     * this signal then releases the information about what the current time is.
     * @param time Current simulation time
     */
    void time_updated(unsigned time);

public slots:

    /**
     * @brief timer_triggered This slot is triggered every time the timer
     * has a timeout. Increments the internal clock by TIME_INCREMENT and
     * fires the time_updated signal.
     */
    void timer_triggered();

    /**
     * @brief time_changed This slot receives the current time from the
     * clock in the UI and sets it as the internal time. This Allows us to
     * immediately react to user generated time changes a the small cost
     * of some wasted energy...
     * @param time The received time value
     */
    void time_changed(const QTime &time);

public:
    Map map;
    std::vector<Line *> lines;

    explicit PublicTransport(QObject *parent = nullptr);
    ~PublicTransport();

	/**
    * @brief loads streets from a csv file
    * @param filename
    */
	void load_map(const char *filename);

    /**
    * @brief loads all stops from a file
    * @param filename
    */
    void load_stops(const char *filename);

    /**
    * @brief load lines and connections from file
    * @param filename
    * @param all_stops vector of all_stops
    */
    void load_lines(const char* filename);

    unsigned get_time() { return this->m_time; }
    void set_time(unsigned time) { this->m_time = time; } //TODO
    void set_timeout(unsigned timeout); /**< Sets the clock speed */ //TODO

    void delete_lines();

    /**
     * @brief Makes all the vehicles move a notch towards their
     * respective destinations
     */
    void update_vehicles();

    /**
     * @brief prepare_connections Assigns routes for each connection from each line
     */
    void prepare_connections();
};

#endif
