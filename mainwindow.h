#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include "route.h"
#include "graph.h"
#include <QGraphicsScene>
#include <QSpinBox>
#include <QPushButton>


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
    void on_pushButton_clicked();
    void on_passengerButton_clicked();
    void on_createRouteButton_clicked();

private:
    void drawRoute(Route path);

    Ui::MainWindow *ui;
    Graph graph;
    QGraphicsScene *scene;

    QSpinBox * box;
    QPushButton * passengerButton;
    QPushButton * createRoute;

    void readFile(const char * filename);

};

#endif // MAINWINDOW_H
