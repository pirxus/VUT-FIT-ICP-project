/**
 * @file Line.cpp
 * @brief
 * @author
 */

#include "Line.h"

void Line::add_stop(Stop *stop) {
}
void Line::add_connection(Connection *conn) {
}
void Line::set_color(char color) {
}

void load_lines(const char *filename, std::vector<Stop *> all_stops){
	std::ifstream file(filename);
	std::string line;
	std::vector<std::string> row;
	int num;
	while(getline(file, line)){
		std::string word;
		row.clear();
		std::stringstream ss(line); 
		while (getline(ss, word, ',')) { 
            // add all the column data 
            // of a row to a vector 
            	row.push_back(word);
        }
            	try {
            		num = std::stoi(row.at(0));
            	}
           		catch (std::invalid_argument const &e) {
           			std::cerr << "Bad input: std::invalid_argument thrown" << '\n';
           		}
           		catch (std::out_of_range const &e) {
           			std::cerr << "Integer is out of range: std::out_of_range thrown" << '\n';
           		}
           		unsigned x = row.size();
           		unsigned y = all_stops.size();
           		for (unsigned i = 1; i < x; i++){
           			for (unsigned j = 0; j < y; j++){
           				if (row.at(i) == all_stops.at(j)->name){
           					this->m_stops.push_back(all_stops.at(j));
           				}
           			}
           		}
        	while (getline(file, line)){
        		if (line.length() == 0){
        			break;
        		}
        	}
}