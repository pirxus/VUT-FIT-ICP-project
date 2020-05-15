/**
 * @file mainwindow.cpp
 * @brief This is the main window module for the application.
 * @authors Šimon Sedláček - xsedla1h, Radim Lipka - xlipka02
 */

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
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    initScene();
    initTraffic();

    m_street_closing_mode = false;
    m_itinerary = nullptr;

    /* Setup button signals */
    connect(m_ui->zoomInButton, &QPushButton::clicked, this, &MainWindow::zoomIn);
    connect(m_ui->zoomOutButton, &QPushButton::clicked, this, &MainWindow::zoomOut);
    connect(m_ui->zoomSlider, &QSlider::valueChanged, this, &MainWindow::sliderZoom);

    connect(m_ui->buttonPause, &QPushButton::clicked, this, &MainWindow::toggle_play_pause);
    connect(m_ui->sliderSpeed, &QSlider::valueChanged, this, &MainWindow::slider_speed);

    /* Setup top menu actions */
    connect(m_ui->actionLoad_map, &QAction::triggered, this, &MainWindow::load_map);
    connect(m_ui->actionLoad_stops, &QAction::triggered, this, &MainWindow::load_stops);
    connect(m_ui->actionLoad_lines, &QAction::triggered, this, &MainWindow::load_lines);

    /* Vehicle updates */
    connect(m_transit, &PublicTransport::vehicles_updated, this, &MainWindow::positions_updated);
    connect(m_ui->timeEdit, &QTimeEdit::timeChanged, this->m_transit, &PublicTransport::time_changed);
    connect(m_transit, &PublicTransport::time_updated, this, &MainWindow::update_time);

    /* Streets */
    connect(m_scene, &Scene::street_selected, this, &MainWindow::street_selected);
    connect(m_scene, &Scene::street_unselected, this, &MainWindow::street_unselected);
    connect(m_ui->spinTraffic, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MainWindow::traffic_situation_changed);

    /* Street closing */
    connect(m_scene, &Scene::street_canceled, this, &MainWindow::street_cancelled);
    connect(m_ui->buttonBoxClose, &QDialogButtonBox::rejected, this, &MainWindow::cancel_street_cancel);
    connect(m_ui->buttonBoxClose, &QDialogButtonBox::accepted, this, &MainWindow::accept_street_cancel);

    connect(m_scene, &Scene::display_itinerary, this, &MainWindow::display_itinerary);
    connect(m_scene, &Scene::clear_itinerary, this, &MainWindow::clear_itinerary);

    connect(m_ui->showStreetNames, &QCheckBox::stateChanged, this, &MainWindow::display_street_name);
    connect(m_ui->showStopNames, &QCheckBox::stateChanged, this, &MainWindow::display_stop_name);
}

MainWindow::~MainWindow()
{
    delete m_ui;
    delete m_transit;
}

void MainWindow::zoomIn()
{
    this->m_ui->graphicsView->scale(1.25, 1.25);
    this->m_ui->zoomSlider->setValue(this->m_ui->zoomSlider->value()+1);
}

void MainWindow::zoomOut()
{
    this->m_ui->graphicsView->scale(0.75, 0.75);
    this->m_ui->zoomSlider->setValue(this->m_ui->zoomSlider->value()-1);
}

void MainWindow::sliderZoom(int z)
{
    auto t = this->m_ui->graphicsView->transform();
    qreal scale = z / 50.0;
    m_ui->graphicsView->setTransform(QTransform(scale, t.m12(), t.m21(), scale, t.dx(), t.dy()));
}

void MainWindow::load_map()
{
    /* First clear the scene */
    m_scene->delete_streets();
    m_scene->delete_labels();
    m_scene->delete_stops();
    m_scene->delete_connections();

    //QString file = QFileDialog::getOpenFileName(this, "Load a street csv file");
    //if (file == nullptr) return;
    //std::string path = QFileInfo(file).absoluteFilePath().toStdString();
    std::string path = QFileInfo("../../examples/streetList.csv").absoluteFilePath().toStdString();

    /* Let the PublicTransport module load the map */
    m_transit->load_map(path.c_str());

    /* And add them to the scene... */
    for (auto it = m_transit->map.streets.begin(); // Streets
        it != m_transit->map.streets.end(); it++) {
        this->m_scene->add_street((*it).second);
    }
}

void MainWindow::load_stops()
{
    /* First clear the scene */
    m_scene->delete_stops();
    m_scene->delete_stop_labels();
    m_scene->delete_connections();

    //QString file = QFileDialog::getOpenFileName(this, "Load a stop csv file");
    //if (file == nullptr) return;
    //std::string path = QFileInfo(file).absoluteFilePath().toStdString();
    std::string path = QFileInfo("../../examples/stopList.csv").absoluteFilePath().toStdString();

    /* Let the PublicTransport module load the stops */
    m_transit->load_stops(path.c_str());
    for (auto it = m_transit->map.stops.begin(); // Stops
         it != m_transit->map.stops.end(); it++) {
        this->m_scene->add_stop((*it).second);
    }
}

void MainWindow::load_lines()
{
    m_scene->delete_connections();
    //QString file = QFileDialog::getOpenFileName(this, "Load a line/connection csv file");
    //if (file == nullptr) return;
    //std::string path = QFileInfo(file).absoluteFilePath().toStdString();
    std::string path = QFileInfo("../../examples/lineList.csv").absoluteFilePath().toStdString();

    /* Let the PublicTransport module load the lines */
    m_transit->load_lines(path.c_str());
    m_transit->prepare_connections();

    /* Add the loaded connections to our scene */
    for (auto line : m_transit->lines) {
        for (auto conn : line->connections) {
            this->m_scene->add_connection(conn);
        }
    }
}

void MainWindow::positions_updated()
{
    /* Redraw all vehicles */
    this->m_scene->redraw_connections();

    /* Update the displayed itinerary for the selected vehicle */
    if (m_itinerary != nullptr) {
        Connection *tmp = m_itinerary;
        clear_itinerary();
        if (tmp->active)
            display_itinerary(tmp);
    }
}

void MainWindow::update_time(unsigned time)
{
    this->m_ui->timeEdit->setTime(QTime(0, 0).addSecs(time));
}

void MainWindow::slider_speed()
{
    unsigned slider_value = this->m_ui->sliderSpeed->value();
    this->m_transit->set_clock_rate(slider_value);
}

void MainWindow::toggle_play_pause()
{
    if (this->m_ui->buttonPause->text() == "Pause") {
        this->m_transit->stop_timer();
        this->m_ui->buttonPause->setText("Play");
    } else {
        this->m_transit->start_timer();
        this->m_ui->buttonPause->setText("Pause");
    }
}

void MainWindow::street_selected(Street *street)
{
    /* Enable the traffic intensity counter and get the current value */
    this->m_ui->spinTraffic->setEnabled(true);
    this->m_currently_edited_street = street;
    this->m_ui->spinTraffic->setValue(street->get_traffic());
}

void MainWindow::street_unselected(Street *street)
{
    /* Disable the traffic intensity counter */
    this->m_ui->spinTraffic->setDisabled(true);
    if (this->m_currently_edited_street == street) {
        this->m_currently_edited_street = nullptr;
    }
}

void MainWindow::traffic_situation_changed(int level)
{
    this->m_currently_edited_street->set_traffic(level);
}

void MainWindow::street_cancelled(ViewStreet *street)
{
    m_currently_cancelled_street = street;
    m_street_closing_mode = true;

    /* Block some UI elements for the time being */
    m_ui->buttonPause->setEnabled(false);
    m_ui->spinTraffic->setEnabled(false);
    m_ui->timeEdit->setEnabled(false);

    m_ui->buttonBoxClose->setEnabled(true);
    m_scene->setBackgroundBrush(QBrush(QColor{Qt::gray}, Qt::SolidPattern));
    m_ui->labelClose->setText("Please specify the detour for the closed street. Choose the streets in order.");
    /* Wait for detour specification */
    m_scene->prepare_for_detour();
}

void MainWindow::cancel_street_cancel()
{
    /* Inform the scene */
    m_scene->end_detour_selection(false);

    /* Reset to the normal state */
    restore_after_cancel();
}

void MainWindow::accept_street_cancel()
{
    /* First extract just the pointers to the street objects from the ViewStreet objects */
    std::vector<Street *> detour_streets;
    m_detour = m_scene->get_detour();
    for (auto street : m_detour) {
        detour_streets.push_back(street->get_street());
    }

    /* Try to setup the detour */
    bool correct;
    correct = !this->m_transit->create_detour(m_currently_cancelled_street->get_street(), detour_streets);

    if (correct) m_currently_cancelled_street->get_street()->close_street();
    /* Inform the scene */
    m_scene->end_detour_selection(correct);

    /* Reset to the normal state */
    restore_after_cancel();
}

void MainWindow::restore_after_cancel()
{
    m_detour.clear();
    m_ui->buttonPause->setEnabled(true);
    m_ui->timeEdit->setEnabled(true);

    m_currently_cancelled_street = nullptr;
    m_street_closing_mode = true;

    m_ui->buttonBoxClose->setEnabled(false);
    m_scene->setBackgroundBrush(QBrush(QColor{Qt::white}, Qt::SolidPattern));
    m_ui->labelClose->setText("In order to close a street, choose one and double-click it.");

}

void MainWindow::display_itinerary(Connection *conn)
{
    clear_itinerary();
    m_itinerary = conn;
    auto schedule = conn->get_schedule();
    if (schedule.empty()) return; /* Avoid segfaults, nothing to display... */

    m_ui->currentDelay->setText("Current delay: ");
    int sch_index = conn->find_schedule_index(m_transit->get_time());
    int delay = conn->get_delay(sch_index);
    int line_number = conn->get_line()->get_line_number();

    m_ui->delayValue->setText(QString::number(delay/60));
    m_ui->nextStop->setText("Next stop: ");
    m_ui->nextStopName->setText(QString(std::get<0>(schedule.at(sch_index + 1))->name().c_str()));
    m_ui->lineNumber->setText("Line number: ");
    m_ui->lineNumberValue->setText(QString::number(line_number));

    /* Add a white ellipse as padding */
    auto padding = m_itinerary_scene->addEllipse(0, -30, 1, 1);
    padding->setBrush(QBrush(QColor{Qt::white}, Qt::SolidPattern));
    padding->setPen(QPen({Qt::white}, 0));

    int sch_size = schedule.size() - 1;
    int line_cord = 60 * sch_size;
    auto line = m_itinerary_scene->addLine(0, 0, 0, line_cord);
    line->setPen(QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap));

    /* Paint the stops */
    for (int i = 0; i < sch_size + 1; i++) {
        /* Show the name of the stop */
        auto text = m_itinerary_scene->addText(QString(std::get<0>(schedule.at(i))->name().c_str()));
        text->setPos(20, -10 + 60 * i);
        text->setFont(QFont("Arial" , 10));

        unsigned stop_delay = std::get<2>(schedule.at(i));
        /* Set the time for the stop */
        if (delay != 0 && i > sch_index) { /* Add the information about the current delay */
            text = m_itinerary_scene->addText(QTime(0, 0).addSecs(std::get<1>(schedule.at(i))).toString("hh:mm"));
            text->setPos(-60, -15 + 60 * i);
            text->setFont(QFont("Arial" , 10));

            /* Delay */
            text = m_itinerary_scene->addText("+" + QString::number((conn->get_delay(-1) + stop_delay)/60));
            text->setPos(-58, -2 + 60 * i);
            text->setFont(QFont("Arial" , 8));
            text->setDefaultTextColor(Qt::red);

        } else {
            text = m_itinerary_scene->addText(QTime(0, 0).addSecs(std::get<1>(schedule.at(i))).toString("hh:mm"));
            text->setPos(-60, -10 + 60 * i);
            text->setFont(QFont("Arial" , 10));
        }


        /* Indicate the stop with an ellipse */
        if (i > sch_index) {
            auto stop = m_itinerary_scene->addEllipse(-9, -9 + 60 * i, 18, 18);
            stop->setBrush(QBrush(QColor{Qt::white}, Qt::SolidPattern));
            stop->setPen(QPen({Qt::black}, 2));

        } else {
            auto stop = m_itinerary_scene->addEllipse(-9, -9 + 60 * i, 18, 18);
            stop->setBrush(QBrush(QColor{QColor(136, 168, 182, 255)}, Qt::SolidPattern));
            stop->setPen(QPen({Qt::black}, 2));
        }
    }

    /* Add a white ellipse as padding */
    padding = m_itinerary_scene->addEllipse(0, 60 * (sch_size) + 20, 1, 1);
    padding->setBrush(QBrush(QColor{Qt::white}, Qt::SolidPattern));
    padding->setPen(QPen({Qt::white}, 0));
}

void MainWindow::clear_itinerary()
{
    this->m_ui->currentDelay->clear();
    this->m_ui->nextStop->clear();
    this->m_ui->lineNumber->clear();
    this->m_ui->delayValue->clear();
    this->m_ui->lineNumberValue->clear();
    this->m_ui->nextStopName->clear();
    qDeleteAll(m_itinerary_scene->items());
}

void MainWindow::display_street_name()
{
    if (m_ui->showStreetNames->isChecked()){
        m_scene->toggle_street_names(true);
    } else {
        m_scene->toggle_street_names(false);
    }
}

void MainWindow::display_stop_name()
{
    if (m_ui->showStopNames->isChecked()){
        m_scene->toggle_stop_names(true);
    } else {
        m_scene->toggle_stop_names(false);
    }
}

void MainWindow::initTraffic()
{
    this->m_transit = new PublicTransport(this);
}

void MainWindow::initScene()
{
    this->m_scene = new Scene(m_ui->graphicsView);
    m_ui->graphicsView->setScene(m_scene);
    m_ui->graphicsView->setRenderHints(QPainter::Antialiasing);
    this->m_itinerary_scene = new Scene(m_ui->itineraryView);
    m_ui->itineraryView->setScene(m_itinerary_scene);
    m_ui->itineraryView->setRenderHints(QPainter::Antialiasing);
}

