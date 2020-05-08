﻿/**
 * @file Map.cpp
 * @brief
 * @author
 */

#include "Map.h"
#include "Stop.h"
#include <QDebug>
#include <QFile>

Map::~Map()
{
    this->delete_streets();
    this->delete_stops();
}

void Map::delete_streets()
{
   for (auto el : this->streets) {
       delete el.second;
   }
   this->streets.clear();
}

void Map::delete_stops()
{
   for (auto el : this->stops) {
       delete el;
   }
   this->stops.clear();
}

void Map::load_streets(const char *filename){
    delete_streets(); // First clear the street map
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
            this->delete_streets();
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
                this->delete_streets();
                return;

            } catch (std::out_of_range const &e) {
                std::cerr << "Integer is out of range: std::out_of_range thrown" << '\n';
                this->delete_streets();
                return;
            }
        }

        /* Add the new street */
        Street *s = new Street(cells[0].toStdString(), coords[0], coords[1], coords[2], coords[3]);
        this->streets.insert(std::pair<std::string, Street *>(cells[0].toStdString(), s));
    }
}

void Map::find_crossroads() {
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
            this->delete_stops();
            return;
        }

        // Extract the stop position
        float pos;
        try {
            pos = std::stof(cells.at(2).toStdString());

        } catch (std::invalid_argument const &e) {
            std::cerr << "Error: stop csv - stop position was not of type float\n";
            this->delete_stops();
            return;

        } catch (std::out_of_range const &e) {
            std::cerr << "Error: stop csv, stop position - float out of range\n";
            this->delete_stops();
            return;
        }

        if (pos <= 0.0 || pos >= 1.0) {
            std::cerr << "Error: stop csv - stop position value has to be in (0, 1)\n";
            this->delete_stops();
            return;
        }

        /* Find the corresponding street */
        auto street = this->streets.find(cells[0].toStdString());
        if (street == this->streets.end()) {
            std::cerr << "Error: stop csv - the street "<<cells[0].toStdString()<<" does not exist. Please check if all streets are loaded\n";
            this->delete_stops();
            return;
        }
        /* Add the new street */
        Stop *s = new Stop(street->second, cells[1].toStdString(), pos);
        this->stops.push_back(s);
    }
}