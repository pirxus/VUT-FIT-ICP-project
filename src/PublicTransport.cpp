/**
 * @file PublicTransport.cpp
 * @brief
 * @author
 */

#include "PublicTransport.h"


void load_map(const char *filename){
	load_streets(filename);
	load_allstops(filename);
}

void load_alllines(const char *filename){
	load_lines(filename);
}