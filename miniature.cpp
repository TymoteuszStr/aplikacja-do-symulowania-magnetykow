#include "miniature.h"
#include "mainwindow.h"

Miniature::Miniature(int latticeN, int xMin, int yMin, QWidget *parent) : QWidget(parent)
{

    latticeNumber = latticeN;
    xMiniature = xMin+1;
    yMiniature = yMin+23;
    xScale = 20;
    yScale = 20;

    this->setFixedSize(138,138);

    latticeData = new LatticeData(latticeNumber,10);

    moveMin();
}

void Miniature::paintEvent(QPaintEvent *event)
{
    drawMiniature(event);
}

void Miniature::drawMiniature(QPaintEvent * event)
{
    this->move(xMiniature,yMiniature);
    QPainter painter(this);

    const QRect rect = event->rect();
    painter.eraseRect(rect);

    drawLattice();
}

void Miniature::drawLattice()
{
    QPainter painter(this);

    for(int i =0; i < latticeData->getNumberOfNodes(); i++)
    {
        for(int k = 0; k < latticeData->getNumberOfNeighbour() ; k++ )
        {
            int neigbourNumber = latticeData->spin[i].neighbour[k];

            double l = (pow(( pow((latticeData->spin[i].x-latticeData->spin[neigbourNumber].x),2) + pow((latticeData->spin[i].y-latticeData->spin[neigbourNumber].y),2)),(0.5)));

               if(l <= 1.9) painter.drawLine(latticeData->spin[i].x*xScale+xMove,latticeData->spin[i].y*yScale+yMove,
                             latticeData->spin[neigbourNumber].x*xScale+xMove,latticeData->spin[neigbourNumber].y*yScale+yMove);
        }
    }
}

void Miniature::moveMin()
{
    switch (latticeNumber) {

    case 0:
        xMove = -2;
        yMove = -2;
        break;

    case 1:
        xMove = -70;
        yMove = -35;
        break;
    case 2:
        xMove = -200;
        yMove = -120;
        break;

    case 3:
        xMove = -100;
        yMove = -80;
        break;
    case 4:
        xMove = -200;
        yMove = -157;
        break;
    case 5:
        xMove = -250;
        yMove = -206;
        break;
    }
}
