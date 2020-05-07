/**
 * @file PublicTransport.cpp
 * @brief
 * @author
 */

#include "PublicTransport.h"


void load_map(){
	const char filename[] = "streetList.csv";
	load_streets(filename);
}

void load_lines(){
	const char filename[] = "testDataStreet.csv";
	load_lines(filename);
}