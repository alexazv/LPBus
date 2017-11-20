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

    passengerButton = new QPushButton("Add passengers");
    createRoute = new QPushButton("Calculate Route");

    connect(createRoute, SIGNAL(clicked()), this, SLOT(on_createRouteButton_clicked()));
    connect(passengerButton, SIGNAL(clicked()), this, SLOT(on_passengerButton_clicked()));

    scene = new QGraphicsScene(this);

    ui->logView = new QTextEdit();
    ui->logView->setReadOnly(true);

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
    graph.setMaxDistance(500); //arbitrary
    graph.calculateDistances();
    graph.setStartNode(19);
    graph.setFinishNode(35);

    int radius = 2;

    QBrush brush(Qt::white);
    QPen pen(Qt::black);

    for(int i = 0; i < graph.n_stops(); i++){

        scene->addEllipse(graph.coord[i].first-radius, graph.coord[i].second-radius, radius*2, radius*2, pen, QBrush(Qt::red));

        std::vector<std::pair<int, double>> routes = graph.getRoutes(i);

        for(int j = 0; j < routes.size(); j++){

            int x1 = graph.coord[i].first;
            int y1 = graph.coord[i].second;
            int x2 = graph.coord[routes[j].first].first;
            int y2 = graph.coord[routes[j].first].second;

            scene->addLine(x1, y1, x2, y2);

            /*QGraphicsTextItem * io = scene->addText(QString::number(i));
            io->setPos(x1, y1);
            QString value;
            value.setNum(routes[j].second*100, 'f');
            io = scene->addText(value);
            io->setPos((x1+x2)/2,(y1+y2)/2);

            /*QString value = QString::fromStdString(std::to_string(routes[i].second));
            io->setPos((x1+x2)/2,(y1+y2)/2);*/

        }

    }

    ui->verticalLayout_2->addWidget(box);
    ui->graphicsView->setScene(scene);

    ui->verticalLayout_2->addWidget(passengerButton);
    ui->verticalLayout_2->addWidget(createRoute);
}

void MainWindow::on_mapLoad_clicked()
{
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
}

void MainWindow::on_createRouteButton_clicked(){
    //teste
    //todo: calculate route

    if(graph.passengerList.empty()){
        QMessageBox msgBox;
        msgBox.setText("There are no passengers");
        msgBox.exec();
        return;
    }

    router.buildAllRoutes();

    if(router.routes.empty()){
        alert("No possible routes!");
        return;
    }

    Route chosen = router.chooseRoute(router.routes);
    drawRoute(chosen);


    QString log(router.makeTrip(chosen).c_str());
    ui->logView->setPlainText(log);
    ui->logView->setReadOnly(true);
    updatePassengerList();
    //ui->logView->setDisabled(true);

    createRoute->setText("Next bus");

}

void MainWindow::drawRoute(Route route){


    scene->clear();

    int radius = 6;

    QBrush brush(Qt::white);
    QPen pen(Qt::black);

    for(int i = 0; i < graph.n_stops(); i++)
        scene->addEllipse(graph.coord[i].first-radius, graph.coord[i].second-radius, radius*2, radius*2, pen, QBrush(Qt::red));

    for(int i = 1; i < route.path.size(); i++){

        int p1 = route.path[i-1];
        int p2 = route.path[i];

        int x1 = graph.coord[route.path[i-1]].first;
        int y1 = graph.coord[route.path[i-1]].second;
        int x2 = graph.coord[route.path[i]].first;
        int y2 = graph.coord[route.path[i]].second;

        scene->addLine(x1, y1, x2, y2, QPen(Qt::green));
        //scene->addPolygon(x1, y1, x2, y2, QPen(Qt::green));

        QString value = QString::fromStdString(std::to_string(route.path[i-1]));
        QGraphicsTextItem * io = scene->addText(value);
        io->setPos(x1, y1);
    }

    int x1 = graph.coord[route.path.size()-1].first;
    int y1 = graph.coord[route.path.size()-1].second;
    QString value = QString::fromStdString(std::to_string(route.path.size()-1));
    QGraphicsTextItem * io = scene->addText(value);
    io->setPos(x1, y1);


}

void MainWindow::on_passengerButton_clicked()
{
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

    graph.setFinishNode(finishNode->value());
    graph.setStartNode(startNode->value());

    router = Router(&graph, this->cost->value(), this->fee->value(),
                        this->maxDistance->value(), this->cmax->value());

    defined = true;
}

void MainWindow::alert(QString message){
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
    return;
}
