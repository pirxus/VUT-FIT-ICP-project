/**
 * @file PublicTransport.cpp
 * @brief
 * @author
 */

#include "PublicTransport.h"


void PublicTransport::load_map(const char *filename){
    this->map = Map();
    this->map.load_streets(filename);
    //load_allstops(filename);
}

//void PublicTransport::load_lines(const char *filename, std::vector<Stop *> all_stops){
//    std::ifstream file(filename); //TODO check if opens
//    std::string line;
//    int i = 0;
//    std::vector<std::string> row;
//    int num;
//
//    while(getline(file, line)){
//        row.clear();
//        //i met the end of set
//        if (line.length() == 0){
//            //call the constructor
//            Line *l = new Line(num, m_stops, m_schedule);
//            this->m_lines.push_back(l);
//            //get first line of new set
//            getline(file, line);
//        }
//        //parsing first lines of each set of lines and connections
//        if (i == 0){
//            std::string word;
//            std::stringstream ss(line);
//            while (getline(ss, word, ',')) {
//                // add all the column data
//                // of a row to a vector
//                row.push_back(word);
//            }
//            //TODO Propagate
//            try {
//                num = std::stoi(row.at(0));
//            }
//            catch (std::invalid_argument const &e) {
//                std::cerr << "Bad input: std::invalid_argument thrown" << '\n';
//            }
//            catch (std::out_of_range const &e) {
//                std::cerr << "Integer is out of range: std::out_of_range thrown" << '\n';
//            }
//            //push stops to the stop vector
//            unsigned x = row.size();
//            unsigned y = all_stops.size();
//            for (unsigned i = 1; i < x; i++){
//                for (unsigned j = 0; j < y; j++){
//                    if (row.at(i) == all_stops.at(j)->name()){
//                        this->m_stops.push_back(all_stops.at(j));
//                    }
//                }
//            }
//        }
//        //parsing rest of lines of each set of lines and connections
//        else {
//            std::string word;
//            std::stringstream ss(line);
//            while (getline(ss, word, ',')) {
//                // add all the column data
//                // of a row to a vector
//                row.push_back(word);
//            }
//        unsigned x = row.size();
//        //push pairs to the list
//        for (unsigned int i; i < x; i++){
//            this->m_schedule.push_back(m_stops.at(i), row.at(i));
//            }
//        }
//        i = 1;
//    }
//}
