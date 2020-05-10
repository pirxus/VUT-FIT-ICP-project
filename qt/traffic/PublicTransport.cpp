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
   connect(this->m_timer, &QTimer::timeout, this, &PublicTransport::timer_triggered);
}

PublicTransport::~PublicTransport()
{
    this->delete_lines();
    delete this->m_timer;
}

void PublicTransport::timer_triggered()
{
    this->m_time += TIME_INCREMENT;
    if (this->m_time >= 24*3600) { this->m_time = 0; } /* day cycles... */
    emit(this->time_updated(this->m_time));
}

void PublicTransport::time_changed(const QTime &time)
{
    this->m_time = time.hour()*3600 + time.minute()*60 + time.second();
    this->update_vehicles();
    emit(this->vehicles_updated());
}

void PublicTransport::load_map(const char *filename)
{
    this->map.load_streets(filename);
}

void PublicTransport::load_stops(const char *filename)
{
    this->map.load_stops(filename);
}

void PublicTransport::load_lines(const char *filename)
{
    //delete_lines(); // First clear the line vector
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
            this->delete_lines();
            return;
        }

        /* Extract the line number */
        int line_number;
        try {
            line_number = std::stoi(cells.at(0).toStdString());

        } catch (std::invalid_argument const &e) {
            std::cerr << "Error: A line number has to be an integer\n";
            this->delete_lines();
            return;

        } catch (std::out_of_range const &e) {
            std::cerr << "Error: Line number out of integer range\n";
            this->delete_lines();
            return;
        }

        /* Now load the streets */
        line = file.readLine();
        cells = line.split(',');
        row_len = cells.size();

        if (row_len == 1) {
            std::cerr << "Error: invalid line csv file format - a line has to have at least one street\n";
            this->delete_lines();
            return;
        }

        /* Now read the street names and fill the stops list with their corresponding street objects */
        std::vector<Street *> streets;
        for (unsigned i = 0; i < row_len; i++) {
            std::string name = cells.at(i).toStdString();

            /* Trim the whitespace characters from the street name */
            name = strip_whitespace(name);

            /* Look for the street object */
            auto find = this->map.streets.find(name);
            if (find == this->map.streets.end()) {
                std::cerr << "Error: The street "<<name<<" doesn't exist\n";
                this->delete_lines();
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
            auto find = this->map.stops.find(name);
            if (find == this->map.stops.end()) {
                std::cerr << "Error: The stop "<<name<<" doesn't exist\n";
                this->delete_lines();
                return;
            }

            /* Append the stop to the stop list */
            stops.push_back(find->second);
        }

        /* Construct the new Line */
        Line *line_ptr = new Line(line_number, get_next_color(), stops, streets);

        /* Load the connections */
        std::vector<Connection *> conns;
        while(!file.atEnd() && (line = file.readLine()) != "\n") {
            cells = line.split(',');
            unsigned row_len_conn = cells.size();
            if (row_len_conn != row_len) {
                std::cerr << "Error: invalid line csv file format - every connection has to visit all stops\n";
                this->delete_lines();
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
                    this->delete_lines();
                    return;
                }

                unsigned time = q_time.hour() * 3600 + q_time.minute() * 60;
                times.push_back(time);
            }

            /* Add the new connection to the line */
            Connection *conn = new Connection(line_ptr, stops, times);
            line_ptr->connections.push_back(conn);
        }
        this->lines.push_back(line_ptr);
    }

    /* Start the simulation */
    this->m_timer->start();
}

void PublicTransport::delete_lines()
{
    for (auto line : this->lines) {
       delete line;
    }
    this->lines.clear();
}

void PublicTransport::update_vehicles()
{
    for (auto line : this->lines) {
        for (auto conn :  line->connections) {
            conn->update_position(this->get_time());
        }
    }
}

void PublicTransport::prepare_connections()
{
    for (auto line : this->lines) {
        //TODO try catch
        /* First compute the routes */
        line->compute_route();
        for (auto conn : line->connections) {
            /* Now set the computed routes to the connections */
            conn->update_route();
        }
    }
}



QColor get_next_color()
{
    /**< This is an index to the color pallete for our lines. */
    static unsigned color_index = 0;
    return line_colors[color_index++ % 10];
}
