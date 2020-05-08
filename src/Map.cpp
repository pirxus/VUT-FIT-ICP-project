/**
 * @file Map.cpp
 * @brief
 * @author
 */

#include "Map.h"

void Map::load_streets(const char *filename){
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
        for (unsigned i = 1; i < row.size();i++){
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
        Street *s = new Street(row[0], coordinates[0], coordinates[1], coordinates[2], coordinates[3]);
 		this->m_streets.push_back(s);
    }
}

void Map::find_crossroads() {
}

void Map::load_allstopss(const char *filename){
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
        for (unsigned i = 2; i < row.size();i++){
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
        Stops *s = new Stop(row[0], row[1], coordinates[0], coordinates[1]);
 		this->m_allstops.push_back(s);
    }
}
