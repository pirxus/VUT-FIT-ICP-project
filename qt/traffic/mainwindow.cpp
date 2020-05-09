﻿#include "mainwindow.h"
#include "scene.h"
#include "PublicTransport.h"
#include "Connection.h"
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

    /* Setup button signals */
    connect(ui->zoomInButton, &QPushButton::clicked, this, &MainWindow::zoomIn);
    connect(ui->zoomOutButton, &QPushButton::clicked, this, &MainWindow::zoomOut);
    connect(ui->zoomSlider, &QSlider::valueChanged, this, &MainWindow::sliderZoom);

    /* Setup top menu actions */
    connect(ui->actionLoad_map, &QAction::triggered, this, &MainWindow::load_map);
    connect(ui->actionLoad_stops, &QAction::triggered, this, &MainWindow::load_stops);
    connect(ui->actionLoad_lines, &QAction::triggered, this, &MainWindow::load_lines);

    /* Vehicle updates */
    connect(transit, &PublicTransport::vehicles_updated, this, &MainWindow::positions_updated);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete transit;
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
    transit->load_map(path.c_str());

    /* And display it... */
    for (auto it = transit->map.streets.begin(); // Streets
        it != transit->map.streets.end(); it++) {
        auto line = this->ui->graphicsView->scene()->addLine((*it).second->start.x(), (*it).second->start.y(),
                     (*it).second->end.x(), (*it).second->end.y());

        line->setPen(QPen({Qt::black}, 3));

        /* Set street name labels.. */
        auto text = this->ui->graphicsView->scene()->addText(QString((*it).second->name.c_str()));
        text->setPos(((*it).second->start.x() + (*it).second->end.x()) / 2.0,
                     ((*it).second->start.y() + (*it).second->end.y()) / 2.0);
    }
}

void MainWindow::load_stops()
{
    QString file = QFileDialog::getOpenFileName(this, "Get Any File");
    if (file == nullptr) return;
    std::string path = QFileInfo(file).absoluteFilePath().toStdString();

    /* Let the PublicTransport module load the stops */
    transit->load_stops(path.c_str());
    for (auto it = transit->map.stops.begin(); // Stops
         it != transit->map.stops.end(); it++) {
        this->ui->graphicsView->scene()->addEllipse(
                    (*it).second->pos.x()-5, (*it).second->pos.y()-5, 10, 10,
                    QPen({Qt::red}, 3), QBrush(Qt::black, Qt::SolidPattern));

        auto text = this->ui->graphicsView->scene()->addText(QString((*it).second->name().c_str()));
        text->setPos((*it).second->pos.x() + 2, (*it).second->pos.y() + 2.0);
    }
}

void MainWindow::load_lines()
{
    QString file = QFileDialog::getOpenFileName(this, "Get Any File");
    if (file == nullptr) return;
    std::string path = QFileInfo(file).absoluteFilePath().toStdString();

    /* Let the PublicTransport module load the lines */
    transit->load_lines(path.c_str());
    transit->prepare_connections();
}

void MainWindow::positions_updated()
{
    for (auto line : transit->lines) {
        for (auto conn : line->connections) {
            if (conn->active) {
                this->ui->graphicsView->scene()->addEllipse(
                    conn->get_pos().x()-4, conn->get_pos().y()-5, 8, 8,
                    QPen({Qt::blue}, 1), QBrush(Qt::yellow, Qt::SolidPattern));
            }
        }
    }
}

void MainWindow::initTraffic()
{
    this->transit = new PublicTransport(this);
}

void MainWindow::initScene()
{
    auto scene = new Scene(ui->graphicsView);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing);

    /*
    auto line = scene->addLine(200, 200 , 20, 2);

    line->setPen(QPen({Qt::red}, 3));
    line->setFlag(QGraphicsItem::ItemIsMovable);
    line->setFlag(QGraphicsItem::ItemIsSelectable);

    this->scene->addEllipse(200, 200 , 200, 100);
    */
}

