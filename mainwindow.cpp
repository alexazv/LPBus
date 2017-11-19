#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsTextItem>
#include <QSpinBox>
#include <QPushButton>
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

    int radius = 10;

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



            QString value = QString::fromStdString(std::to_string(routes[i].second));
            QGraphicsTextItem * io = scene->addText(value);
            io->setPos((x1+x2)/2,(y1+y2)/2);

        }

    }

    ui->verticalLayout_2->addWidget(box);
    ui->graphicsView->setScene(scene);

    ui->verticalLayout_2->addWidget(passengerButton);
    ui->verticalLayout_2->addWidget(createRoute);
}

void MainWindow::on_pushButton_clicked()
{
    readFile("..//LPBUS//maps//map-stanford-small.txt");
}

void MainWindow::on_createRouteButton_clicked(){
    //teste
    //todo: calculate route
    Router router(graph, 5.0, 3.5, 10.0, 5);
    drawRoute(router.findPath(std::vector<int>{graph.getStartNode(), 0, 29, graph.getFinishNode()}));
}

void MainWindow::drawRoute(Route path){

    scene->clear();

    int radius = 10;

    QBrush brush(Qt::white);
    QPen pen(Qt::black);

    for(int i = 0; i < graph.n_stops(); i++)
        scene->addEllipse(graph.coord[i].first-radius, graph.coord[i].second-radius, radius*2, radius*2, pen, QBrush(Qt::red));

    for(int i = 1; i < path.path.size(); i++){

        int p1 = path.path[i];
        int p2 = path.path[i-1];

        int x1 = graph.coord[path.path[i]].first;
        int y1 = graph.coord[path.path[i]].second;
        int x2 = graph.coord[path.path[i-1]].first;
        int y2 = graph.coord[path.path[i-1]].second;

        scene->addLine(x1, y1, x2, y2, QPen(Qt::red));
    }
}

void MainWindow::on_passengerButton_clicked()
{
    PassengerCreator creator;
    creator.createPasengers(&graph, box->value());
}
