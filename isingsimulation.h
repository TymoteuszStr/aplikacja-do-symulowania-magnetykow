#ifndef ISINGSIMULATION_H
#define ISINGSIMULATION_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QTimer>
#include <cmath>
#include "latticedata.h"

class IsingSimulation : public QWidget
{
    Q_OBJECT
public:
    explicit IsingSimulation(int latticeN, int latticeS,int W, int H, double temp, int FPS, int j, bool,bool,bool,bool, QWidget *parent = nullptr);
    ~IsingSimulation();
    LatticeData * latticeData;

    bool showLattice{false},showOnLattice{false},showArrows{false},showField{false};

protected:

    virtual void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public:
    //zmienne

    double Esr{0},E2sr{0},Msr{0},M2sr{0},M4sr{0},M2{0};
    bool isRunning=true;
    QString TextInfo = "++++++++ START: SYMULACJA ISINGA ++++++++ \n";
    int counter{0},counterCalc{0};
    int latticeNumber,latticeSize;
    int width,height,xScale,yScale;
    double J{1.0};
    int fps{50};
    int xClick{-100},yClick{-100};
    int iterator{0};
    int maxIteration{200000};
    int startPoint{1000};
    double sukces{0.0};
    double * fieldDirection;
    double * fieldValue;
    double T{1.00};
    QColor color;
    double energy{0},magnetization{0};
    QTimer * timer ;


    //funkcje
    void init();
    void drawOnLattice();
    void stepMC();
    void drawLattice();
    void drawArrows();
    void setArrow(double,double,double,double,double);
    void drawNeighbour();
    void calculate();
    double randomNumber();
    void saveOutput(QString name);
    double toggleSpin(double);
    double randomNumber(double a, double b);
    void uniformField();
    void nonUniformField();
    void drawField();
    bool uniformFieldBool{true}; // false = nonuniform field
    double uniformFieldDirection{0.0}, uniformFieldValue{0.0};
    QString generateMsg();
    void turnOnField(double, double);
    void turnOffField();
    int whichField{0};



signals:

public slots:
    void redraw();

};


#endif // ISINGSIMULATION_H
