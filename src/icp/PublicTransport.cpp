/**
 * @file PublicTransport.cpp
 * @brief
 * @author
 */

#include <QFile>
#include "PublicTransport.h"
#include "Connection.h"
#include "utilities.h"
#include <regex>



PublicTransport::PublicTransport(QObject *parent) : QObject(parent)
{
   m_time = DEFAULT_TIME;
   m_timer = new QTimer(parent);
   map = Map();
   m_clock_rate = DEFAULT_CLOCK_RATE;
   m_timer->setInterval(DEFAULT_TIMEOUT / m_clock_rate);

   /* Connect the timer timeouts */
   connect(m_timer, &QTimer::timeout, this, &PublicTransport::timer_triggered);
}

PublicTransport::~PublicTransport()
{
    delete_lines();
    delete m_timer;
}

void PublicTransport::timer_triggered()
{
    m_time += TIME_INCREMENT;
    if (m_time >= 24*3600) { m_time = 0; } /* day cycles... */
    emit(time_updated(m_time));
}

void PublicTransport::time_changed(const QTime &time)
{
    m_time = time.hour()*3600 + time.minute()*60 + time.second();
    update_vehicles();
    emit(vehicles_updated());
}

void PublicTransport::load_map(const char *filename)
{
    delete_lines();
    map.load_streets(filename);
}

void PublicTransport::load_stops(const char *filename)
{
    delete_lines();
    map.load_stops(filename);
}

void PublicTransport::load_lines(const char *filename)
{
    delete_lines(); // First clear the line vector
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Error: could not open csv line file\n";
        return;
    }

    while(!file.atEnd()) {
        QByteArray line = file.readLine();
        QList<QByteArray> cells = line.split(',');
        unsigned row_len = cells.size();
        /* First, load the line number */
        if (row_len != 1) {
            std::cerr << "Error: invalid line csv - line number has to be specified first\n";
            delete_lines();
            return;
        }

        /* Extract the line number */
        int line_number;
        try {
            line_number = std::stoi(cells.at(0).toStdString());

        } catch (std::invalid_argument const &e) {
            std::cerr << "Error: A line number has to be an integer\n";
            delete_lines();
            return;

        } catch (std::out_of_range const &e) {
            std::cerr << "Error: Line number out of integer range\n";
            delete_lines();
            return;
        }

        /* Now load the streets */
        line = file.readLine();
        cells = line.split(',');
        row_len = cells.size();

        if (row_len == 1) {
            std::cerr << "Error: invalid line csv file format - a line has to have at least one street\n";
            delete_lines();
            return;
        }

        /* Now read the street names and fill the stops list with their corresponding street objects */
        std::vector<Street *> streets;
        for (unsigned i = 0; i < row_len; i++) {
            std::string name = cells.at(i).toStdString();

            /* Trim the whitespace characters from the street name */
            name = strip_whitespace(name);

            /* Look for the street object */
            auto find = map.streets.find(name);
            if (find == map.streets.end()) {
                std::cerr << "Error: The street "<<name<<" doesn't exist\n";
                delete_lines();
                return;
            }

            /* Append the street to the street list */
            streets.push_back(find->second);
        }

        /* Now load the stops */
        line = file.readLine();
        cells = line.split(',');
        row_len = cells.size();

        /* Now read the stop names and fill the stops list with their corresponding stop objects */
        std::vector<Stop *> stops;
        for (unsigned i = 0; i < row_len; i++) {
            std::string name = cells.at(i).toStdString();

            /* Trim the whitespace characters from the stop name */
            name = strip_whitespace(name);

            /* Look for the stop object */
            auto find = map.stops.find(name);
            if (find == map.stops.end()) {
                std::cerr << "Error: The stop "<<name<<" doesn't exist\n";
                delete_lines();
                return;
            }

            /* Append the stop to the stop list */
            stops.push_back(find->second);
        }

        /* Construct the new Line */
        Line *line_ptr = new Line(line_number, get_next_color(), stops, streets, map.streets_by_endpoints);

        /* Load the connections */
        std::vector<Connection *> conns;
        while(!file.atEnd() && (line = file.readLine()) != "\n") {
            cells = line.split(',');
            unsigned row_len_conn = cells.size();
            if (row_len_conn != row_len) {
                std::cerr << "Error: invalid line csv file format - every connection has to visit all stops\n";
                delete_lines();
                return;
            }

            /* Now for each time in the connection schedule, convert it to our unsigned time and
             * add it to a list */
            std::vector<unsigned> times;
            for (auto cell : cells) {
                /* Strip the whitespace characterss */
                std::string cell_str = strip_whitespace(cell.toStdString());
                QTime q_time = QTime::fromString(QString(cell_str.c_str()), "hh:mm");
                if (!q_time.isValid()) {
                    //TODO delete connections
                    std::cerr << "Error: invalid line csv file format - invalid time format: "<<cell.toStdString()<<"\n";
                    delete_lines();
                    return;
                }

                unsigned time = q_time.hour() * 3600 + q_time.minute() * 60;
                times.push_back(time);
            }

            /* Add the new connection to the line */
            Connection *conn = new Connection(line_ptr, stops, times);
            line_ptr->connections.push_back(conn);
        }
        lines.push_back(line_ptr);
    }
}

void PublicTransport::set_clock_rate(unsigned clock_rate)
{
    m_timer->setInterval(DEFAULT_TIMEOUT/clock_rate);
}

void PublicTransport::delete_lines()
{
    for (auto line : lines) {
       delete line;
    }
    lines.clear();
}

void PublicTransport::update_vehicles()
{
    for (auto line : lines) {
        for (auto conn :  line->connections) {
            conn->update_position(get_time());
        }
    }
}

void PublicTransport::prepare_connections()
{
    for (auto line : lines) {
        //TODO try catch
        /* First compute the routes */
        line->compute_route();
        for (auto conn : line->connections) {
            /* Now set the computed routes to the connections */
            conn->update_route();
        }
    }
}

void PublicTransport::start_timer()
{
    m_timer->start();
}

void PublicTransport::stop_timer()
{
    m_timer->stop();
}

int PublicTransport::create_detour(Street *closed, std::vector<Street *> detour)
{
    /* First try to get a substitute set of waypoints */
    std::vector<Waypoint> route = compute_waypoints_detour(closed, detour);

    if (route.empty()) {
        std::cerr << "Detour error: could not create detour\n";
        return 1;
    }

    /* Now replace the closed street in all affected lines */
    closed->close_street();
    for (auto line : lines) {
        line->implement_detour(closed, detour, route);
    }
    return 0;
}

std::vector<Waypoint> PublicTransport::compute_waypoints_detour(Street *closed, std::vector<Street *> detour)
{
    /* The detour has to start on one of these and end on the other one */
    QPoint x1 = closed->start;
    QPoint x2 = closed->end;
    std::vector<Waypoint> route;

    if (detour.size() < 2) {
        std::cerr <<"Detour error: the specified detour was too short to be real\n";
        return route;
    }

    if ((x1 == detour.front()->start || x1 == detour.front()->end)
            && (x2 == detour.back()->start || x2 == detour.back()->end)) {

    } else if ((x2 == detour.front()->start || x2 == detour.front()->end)
            && (x1 == detour.back()->start || x1 == detour.back()->end)) {

        /* reverse the detour vector for consistency */
        std::reverse(detour.begin(), detour.end());

    } else {
        std::cerr << "Detour error: the specified detour did not wrap around the closed street properly\n";
        return route;
    }

    char last_segment;
    if (x1 == detour.front()->start) last_segment = 's';
    else last_segment = 'e';


    route.push_back(Waypoint(x1, closed));

    /* Now check that the detour is continuous and fill the route vector */
    unsigned len = detour.size();
    for (unsigned i = 1; i < len; i++) {

        if (last_segment == 's') {
            /* Compare the endpoint of the previous segment */
            if (detour.at(i)->start == detour.at(i - 1)->end) {
                last_segment = 's';
                route.push_back(Waypoint(detour.at(i)->start, detour.at(i)));

            } else if (detour.at(i)->end == detour.at(i - 1)->end) {
                last_segment = 'e';
                route.push_back(Waypoint(detour.at(i)->end, detour.at(i)));

            } else {
                std::cerr << "Detour error: the specified detour did not connect correctly\n";
                route.clear();
                return route;
            }

        } else {
            /* Compare the startpoint of the previous segment */

            if (detour.at(i)->start == detour.at(i - 1)->start) {
                last_segment = 's';
                route.push_back(Waypoint(detour.at(i)->start, detour.at(i)));

            } else if (detour.at(i)->end == detour.at(i - 1)->start) {
                last_segment = 'e';
                route.push_back(Waypoint(detour.at(i)->end, detour.at(i)));

            } else {
                std::cerr << "Detour error: the specified detour did not connect correctly\n";
                route.clear();
                return route;
            }
        }
    }

    /* Check if the last segment connects to the ending point of the closed street */
    if (last_segment == 's') {
        if (detour.back()->end != x2) {
            std::cerr << "Detour error: the specified detour did not connect correctly\n";
            route.clear();
            return route;
        }

    } else {
        if (detour.back()->start != x2) {
            std::cerr << "Detour error: the specified detour did not connect correctly\n";
            route.clear();
            return route;
        }
    }

    route.push_back(Waypoint(x2, closed));
    return route;
}
