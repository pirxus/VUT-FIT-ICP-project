﻿/**
 * @file Map.cpp
 * @brief This module represents the the map.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 */

#include "Map.h"
#include "Stop.h"
#include "utilities.h"
#include <QDebug>
#include <QFile>
#include <string>
#include <regex>

Map::~Map()
{
    delete_streets();
    delete_stops();
}

void Map::delete_streets()
{
   for (auto el : streets) {
       delete el.second;
   }
   streets.clear();
}

void Map::delete_stops()
{
   for (auto el : stops) {
       delete el.second;
   }
   stops.clear();
}

void Map::load_streets(const char *filename){
    delete_streets(); // First clear the street map
    delete_stops();
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Error: could not open csv street file\n";
        return;
    }

    while(!file.atEnd()) {
        QByteArray line = file.readLine();
        QList<QByteArray> cells = line.split(',');

        if (cells.size() != 5) {
            std::cerr << "Error: invalid street csv file format\n";
            delete_streets();
            return;
        }

        // Extract the coords
        std::vector<int> coords;
        for (unsigned i = 1; i < 5; i++) {
            try {
                int num = std::stoi(cells.at(i).toStdString());
                coords.push_back(num);

            } catch (std::invalid_argument const &e) {
                std::cerr << "Bad input: std::invalid_argument thrown" << '\n';
                delete_streets();
                return;

            } catch (std::out_of_range const &e) {
                std::cerr << "Integer is out of range: std::out_of_range thrown" << '\n';
                delete_streets();
                return;
            }
        }

        /* Trim the whitespace characters from the street name */
        std::string street_name = strip_whitespace(cells[0].toStdString());

        /* Check for duplicate streets */
        auto street = streets.find(street_name);
        if (street != streets.end()) {
            std::cerr << "Error: stop csv - the street "<<street_name<<" already exists\n";
            delete_streets();
            return;
        }

        /* Add the new street */
        Street *s = new Street(street_name, coords[0], coords[1], coords[2], coords[3]);
        streets.insert(std::pair<std::string, Street *>(street_name, s));
        streets_by_endpoints.insert(
                    std::pair<std::tuple<int, int, int, int>, Street *>(
                        std::tuple<int, int, int, int>(s->start.x(), s->start.y(), s->end.x(), s->end.y()), s));
    }
}

void Map::load_stops(const char *filename){
    delete_stops(); // First clear the stop vector
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Error: could not open csv stops file\n";
        return;
    }

    while(!file.atEnd()) {
        QByteArray line = file.readLine();
        QList<QByteArray> cells = line.split(',');

        if (cells.size() != 3) {
            std::cerr << "Error: invalid stops csv file format\n";
            delete_stops();
            return;
        }

        // Extract the stop position
        float pos;
        try {
            pos = std::stof(strip_whitespace(cells.at(2).toStdString()));

        } catch (std::invalid_argument const &e) {
            std::cerr << "Error: stop csv - stop position was not of type float\n";
            delete_stops();
            return;

        } catch (std::out_of_range const &e) {
            std::cerr << "Error: stop csv, stop position - float out of range\n";
            delete_stops();
            return;
        }

        if (pos <= 0.0 || pos >= 1.0) {
            std::cerr << "Error: stop csv - stop position value has to be in (0, 1)\n";
            std::cerr << pos << cells.at(0).toStdString() << cells.at(1).toStdString() << "\n";
            delete_stops();
            return;
        }

        /* Find the corresponding street */
        auto street = streets.find(cells[0].toStdString());
        if (street == streets.end()) {
            std::cerr << "Error: stop csv - the street "<<cells[0].toStdString()<<" does not exist. Please check if all streets are loaded\n";
            delete_stops();
            return;
        }

        /* Trim the whitespace characters from the stop name */
        std::string stop_name = strip_whitespace(cells[1].toStdString());

        /* Check for duplicate names */
        auto find = stops.find(stop_name);
        if (find != stops.end()) {
            std::cerr << "Error: The stop "<<stop_name<<" already exists\n";
            delete_stops();
            return;
        }

        /* Add the new stop */
        Stop *s = new Stop(street->second, stop_name, pos);
        stops.insert(std::pair<std::string, Stop *>(s->name(), s));
    }
}
