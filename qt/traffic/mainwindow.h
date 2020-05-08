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

    PublicTransport transit; /**< The controller of the program logic */

private slots:
    void zoomIn();
    void zoomOut();
    void sliderZoom(int z);
    void load_map();
    void load_stops();

private:
    void initTraffic(); /**< Creates a public transport object */
    void initScene();
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
