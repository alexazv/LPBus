#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsTextItem>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include "PassengerCreator.h"
#include "router.h"
#include <QBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    box = new QSpinBox();
    box->setMinimum(1);

    defined = false;

    passengerButton = new QPushButton("Add passengers");
    createRoute = new QPushButton("Calculate Route");

    connect(createRoute, SIGNAL(clicked()), this, SLOT(on_createRouteButton_clicked()));
    connect(passengerButton, SIGNAL(clicked()), this, SLOT(on_passengerButton_clicked()));

    scene = new QGraphicsScene(this);

    ui->logView = new QTextEdit();
    ui->logView->setReadOnly(true);

    ui->parametersview = new QLabel();
    ui->status = new QLabel();

    ui->gridLayout = new QGridLayout();
    cmax = new QSpinBox();
    cmax->setMinimum(1);
    startNode = new QSpinBox();
    finishNode = new QSpinBox();
    maxDistance = new QDoubleSpinBox();
    fee = new QDoubleSpinBox();
    cost = new QDoubleSpinBox();

    setParameters = new QPushButton("Change Parameters");
    connect(setParameters, SIGNAL(clicked()), this, SLOT(setParameters_clicked()));

    ui->gridLayout->setSpacing(0);

    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readFile(const char * filename){
    graph =  Graph(filename);
    graph.calculateDistances();

    drawMap();

    ui->verticalLayout_2->addWidget(box);
    ui->graphicsView->setScene(scene);

    ui->verticalLayout_2->addWidget(passengerButton);
    ui->verticalLayout_2->addWidget(createRoute);
}

void MainWindow::on_mapLoad_clicked()
{
    ui->status->setText("Loading...");

    readFile("..//LPBUS//maps//map-stanford-small.txt");

    ui->gridLayout->addWidget(new QLabel("Starting Node"), 0, 0);
    startNode->setMaximum(graph.n_stops()-1);
    ui->gridLayout->addWidget(startNode, 0, 1);

    ui->gridLayout->addWidget(new QLabel("Finish Node"), 1, 0);
    finishNode->setMaximum(graph.n_stops()-1);
    ui->gridLayout->addWidget(finishNode, 1, 1);

    ui->gridLayout->addWidget(new QLabel("Max Distance"), 2, 0);
    ui->gridLayout->addWidget(maxDistance, 2, 1);

    ui->gridLayout->addWidget(new QLabel("Vehicle Capacity"), 3, 0);
    ui->gridLayout->addWidget(cmax, 3, 1);

    ui->gridLayout->addWidget(new QLabel("Cost/Distance"), 4, 0);
    ui->gridLayout->addWidget(cost, 4, 1);

    ui->gridLayout->addWidget(new QLabel("Cost/Passenger"), 5, 0);
    ui->gridLayout->addWidget(fee, 5, 1);

    ui->gridLayout->addWidget(setParameters, 6,0);

    ui->status->setText("Done");
}

void MainWindow::on_createRouteButton_clicked(){

    if(graph.passengerList.empty()){
        alert("No Passengers");
        return;
    }

     ui->status->setText("Calculating...");

    router.buildAllRoutes();

    if(router.routes.empty()){
        alert("No possible routes!");
        return;
    }

    Route chosen = router.chooseRoute(&router.routes);
    drawRoute(chosen);


    QString log(router.makeTrip(chosen).c_str());
    ui->logView->append(log);
    ui->logView->append("###########");
    ui->logView->setReadOnly(true);
    updatePassengerList();

    createRoute->setText("Next bus");

     ui->status->setText("Done");

}

void MainWindow::drawMap(){
    scene->clear();

    int radius = 6;

    QBrush brush(Qt::white);
    QPen pen(Qt::black);

    for(int i = 0; i < graph.n_stops(); i++){

        scene->addEllipse(graph.coord[i].first-radius, graph.coord[i].second-radius, radius*2, radius*2, pen, QBrush(Qt::red));

        std::vector<std::pair<int, double>> routes = graph.getRoutes(i);

        int x1 = graph.coord[i].first;
        int y1 = graph.coord[i].second;

        QString value = QString::fromStdString(std::to_string(i));
        QGraphicsTextItem * io = scene->addText(value, QFont("Arial", 12, QFont::Bold));
        io->setPos(x1, y1);

        for(int j = 0; j < routes.size(); j++){

            int x2 = graph.coord[routes[j].first].first;
            int y2 = graph.coord[routes[j].first].second;

            scene->addLine(x1, y1, x2, y2);

            QGraphicsTextItem * dist = scene->addText(QString::number(routes[j].second, 'f', 3));
            //QGraphicsRectItem * rect = scene->addRect(dist->boundingRect(), pen, brush);
            QRectF rect = dist->boundingRect();
            dist->setPos((x1+x2-rect.width())/2,(y1+y2-rect.height())/2);
            //rect->setPos((x1+x2)/2,(y1+y2)/2);
        }
    }
}

void MainWindow::drawRoute(Route route){

    drawMap();

    int radius = 8;

    QBrush brush(Qt::white);
    QPen pen(Qt::black);

    for(int i = 1; i < route.path.size(); i++){

        int p1 = route.path[i-1];
        int p2 = route.path[i];

        int x1 = graph.coord[route.path[i-1]].first;
        int y1 = graph.coord[route.path[i-1]].second;
        int x2 = graph.coord[route.path[i]].first;
        int y2 = graph.coord[route.path[i]].second;

        QPen linePen(QColor(255-i*5, 0, 0+i*5));
        linePen.setWidth(3);

        scene->addLine(x1, y1, x2, y2, linePen);
        //scene->addPolygon(x1, y1, x2, y2, QPen(Qt::green));
    }

    QString value = QString::fromStdString(std::to_string(graph.n_stops()-1));
    QGraphicsTextItem * io = scene->addText(value);
    io->setPos(1, 3);

}

void MainWindow::on_passengerButton_clicked()
{
    if(!defined){
        alert("There are parameters not set");
        return;
    }

    PassengerCreator creator;
    creator.createPasengers(&graph, box->value());
    updatePassengerList();
}

void MainWindow::updatePassengerList(){

    ui->listWidget->clear();
    for(int i = 0; i < graph.passengerList.size(); i++){

        QString passenger;
        passenger += QString::number(graph.passengerList[i].first);
        passenger += "->";
        passenger += QString::number(graph.passengerList[i].second);

        ui->listWidget->addItem(passenger);
    }
    QString number = QString::number(graph.n_passengers());
    number += " passengers left";
    ui->passenger_number->setText(number);
}

void MainWindow::setParameters_clicked(){

    if(startNode->value() == finishNode->value()){
        alert("Start Node can't be the same as finish node");
        return;
    }


    if(fee->value() == 0){
        alert("Passenger cost can't be 0");
        return;
    }

    if(cost->value() == 0){
        alert("Distance cost cost can't be 0");
        return;
    }

    if(maxDistance->value() == 0){
        alert("Max Distance can't be 0");
        return;
    }

    if(cmax->value() == 0){
        alert("Vehicle capacity can't be 0");
        return;
    }

    graph.setFinishNode(finishNode->value());
    graph.setStartNode(startNode->value());

    router = Router(&graph, this->cost->value(), this->fee->value(),
                        this->maxDistance->value(), this->cmax->value());

    QString view;
    view += "Start Node: " + QString::number(startNode->value()) + "\n";
    view += "Finish Node: " + QString::number(finishNode->value()) + "\n";
    view += "Max Distance " + QString::number(maxDistance->value()) + "\n";
    view += "Passenger Cost: " + QString::number(fee->value()) + "\n";
    view += "Distance Cost: " + QString::number(cost->value()) + "\n";
    view += "Vehicle Capacity: " + QString::number(cmax->value()) + "\n";

    ui->parametersview->setText(view);

    defined = true;
}

void MainWindow::alert(QString message){
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
    return;
}
