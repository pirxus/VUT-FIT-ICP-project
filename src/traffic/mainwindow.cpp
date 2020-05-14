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
    m_itinerary = nullptr;

    /* Setup button signals */
    connect(ui->zoomInButton, &QPushButton::clicked, this, &MainWindow::zoomIn);
    connect(ui->zoomOutButton, &QPushButton::clicked, this, &MainWindow::zoomOut);
    connect(ui->zoomSlider, &QSlider::valueChanged, this, &MainWindow::sliderZoom);

    connect(ui->buttonPause, &QPushButton::clicked, this, &MainWindow::toggle_play_pause);
    connect(ui->sliderSpeed, &QSlider::valueChanged, this, &MainWindow::slider_speed);

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
    }
    ui->actionLoad_map->setEnabled(false);
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
    }
    ui->actionLoad_stops->setEnabled(false);
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
    ui->actionLoad_lines->setEnabled(false);
}

void MainWindow::positions_updated()
{
    /* Redraw all vehicles */
    this->scene->redraw_connections();

    /* Update the displayed itinerary for the selected vehicle */
    if (m_itinerary != nullptr) {
        Connection *tmp = m_itinerary;
        clear_itinerary();
        display_itinerary(tmp);
    }
}

void MainWindow::update_time(unsigned time)
{
    this->ui->timeEdit->setTime(QTime(0, 0).addSecs(time));
}

void MainWindow::slider_speed()
{
    unsigned slider_value = this->ui->sliderSpeed->value();
    this->transit->set_clock_rate(slider_value);
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
    m_itinerary = conn;
     auto schedule = conn->get_schedule();
     if (schedule.empty()) return; /* Avoid segfaults, nothing to display... */
     ui->currentDelay->setText("Current delay: ");
     int sch_index = conn->find_schedule_index(transit->get_time());
     int delay = conn->get_delay(sch_index);
     int line_number = conn->get_line()->get_line_number();

     ui->delayValue->setText(QString::number(delay/60));
     ui->nextStop->setText("Next stop: ");
     ui->nextStopName->setText(QString(std::get<0>(schedule.at(sch_index + 1))->name().c_str()));
     ui->lineNumber->setText("Line number: ");
     ui->lineNumberValue->setText(QString::number(line_number));

     /* Add a white ellipse as padding */
     auto padding = itineraryScene->addEllipse(0, -30, 1, 1);
     padding->setBrush(QBrush(QColor{Qt::white}, Qt::SolidPattern));
     padding->setPen(QPen({Qt::white}, 0));

     int sch_size = schedule.size() - 1;
     int line_cord = 60 * sch_size;
     auto line = itineraryScene->addLine(0, 0, 0, line_cord);
     line->setPen(QPen(Qt::darkGray, 5, Qt::SolidLine, Qt::RoundCap));
     for (int i = 0; i < sch_size + 1; i++) {
         /* Show the name of the stop */
         auto text = itineraryScene->addText(QString(std::get<0>(schedule.at(i))->name().c_str()));
         text->setPos(20, -10 + 60 * i);
         text->setFont(QFont("Arial" , 10));

         unsigned stop_delay = std::get<2>(schedule.at(i));
         /* Set the time for the stop */
         if (delay != 0 && i > sch_index) { /* Add the information about the current delay */
             text = itineraryScene->addText(QTime(0, 0).addSecs(std::get<1>(schedule.at(i))).toString("hh:mm"));
             text->setPos(-60, -15 + 60 * i);
             text->setFont(QFont("Arial" , 10));

             /* Delay */
             text = itineraryScene->addText("+" + QString::number((conn->get_delay(-1) + stop_delay)/60));
             text->setPos(-58, -2 + 60 * i);
             text->setFont(QFont("Arial" , 8));

         } else {
             text = itineraryScene->addText(QTime(0, 0).addSecs(std::get<1>(schedule.at(i))).toString("hh:mm"));
             text->setPos(-60, -10 + 60 * i);
             text->setFont(QFont("Arial" , 10));
         }


         /* Indicate the stop with an ellipse */
         if (i > sch_index){
            auto stop = itineraryScene->addEllipse(-10, -10 + 60 * i, 20, 20);
            stop->setBrush(QBrush(QColor{Qt::white}, Qt::SolidPattern));
            stop->setPen(QPen({Qt::black}, 2));

         } else {
            auto stop = itineraryScene->addEllipse(-10, -10 + 60 * i, 20, 20);
            stop->setBrush(QBrush(QColor{Qt::green}, Qt::SolidPattern));
            stop->setPen(QPen({Qt::black}, 2));
         }
     }

     /* Add a white ellipse as padding */
     padding = itineraryScene->addEllipse(0, 60 * (sch_size) + 20, 1, 1);
     padding->setBrush(QBrush(QColor{Qt::white}, Qt::SolidPattern));
     padding->setPen(QPen({Qt::white}, 0));
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
    if (ui->showStreetNames->isChecked()){
        scene->toggle_street_names(true);
    } else {
        scene->toggle_street_names(false);
    }
}

void MainWindow::display_stop_name()
{
    if (ui->showStopNames->isChecked()){
        scene->toggle_stop_names(true);
    } else {
        scene->toggle_stop_names(false);
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
}

