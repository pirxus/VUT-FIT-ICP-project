/**
 * @file Map.cpp
 * @brief
 * @author
 */

#include "Map.h"
#include "Stop.h"
#include <QDebug>
#include <QFile>

void Map::load_streets(const char *filename){
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Error: could not open csv street file\n";
        return;
    }

    //QStringList cells;
    while(!file.atEnd()) {
        QByteArray line = file.readLine();
        QList<QByteArray> cells = line.split(',');

        if (cells.size() != 5) {
            std::cerr << "Error: invalid street csv file format\n";
            return;
        }

        // Extract the coords
        std::vector<int> coords;
        for (unsigned i = 1; i < 5; i++) {
            try {
                int num = std::stoi(cells.at(i).toStdString());
                coords.push_back(num);
            }
            catch (std::invalid_argument const &e) {
                std::cerr << "Bad input: std::invalid_argument thrown" << '\n';
                return;
            }
            catch (std::out_of_range const &e) {
                std::cerr << "Integer is out of range: std::out_of_range thrown" << '\n';
                return;
            }
        }

        /* Add the new street */
        Street *s = new Street(cells[0].toStdString(), coords[0], coords[1], coords[2], coords[3]);
        this->streets.push_back(s);
    }
}

void Map::find_crossroads() {
}

void Map::load_all_stops(const char *filename){
	std::ifstream file(filename);
	std::string line;
	std::vector<std::string> row;
	std::vector<int> coordinates;
	while (getline(file, line)) {
		std::string word;
		row.clear();
		coordinates.clear();
		std::stringstream ss(line);
		while (getline(ss, word, ',')) {
            // add all the column data
            // of a row to a vector
            	row.push_back(word);
        }
        for (unsigned int i = 2; i < row.size();i++){
            		try {
            			int num = std::stoi(row.at(i));
            			coordinates.push_back(num);
            		}
            		//TODO Propagate
            		catch (std::invalid_argument const &e) {
            			std::cerr << "Bad input: std::invalid_argument thrown" << '\n';
            		}
            		catch (std::out_of_range const &e) {
            			std::cerr << "Integer is out of range: std::out_of_range thrown" << '\n';
            		}
            	}
        Stop *s = new Stop(row[0], row[1], coordinates[0], coordinates[1]);
 		this->m_allstops.push_back(s);
    }
}
