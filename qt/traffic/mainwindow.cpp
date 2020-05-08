#include "mainwindow.h"
#include "scene.h"
#include "PublicTransport.h"
#include "ui_mainwindow.h"
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initScene();
    initTraffic();

    connect(ui->actionLoad_map, &QAction::triggered, this, &MainWindow::load_map);
    connect(ui->zoomInButton, &QPushButton::clicked, this, &MainWindow::zoomIn);
    connect(ui->zoomOutButton, &QPushButton::clicked, this, &MainWindow::zoomOut);
    connect(ui->zoomSlider, &QSlider::valueChanged, this, &MainWindow::sliderZoom);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::zoomIn()
{
    this->ui->graphicsView->scale(1.05, 1.05);
}

void MainWindow::zoomOut()
{
    this->ui->graphicsView->scale(0.95, 0.95);
}

void MainWindow::sliderZoom(int z)
{
    auto t = this->ui->graphicsView->transform();
    qreal scale = z / 10.0;
    ui->graphicsView->setTransform(QTransform(scale, t.m12(), t.m21(), scale, t.dx(), t.dy()));
}

void MainWindow::load_map()
{
    QString file = QFileDialog::getOpenFileName(this, "Get Any File");
    if (file == nullptr) return;
    std::string path = QFileInfo(file).absoluteFilePath().toStdString();

    /* Let the PublicTransport module load the map */
    transit.load_map(path.c_str());

    /* And display it... */
    for (auto it = transit.map.streets.begin();
         it != transit.map.streets.end(); it++) {
        auto line = this->scene->addLine((*it)->start.x(), (*it)->start.y(), (*it)->end.x(), (*it)->end.y());
    }
}

void MainWindow::initTraffic()
{
    this->transit = PublicTransport();
}

void MainWindow::initScene()
{
    this->scene = new Scene(ui->graphicsView);
    ui->graphicsView->setScene(this->scene);

    /*
    auto line = scene->addLine(200, 200 , 20, 2);

    line->setPen(QPen({Qt::red}, 3));
    line->setFlag(QGraphicsItem::ItemIsMovable);
    line->setFlag(QGraphicsItem::ItemIsSelectable);

    this->scene->addEllipse(200, 200 , 200, 100);
    */

    ui->graphicsView->setRenderHints(QPainter::Antialiasing);

}

