#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsTextItem>
#include <QSpinBox>
#include <QPushButton>
#include "PassengerCreator.h"
#include <QBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    box = new QSpinBox();
    box->setMinimum(1);

    //passengerButton = new QPushButton("Add passengers", this);
    passengerButton->setText("Add passengers");

    scene = new QGraphicsScene(this);

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readFile(const char * filename){
    Graph graph(filename);

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


            QGraphicsTextItem * io = new QGraphicsTextItem;
            io->setPos((x1+x2)/2,(y1+y2)/2);
            io->setPlainText(QString::number(routes[i].second, 'f', 3));

            scene->addItem(io);

        }

    }

    ui->verticalLayout_2->addWidget(box);
    ui->graphicsView->setScene(scene);

}

void MainWindow::on_pushButton_clicked()
{
    readFile("..//LPBUS//maps//map-stanford-small.txt");
}

void MainWindow::on_passengerButton_clicked()
{
    PassengerCreator creator;
    creator.createPasengers(&graph, box->value());
}
