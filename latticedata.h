#ifndef LATTICEDATA_H
#define LATTICEDATA_H

#include <QPoint>
#include <cmath>
#include <QList>

const int neighbourMax = 6;

typedef struct node{
    double theta; // angle
    double x,y;  // coordinates
    int xInLattice,yInLattice; // coordinates in lattice
    int neighbour[neighbourMax];// list of neigbour

}node;

class LatticeData
{
public:
    explicit LatticeData( int latticeN , int latticeS);
    LatticeData();
    node * spin;
    node *preSpin;
    QList<node *> list;
    int getNumberOfNodes();
    int getNumberOfNeighbour();

private:
    int latticeNumber;
    int latticeSize;
    int numberOfNodes;
    int numberOfNeighbour;

    void init();
    void lattice0();
    void lattice1();
    void lattice2();
    void lattice3();
    void lattice4();
    void lattice5();



};
#endif // LATTICEDATA_H
