#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "PublicTransport.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void zoomIn();
    void zoomOut();
    void sliderZoom(int z);
    void load_map();
    void load_stops();
    void load_lines();


private:
    void initTraffic(); /**< Creates a public transport object */
    void initScene();
    PublicTransport *transit; /**< The controller of the program logic */
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
