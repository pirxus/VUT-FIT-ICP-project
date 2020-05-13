#include "mainwindow.h"
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

    street_closing_mode = false;

    /* Setup button signals */
    connect(ui->zoomInButton, &QPushButton::clicked, this, &MainWindow::zoomIn);
    connect(ui->zoomOutButton, &QPushButton::clicked, this, &MainWindow::zoomOut);
    connect(ui->zoomSlider, &QSlider::valueChanged, this, &MainWindow::sliderZoom);

    connect(ui->buttonPause, &QPushButton::clicked, this, &MainWindow::toggle_play_pause);

    /* Setup top menu actions */
    connect(ui->actionLoad_map, &QAction::triggered, this, &MainWindow::load_map);
    connect(ui->actionLoad_stops, &QAction::triggered, this, &MainWindow::load_stops);
    connect(ui->actionLoad_lines, &QAction::triggered, this, &MainWindow::load_lines);

    /* Vehicle updates */
    connect(transit, &PublicTransport::vehicles_updated, this, &MainWindow::positions_updated);
    connect(ui->timeEdit, &QTimeEdit::timeChanged, this->transit, &PublicTransport::time_changed);
    connect(transit, &PublicTransport::time_updated, this, &MainWindow::update_time);

    /* Streets */
    connect(scene, &Scene::street_selected, this, &MainWindow::street_selected);
    connect(scene, &Scene::street_unselected, this, &MainWindow::street_unselected);
    connect(ui->spinTraffic, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MainWindow::traffic_situation_changed);

    /* Street closing */
    connect(scene, &Scene::street_canceled, this, &MainWindow::street_cancelled);
    connect(ui->buttonBoxClose, &QDialogButtonBox::rejected, this, &MainWindow::cancel_street_cancel);
    connect(ui->buttonBoxClose, &QDialogButtonBox::accepted, this, &MainWindow::accept_street_cancel);

    connect(scene, &Scene::display_itinerary, this, &MainWindow::display_itinerary);
    connect(scene, &Scene::clear_itinerary, this, &MainWindow::clear_itinerary);

    connect(ui->showStreetNames, &QCheckBox::stateChanged, this, &MainWindow::display_street_name);
    connect(ui->showStopNames, &QCheckBox::stateChanged, this, &MainWindow::display_stop_name);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete transit;
}

void MainWindow::zoomIn()
{
    this->ui->graphicsView->scale(1.25, 1.25);
    this->ui->zoomSlider->setValue(this->ui->zoomSlider->value()+1);
}

void MainWindow::zoomOut()
{
    this->ui->graphicsView->scale(0.75, 0.75);
    this->ui->zoomSlider->setValue(this->ui->zoomSlider->value()-1);
}

void MainWindow::sliderZoom(int z)
{
    auto t = this->ui->graphicsView->transform();
    qreal scale = z / 50.0;
    ui->graphicsView->setTransform(QTransform(scale, t.m12(), t.m21(), scale, t.dx(), t.dy()));
}

void MainWindow::load_map()
{
    //QString file = QFileDialog::getOpenFileName(this, "Get Any File");
    //if (file == nullptr) return;
    //std::string path = QFileInfo(file).absoluteFilePath().toStdString();
    std::string path = QFileInfo("../../examples/streetList.csv").absoluteFilePath().toStdString();

    /* Let the PublicTransport module load the map */
    transit->load_map(path.c_str());

    /* And add them to the scene... */
    for (auto it = transit->map.streets.begin(); // Streets
        it != transit->map.streets.end(); it++) {
        this->scene->add_street((*it).second);
        /*
        // LABELS
        auto text = this->ui->graphicsView->scene()->addText(QString((*it).second->name.c_str()));
        text->setPos(((*it).second->start.x() + (*it).second->end.x()) / 2.0,
                     ((*it).second->start.y() + (*it).second->end.y()) / 2.0);
                     */
    }
}

void MainWindow::load_stops()
{
    //QString file = QFileDialog::getOpenFileName(this, "Get Any File");
    //if (file == nullptr) return;
    //std::string path = QFileInfo(file).absoluteFilePath().toStdString();
    std::string path = QFileInfo("../../examples/stopList.csv").absoluteFilePath().toStdString();

    /* Let the PublicTransport module load the stops */
    transit->load_stops(path.c_str());
    for (auto it = transit->map.stops.begin(); // Stops
         it != transit->map.stops.end(); it++) {
        this->scene->add_stop((*it).second);
        /*
        auto text = this->ui->graphicsView->scene()->addText(QString((*it).second->name().c_str()));
        text->setPos((*it).second->pos.x() + 2, (*it).second->pos.y() + 2.0);
        */
    }
}

void MainWindow::load_lines()
{
    //QString file = QFileDialog::getOpenFileName(this, "Get Any File");
    //if (file == nullptr) return;
    //std::string path = QFileInfo(file).absoluteFilePath().toStdString();
    std::string path = QFileInfo("../../examples/lineList.csv").absoluteFilePath().toStdString();

    /* Let the PublicTransport module load the lines */
    transit->load_lines(path.c_str());
    transit->prepare_connections();

    /* Add the loaded connections to our scene */
    for (auto line : transit->lines) {
        for (auto conn : line->connections) {
            this->scene->add_connection(conn);
        }
    }
}

void MainWindow::positions_updated()
{
    this->scene->redraw_connections();
}

void MainWindow::update_time(unsigned time)
{
    this->ui->timeEdit->setTime(QTime(0, 0).addSecs(time));
}

void MainWindow::toggle_play_pause()
{
    if (this->ui->buttonPause->text() == "Pause") {
        this->transit->stop_timer();
        this->ui->buttonPause->setText("Play");
    } else {
        this->transit->start_timer();
        this->ui->buttonPause->setText("Pause");
    }
}

void MainWindow::street_selected(Street *street)
{
    /* Enable the traffic intensity counter and get the current value */
    this->ui->spinTraffic->setEnabled(true);
    this->currently_edited_street = street;
    this->ui->spinTraffic->setValue(street->get_traffic());
}

void MainWindow::street_unselected(Street *street)
{
    /* Disable the traffic intensity counter */
    this->ui->spinTraffic->setDisabled(true);
    if (this->currently_edited_street == street) {
        this->currently_edited_street = nullptr;
    }
}

void MainWindow::traffic_situation_changed(int level)
{
    this->currently_edited_street->set_traffic(level);
}

void MainWindow::street_cancelled(ViewStreet *street)
{
    currently_cancelled_street = street;
    street_closing_mode = true;

    /* Block some UI elements for the time being */
    ui->buttonPause->setEnabled(false);
    ui->spinTraffic->setEnabled(false);
    ui->timeEdit->setEnabled(false);

    ui->buttonBoxClose->setEnabled(true);
    scene->setBackgroundBrush(QBrush(QColor{Qt::gray}, Qt::SolidPattern));
    ui->labelClose->setText("Please specify the detour for the closed street. Choose the streets in order.");
    /* Wait for detour specification */
    scene->prepare_for_detour();
}

void MainWindow::cancel_street_cancel()
{
    /* Inform the scene */
    scene->end_detour_selection(false);

    /* Reset to the normal state */
    restore_after_cancel();
}

void MainWindow::accept_street_cancel()
{
    /* First extract just the pointers to the street objects from the ViewStreet objects */
    std::vector<Street *> detour_streets;
    detour = scene->get_detour();
    for (auto street : detour) {
        detour_streets.push_back(street->get_street());
    }

    /* Try to setup the detour */
    bool correct;
    correct = !this->transit->create_detour(currently_cancelled_street->get_street(), detour_streets);

    if (correct) currently_cancelled_street->get_street()->close_street();
    /* Inform the scene */
    scene->end_detour_selection(correct);

    /* Reset to the normal state */
    restore_after_cancel();
}

void MainWindow::restore_after_cancel()
{
    detour.clear();
    ui->buttonPause->setEnabled(true);
    ui->spinTraffic->setEnabled(true);
    ui->timeEdit->setEnabled(true);

    currently_cancelled_street = nullptr;
    street_closing_mode = true;

    ui->buttonBoxClose->setEnabled(false);
    scene->setBackgroundBrush(QBrush(QColor{Qt::white}, Qt::SolidPattern));
    ui->labelClose->setText("In order to close a street, choose one and double-click it.");

}

void MainWindow::display_itinerary(Connection *conn)
{
     auto schedule = conn->get_schedule();
     this->ui->currentDelay->setText("Current delay: ");
     int delay = conn->get_delay();
     this->ui->delayValue->setText(QString::number(delay));
     this->ui->nextStop->setText("Next stop: ");
     int index = conn->find_schedule_index(transit->get_time() - delay);
     this->ui->nextStopName->setText(QString(schedule.at(index + 1).first->name().c_str()));
     this->ui->lineNumber->setText("Line number: ");
     int line_number = conn->get_line()->get_line_number();
     this->ui->lineNumberValue->setText(QString::number(line_number));

     int sch_size = schedule.size() - 1;
     int line_cord = -60 * sch_size;
     auto line = itineraryScene->addLine(0, 0, 0, line_cord);
     line->setPen(QPen(Qt::gray, 10, Qt::SolidLine, Qt::RoundCap));
     for (int i = 0; i < sch_size + 1; i++) {
         auto text = itineraryScene->addText(QString(schedule.at(i).first->name().c_str()));
         text->setPos(20, -15 - 60 * i);
         text->setFont(QFont("Arial" , 10));

         if (i > index){
            auto stop = itineraryScene->addEllipse(-10, -10 - 60 * i, 20, 20);
            stop->setBrush(QBrush(QColor{Qt::white}, Qt::SolidPattern));
            stop->setPen(QPen({Qt::black}, 2));
         }
         else {
            auto stop = itineraryScene->addEllipse(-10, -10 - 60 * i, 20, 20);
            stop->setBrush(QBrush(QColor{Qt::green}, Qt::SolidPattern));
            stop->setPen(QPen({Qt::black}, 2));
         }
     }
}

void MainWindow::clear_itinerary()
{
    this->ui->currentDelay->clear();
    this->ui->nextStop->clear();
    this->ui->lineNumber->clear();
    this->ui->delayValue->clear();
    this->ui->lineNumberValue->clear();
    this->ui->nextStopName->clear();
    qDeleteAll(itineraryScene->items());
}

void MainWindow::display_street_name()
{
        if (this->ui->showStreetNames->isChecked()){
        // And add them to the scene...
            for (auto it = transit->map.streets.begin(); it != transit->map.streets.end(); it++) {
                // LABELS
                auto text = this->ui->graphicsView->scene()->addText(QString((*it).second->name.c_str()));
                text->setFont(QFont("Arial" , 9));
                text->setDefaultTextColor(Qt::red);
                text->setPos(((*it).second->start.x() + (*it).second->end.x()) / 2.0,
                            ((*it).second->start.y() + (*it).second->end.y()) / 2.0);
             }
        }
        else {
            // remove them from the scene//TODO
            for (auto it = transit->map.streets.begin(); it != transit->map.streets.end(); it++) {
                // LABELS
                auto text = this->ui->graphicsView->scene()->addText(QString((*it).second->name.c_str()));
                text->setDefaultTextColor(Qt::white);
                text->setPos(((*it).second->start.x() + (*it).second->end.x()) / 2.0,
                             ((*it).second->start.y() + (*it).second->end.y()) / 2.0);
            }
         }
}

void MainWindow::display_stop_name()
{
        if (this->ui->showStopNames->isChecked()){
            for (auto it = transit->map.stops.begin(); it != transit->map.stops.end(); it++) {
                auto text = this->ui->graphicsView->scene()->addText(QString((*it).second->name().c_str()));
                text->setDefaultTextColor(Qt::blue);
                text->setFont(QFont("Arial" , 9));
                text->setPos((*it).second->pos.x() + 2, (*it).second->pos.y() + 2.0);
            }
        }
        else {
            // remove them from the scene//TODO
            for (auto it = transit->map.stops.begin(); it != transit->map.stops.end(); it++) {
                auto text = this->ui->graphicsView->scene()->addText(QString((*it).second->name().c_str()));
                text->setDefaultTextColor(Qt::white);
                text->setPos((*it).second->pos.x() + 2, (*it).second->pos.y() + 2.0);
            }
        }
}

void MainWindow::initTraffic()
{
    this->transit = new PublicTransport(this);
}

void MainWindow::initScene()
{
    this->scene = new Scene(ui->graphicsView);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing);
    this->itineraryScene = new Scene(ui->itineraryView);
    ui->itineraryView->setScene(itineraryScene);
    ui->itineraryView->setRenderHints(QPainter::Antialiasing);

    /*
    auto line = scene->addLine(200, 200 , 20, 2);

    line->setPen(QPen({Qt::red}, 3));
    line->setFlag(QGraphicsItem::ItemIsMovable);
    line->setFlag(QGraphicsItem::ItemIsSelectable);

    this->scene->addEllipse(200, 200 , 200, 100);
    */
}

