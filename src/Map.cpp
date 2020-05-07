/**
 * @file Map.cpp
 * @brief
 * @author
 */

#include "Map.h"

/**
 * @param filename
 *
 */
void Map::load_streets(const char *filename){
	std::ifstream file(filename);
	std::string line;
	std::vector<std::string> row;
	std::vector<int> coords;
	while (getline(file, line)) {
		std::string word;
		row.clear();
		std::stringstream ss(line); 
		while (getline(ss, word, ',')) { 
            // add all the column data 
            // of a row to a vector 
            	row.push_back(word);
            	for (unsigned int i = 1; i < row.size();i++){
            		int num = atoi(row.at(i).c_str());
            		coords.push_back(num);
            	}
           		Street *s = new Street(row[0], num[0], num[1], num[2], num[3]);
 				this->m_streets.push_back(s);
        }
    }
}