/**
 * @file main.cpp
 * @brief This is the main module of the program.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 *
 * This is the main module of the program. The only purpose of this module is to
 * create and initialize the QApplication and MainWindow objects and then enter the main
 * program loop.
 */

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
