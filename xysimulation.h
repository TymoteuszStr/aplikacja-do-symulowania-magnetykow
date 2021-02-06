#ifndef SIMULATION_H
#define SIMULATION_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QTimer>
#include <cmath>
#include "latticedata.h"


class XYSimulation : public QWidget
{
    Q_OBJECT
public:
    explicit XYSimulation(int latticeN, int latticeS,int W, int H, double temp, int FPS,int j, bool,bool,bool,bool,QWidget *parent = nullptr);

    ~XYSimulation();
    LatticeData * latticeData;

protected:

    virtual void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
public:


    bool showLattice{false},showOnLattice{false},showArrows{false},showField{false};
    bool uniformFieldBool{true}; // false = nonuniform field
    double uniformFieldDirection{0.0}, uniformFieldValue{0.0};

    void turnOnField(double, double);
    void turnOffField();

    //zmienne

    double Esr{0},E2sr{0},Msr{0},M2sr{0},M4sr{0}, M2{0};
    bool isRunning=true;
    QString TextInfo = "++++++++ START: SYMULACJA XY ++++++++ \n";
    int counter{0},counterCalc{0};
    int latticeNumber,latticeSize;
    int width,height,xScale,yScale;
    int J{1};
    int fps{50};
    int xClick{-100},yClick{-100};
    int iterator{0};
    int whichField{0};
    int maxIteration{200000};
    int startPoint{1000};
    double sukces{0.0};
    double * fieldDirection;
    double * fieldValue;
    double T{1.0};
    double color;
    double energy,magnetization;

    QTimer * timer ;


    //funkcje
    void init();
    void drawOnLattice();
    void stepMC();
    void drawLattice();
    void drawArrows();
    void setArrow(double,double,double,double,double);
    void drawNeighbour();
    void uniformField();
    void nonUniformField();
    void drawField();
    void calculate();
    double randomNumber(double,double);
    void saveOutput(QString path);
    QString generateMsg();
signals:

public slots:
    void redraw();

};

#endif // SIMULATION_H
