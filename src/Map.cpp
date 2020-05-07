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
void load_streets(const char *filename){
	ifstream file(filename);
	string line;
	vector<string> row;
	while (getline(file, line)) {
		string word;
		row.clear();;
		stringstream ss(line); 
		while (getline(ss, word, ',')) { 
            // add all the column data 
            // of a row to a vector 
            	row.push_back(word);
        	}
        	Street s(row[0], row[1], row[2], row[3], row[4]);
        	/*
        	for (unsigned int i = 0; i < row.size(); i++){
        			cout << row.at(i) << endl;
        		} 
        		*/
    	}
}