#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include "route.h"
#include "graph.h"
#include <QGraphicsScene>
#include <QSpinBox>
#include <QPushButton>
#include "router.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_passengerButton_clicked();
    void on_createRouteButton_clicked();
    void on_mapLoad_clicked();
    void setParameters_clicked();

private:
    void drawRoute(Route path);

    bool defined;

    Ui::MainWindow *ui;
    Graph graph;
    QGraphicsScene *scene;

    Router router;

    QSpinBox * box;
    QPushButton * passengerButton;
    QPushButton * createRoute;

    QSpinBox * cmax;
    QSpinBox * startNode;
    QSpinBox * finishNode;
    QDoubleSpinBox * maxDistance;
    QPushButton * setParameters;

    QDoubleSpinBox * cost;
    QDoubleSpinBox * fee;

    void readFile(const char * filename);
    void updatePassengerList();
    void alert(QString message);
    void drawMap();

};

#endif // MAINWINDOW_H
