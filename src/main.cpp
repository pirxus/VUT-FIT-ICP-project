/**
 * @file main.cpp
 * @brief
 * @author
 */

#include <iostream>

#include "Coords.h"
#include "Stop.h"
#include "Street.h"

using namespace std;

int main() {
	cout << "Hello World!\n";
    Coords p;
    Coords e(19, 10);
    p.x =1;
    p.y =2;
    Coords a = e + p;

    bool hello = p == p;
    cout << p.x << " " << p.y << endl;
    cout << e.x << " " << e.y << endl;
    cout << a.x << " " << a.y << endl;
    cout << hello << endl;
}
