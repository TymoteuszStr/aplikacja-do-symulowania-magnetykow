#include "pottssimulation.h"

PottsSimulation::PottsSimulation(int latticeN, int size,int W, int H,double temp, int FPS, int j,bool showOnL,bool showA,bool showL, bool showF, QWidget *parent) : QWidget(parent)
{


    latticeNumber = latticeN;
    latticeSize = size;
    width = W;
    height = H;
    xScale = width/latticeSize;
    yScale = height/latticeSize;
    J=j;
    T=temp;
    fps=FPS;
    showOnLattice=showOnL;
    showArrows = showA;
    showLattice =showL;
    showField = showF;

    init();

    this->setFixedSize(2000,1000);


    timer = new  QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(redraw()));
    timer->start(1000/fps);
}

void PottsSimulation::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
   if(showOnLattice) drawOnLattice();
   if(showArrows) drawArrows();
   if(showLattice)  drawLattice();
   if(showField) drawField();
      drawNeighbour();
}

void PottsSimulation::init()
{
    latticeData = new LatticeData(latticeNumber,latticeSize);
    fieldDirection = new double[latticeData->getNumberOfNodes()];
    fieldValue = new double[latticeData->getNumberOfNodes()];

    for(int i = 0; i < latticeData->getNumberOfNodes(); i++)
    {
        fieldValue[i] = 0.0;

        latticeData->spin[i].theta = randomNumber(0.0, 1.0);
    }

    for(int i = 0; i < latticeData->getNumberOfNodes(); i++)
    {
        fieldDirection[i] = 0.0;
        fieldValue[i] = 0.0;
    }
}

void PottsSimulation::stepMC()
{

    iterator++;

    int nodeNumber = rand() % (latticeData->getNumberOfNodes());
    double dE=0.0;
    double dTheta=randomNumber(-0.3,0.3);
    double newAngle = latticeData->spin[nodeNumber].theta + dTheta;

    for(int i = 0; i < latticeData->getNumberOfNeighbour() ; i++ )
    {
        int neigbourNumber = latticeData->spin[nodeNumber].neighbour[i];

        dE+=J*cos(latticeData->spin[nodeNumber].theta - latticeData->spin[neigbourNumber].theta);

        dE-=J*cos(newAngle - latticeData->spin[neigbourNumber].theta);

    }

    dE +=   fieldValue[nodeNumber] * (cos(-latticeData->spin[nodeNumber].theta + fieldDirection[nodeNumber] )
                                    - cos(fieldDirection[nodeNumber] - newAngle));


    if(dE<0.0)
    {
        latticeData->spin[nodeNumber].theta = newAngle;
        sukces++;
    }
    else
    {
        double u = static_cast<double>(rand()%RAND_MAX)/RAND_MAX;
        if(u<exp(-dE/T)) {
            latticeData->spin[nodeNumber].theta = newAngle;
            sukces++;
        }
    }

    if( latticeData->spin[nodeNumber].theta>2.0*M_PI)  latticeData->spin[nodeNumber].theta-=2.0*M_PI;
    if( latticeData->spin[nodeNumber].theta<0*M_PI)  latticeData->spin[nodeNumber].theta+=2.0*M_PI;

    //if (iterator%20 == 0 ) qDebug()<<sukces*100/iterator;

}
void PottsSimulation::calculate()
{


    double buffSumSin=0;
    double buffSumCos=0;
    double buffEnergy=0;
    double buffEnergySin=0;
    double buffEnergyCos=0;
    double buffMag=0;
    double buffMagSin=0;
    double buffMagCos=0;
    double buffFieldSin=0;
    double buffFieldCos=0;

    for(int i = 0; i < latticeData->getNumberOfNodes(); i++)
    {
        buffMagSin += cos(latticeData->spin[i].theta);
        buffMagCos += sin(latticeData->spin[i].theta);
        buffSumSin=0;
        buffSumCos=0;

        for(int j = 0; j < latticeData->getNumberOfNeighbour(); j++)
        {
            int neigbourNumber = latticeData->spin[i].neighbour[j];
            buffSumCos += cos(latticeData->spin[neigbourNumber].theta);
            buffSumSin += sin(latticeData->spin[neigbourNumber].theta);
        }
        buffEnergySin += sin(latticeData->spin[i].theta)* buffSumSin;
        buffEnergyCos += cos(latticeData->spin[i].theta)* buffSumCos;

        buffFieldSin += sin(latticeData->spin[i].theta) * sin(fieldDirection[i])*fieldValue[i];
        buffFieldCos += cos(latticeData->spin[i].theta) * cos(fieldDirection[i])*fieldValue[i];
    }

     buffEnergy = -0.5*J*(buffEnergyCos+buffEnergySin) - (buffFieldSin+buffFieldCos);
     buffMag = pow(pow(buffMagSin,2) + pow(buffMagCos,2),0.5);

     buffEnergy /= (double)latticeData->getNumberOfNodes();
     buffMag /= (double)latticeData->getNumberOfNodes();

     if(counterCalc > startPoint)
     {
         Esr += buffEnergy;
         E2sr += buffEnergy*buffEnergy;
         Msr += buffMag;
         M2sr += buffMag*buffMag;
         M4sr += buffMag*buffMag*buffMag*buffMag;

     }

     if(counterCalc > startPoint)
     {
         Esr += buffEnergy;
         E2sr += buffEnergy*buffEnergy;
         Msr += buffMag;
         M2sr += buffMag*buffMag;
         M4sr += buffMag*buffMag*buffMag*buffMag;

     }

     if(counterCalc%(5000) == 0){
         QString currentText ="";
         currentText += "\n";
         currentText += "Symulacja trwa... \n";
         currentText += "Numer iteracji: "+QString::number(counterCalc-startPoint)+"\n";
         currentText += "Energia: "+QString::number(Esr/double(counterCalc-startPoint))+"\n";
         currentText += "Namagnesowanie: "+QString::number(Msr/double(counterCalc-startPoint))+"\n";
         currentText +="\n";

         TextInfo = currentText + TextInfo;
         currentText = "";
     }

     if(counterCalc == maxIteration+startPoint)
     {
         QString currentText ="++++++++ KONIEC SYMULACJI ++++++++ \n";
         TextInfo = currentText+ generateMsg() + TextInfo;
         saveOutput("./save/PottsSimulation/default.txt");
     }
}
void PottsSimulation::drawLattice()
{
    QPainter painter(this);

    for(int i =0; i < latticeData->getNumberOfNodes(); i++)
    {
        for(int k = 0; k < latticeData->getNumberOfNeighbour() ; k++ )
        {
            int neigbourNumber = latticeData->spin[i].neighbour[k];

            double l = (pow(( pow((latticeData->spin[i].x-latticeData->spin[neigbourNumber].x),2) + pow((latticeData->spin[i].y-latticeData->spin[neigbourNumber].y),2)),(0.5)));

               if(l < 2) painter.drawLine(latticeData->spin[i].x*xScale,latticeData->spin[i].y*yScale,
                             latticeData->spin[neigbourNumber].x*xScale,latticeData->spin[neigbourNumber].y*yScale);
        }
    }
}

void PottsSimulation::drawOnLattice()
{
    QPainter painter(this);

    int xSize = xScale-xScale/10;
    int ySize = yScale-yScale/10;

    for(int i =0; i < latticeData->getNumberOfNodes(); i++)
    {
       // color = latticeData->spin[i].theta/(2.0*M_PI)*255.0;
       // QColor newColor(static_cast<int>(color),0,static_cast<int>(255-color));

        QColor newColor;
        double spin= latticeData->spin[i].theta;

        if(spin >= 0 && spin < 0.33*M_PI) newColor = QColor(255, 255*spin/(0.33*M_PI), 0);
        else if( spin>= 0.33*M_PI && spin < 0.66*M_PI) newColor = QColor(255*(0.66*M_PI - spin)/(0.33*M_PI), 255, 0);
        else if ( spin >= 0.66*M_PI && spin < 0.99*M_PI ) newColor = QColor(0, 255, 255*(spin-0.66*M_PI)/(0.33*M_PI) );
        else if( spin >= 0.99*M_PI && spin < 1.32*M_PI) newColor = QColor(0, 255*(1.32*M_PI-spin)/(0.33*M_PI) ,255);
        else if ( spin >= 1.32*M_PI && spin < 1.65*M_PI) newColor = QColor(255*(spin-1.32*M_PI)/(0.33*M_PI) ,0,255);
        else if (spin >=1.65*M_PI && spin<=2.0*M_PI)newColor = QColor(255, 0, 255*(2.0*M_PI-spin)/(0.35*M_PI));
        else {
            newColor = QColor(255,255,255);
            qDebug()<<spin;
        }
        painter.fillRect(latticeData->spin[i].x*xScale - xSize/2,latticeData->spin[i].y*yScale - ySize/2,
                         xSize,ySize,newColor);

    }
}
void PottsSimulation::drawArrows()
{
    for(int i =0; i < latticeData->getNumberOfNodes(); i++)
    {
        double angle = latticeData->spin[i].theta;

        double  dx=0.4*(yScale-0.001)*sin(angle);
        double  dy=0.4*(xScale-0.001)*cos(angle);

        setArrow(
                 xScale*(latticeData->spin[i].x),
                 yScale*(latticeData->spin[i].y),
                 xScale*(latticeData->spin[i].x)+dx,
                 yScale*(latticeData->spin[i].y)+dy,
                 angle
                );


    }

}

void PottsSimulation::setArrow(double x0, double y0, double x1, double y1, double angle)
{
    QPainter painter(this);
    painter.setPen(Qt::black);

    double l = (pow(( pow((x1-x0),2) + pow((y1-y0),2)),(0.5))) /3;

    painter.drawLine(
                static_cast<int>(x0),
                static_cast<int>(y0),
                static_cast<int>(x1),
                static_cast<int>(y1));

    painter.drawLine(
                static_cast<int>(x1 + cos(M_PI - angle + M_PI/6)*l),
                static_cast<int>(y1 + sin(M_PI - angle + M_PI/6)*l),
                static_cast<int>(x1),
                static_cast<int>(y1));

    painter.drawLine(
              static_cast<int>(x1 + cos(-M_PI/2 - angle + M_PI/6)*l),
                static_cast<int>(y1 + sin(-M_PI/2 - angle + M_PI/6)*l),
             static_cast<int>(x1),
             static_cast<int>(y1));



}

void PottsSimulation::drawNeighbour()
{
    QPainter painter(this);

    for(int i = 0; i < latticeData->getNumberOfNodes(); i++)
    {

        if(latticeData->spin[i].x*xScale > xClick - xScale/3 && latticeData->spin[i].x*xScale < xClick + xScale/3
                && latticeData->spin[i].y*yScale > yClick- yScale/3 && latticeData->spin[i].y*yScale < yClick + yScale/3)
        {

            painter.setPen(Qt::green);
            painter.drawRect(latticeData->spin[i].x * xScale - xScale/4,latticeData->spin[i].y * yScale - yScale/4,xScale/2,yScale/2);

            painter.setPen(QColor(255, 165, 0));
            for(int j = 0; j < latticeData->getNumberOfNeighbour(); j++){

                int neigbourNumber = latticeData->spin[i].neighbour[j];

                painter.drawRect(latticeData->spin[neigbourNumber].x * xScale - xScale/4,latticeData->spin[neigbourNumber].y * yScale - yScale/4,xScale/2,yScale/2);

            }

        }

    }
}

void PottsSimulation::uniformField()
{
    for(int i = 0; i < latticeData->getNumberOfNodes(); i++)
    {
        fieldDirection[i] = uniformFieldDirection;
        fieldValue[i] = uniformFieldValue;
    }
}

void PottsSimulation::nonUniformField()
{
    for(int i = 0; i < latticeData->getNumberOfNodes(); i++)
    {
        fieldDirection[i] = uniformFieldDirection;

        int X= latticeData->spin[i].xInLattice -(latticeSize/2);        // x układu kartezjańskiego
        int Y= -1*(latticeData->spin[i].yInLattice -(latticeSize/2));   // y układu kartezjaśkiego

          switch (whichField)
          {
          case 0:
              fieldValue[i] =  static_cast<double>(uniformFieldValue)*static_cast<double>(latticeData->spin[i].xInLattice)/latticeSize;
              break;
          case 1:
              fieldValue[i] =  static_cast<double>(uniformFieldValue)*static_cast<double>(latticeData->spin[i].yInLattice)/latticeSize;
              break;

          case 2:
              if(Y<(latticeSize/2)*sin((10.0/latticeSize)*X))
                     fieldValue[i] =  static_cast<double>(uniformFieldValue);
               else fieldValue[i]  = 0;
                     break;
          case 3:
              if(Y<(latticeSize/2)*cos((10.0/latticeSize)*X))
                     fieldValue[i] =  static_cast<double>(uniformFieldValue);
               else fieldValue[i]  = 0;
                     break;

          case 4:

              if(Y>(latticeSize)*pow((3.0/latticeSize)*X,2))
                  fieldValue[i] =  static_cast<double>(uniformFieldValue);
              else fieldValue[i] =  0;

              break;

          case 5:

              if(Y<(latticeSize)*-pow((3.0/latticeSize)*X,2))
                  fieldValue[i] =  static_cast<double>(uniformFieldValue);
              else fieldValue[i] =  0;

              break;

          case 6:

              if(pow((latticeSize/4),2)>(pow(Y,2)+pow(X,2))) fieldValue[i] = uniformFieldValue;
              else fieldValue[i] = 0;

              break;

          case 7:

            if(latticeData->spin[i].xInLattice%(2*latticeSize/5)  <latticeSize/5 && latticeData->spin[i].yInLattice%(2*latticeSize/5) <latticeSize/5)
                fieldValue[i] =  static_cast<double>(uniformFieldValue);
            else fieldValue[i] =  0;


              break;

          }
    }

}

void PottsSimulation::drawField()
{
    QPainter painter(this);
    double max{0};
    int xSize = xScale-xScale/10;
    int ySize = yScale-yScale/10;

    for(int i = 0; i < latticeData->getNumberOfNodes(); i++)
    {
        if(max < fieldValue[i]) max = fieldValue[i];
    }



    for(int i = 0; i < latticeData->getNumberOfNodes(); i++)
    {
        double colorField;
        if(max != 0.0) colorField = 255*fieldValue[i]/max;
        else colorField = 1;

        QColor newColor(static_cast<int>(colorField),static_cast<int>(colorField),static_cast<int>(colorField));

        painter.fillRect(latticeData->spin[i].x*xScale - xSize/2,latticeData->spin[i].y*yScale - ySize/2,
                         xSize,ySize,newColor);


    }

}



double PottsSimulation::randomNumber(double a, double b)
{
    double u = static_cast<double>(rand()%RAND_MAX)/RAND_MAX;
    double randomNum = a+(b-a)*u;

    if(randomNum>= 0 && randomNum < 0.1) randomNum = 2*M_PI/10;
    else if(randomNum >= 0.1 && randomNum < 0.2) randomNum = 2*2*M_PI/10;
    else if(randomNum >= 0.2 && randomNum < 0.3) randomNum = 3*2*M_PI/10;
    else if(randomNum >= 0.3 && randomNum < 0.4) randomNum = 4*2*M_PI/10;
    else if(randomNum >= 0.4 && randomNum < 0.5) randomNum = 5*2*M_PI/10;
    else if(randomNum >= 0.5 && randomNum < 0.6) randomNum = 6*2*M_PI/10;
    else if(randomNum >= 0.6 && randomNum < 0.7) randomNum = 7*2*M_PI/10;
    else if(randomNum >= 0.7 && randomNum < 0.8) randomNum = 8*2*M_PI/10;
    else if(randomNum >= 0.8 && randomNum < 0.9) randomNum = 9*2*M_PI/10;
    else if(randomNum >= 0.9 && randomNum <= 1) randomNum = 2*M_PI;
    else if(randomNum <= 0 && randomNum > -0.1) randomNum = -2*M_PI/10;
    else if(randomNum <= -0.1 && randomNum > -0.2) randomNum = -2*2*M_PI/10;
    else if(randomNum <= -0.2 && randomNum > -0.3) randomNum = -3*2*M_PI/10;
    else if(randomNum <= -0.3 && randomNum > -0.4) randomNum = -4*2*M_PI/10;
    else if(randomNum <= -0.4 && randomNum > -0.5) randomNum = -5*2*M_PI/10;
    else if(randomNum <= -0.5 && randomNum > -0.6) randomNum = -6*2*M_PI/10;
    else if(randomNum <= -0.6 && randomNum > -0.7) randomNum = -7*2*M_PI/10;
    else if(randomNum <= -0.7 && randomNum > -0.8) randomNum = -8*2*M_PI/10;
    else if(randomNum <= -0.8 && randomNum > -0.9) randomNum = -9*2*M_PI/10;
    else if(randomNum <= -0.9 && randomNum >= -1) randomNum = -10*2*M_PI/10;

    return randomNum; // losowe zmienne od a do b
}
void PottsSimulation::turnOnField(double value, double direction)
{
    uniformFieldValue = value;
    uniformFieldDirection = direction;

    if(uniformFieldBool) uniformField();
    else nonUniformField();
}

void PottsSimulation::turnOffField()
{
    uniformFieldValue = 0;
    uniformField();
}
void PottsSimulation::saveOutput(QString path)
{
    QString output= generateMsg();

    QFile file(path);
       if (!file.open(QIODevice::Append | QIODevice::Text))
           return;

    QTextStream out(&file);
    out << output;
    file.close();
}


void PottsSimulation::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    xClick = event->x();
    yClick = event->y();
    update();
}

void PottsSimulation::redraw()
{
    for(int i = 0; i < latticeData->getNumberOfNodes(); i++) stepMC();

    counterCalc++;
    calculate();
    update();
}

QString PottsSimulation::generateMsg()
{
    double namagnesowanie = Msr/double(maxIteration);
    double energia = Esr/double(maxIteration);
    double energia2 = E2sr/double(maxIteration);
    double namagnesowanie2 = M2sr/double(maxIteration);
    double kumulanta = 1 - (  ( M4sr/double(maxIteration) )/( 3 * pow((M2sr/double(maxIteration)),2) )  );
    double Cp = (energia2 - (energia*energia)) / (T*T);
    double podatnosc = (namagnesowanie2 - (namagnesowanie*namagnesowanie))/T;
    timer->stop();
    isRunning=false;

    QString currentText = "";
    currentText +="----------------------------------------------- \n";
    currentText +="Ilość iteracji: "+QString::number(counterCalc)+"\n";
    currentText +="Rozmiar sieci: "+QString::number(latticeSize)+"\n";
    currentText +="Temperatura: "+QString::number(T)+"\n";
    currentText +="----------------------------------------------- \n";
    currentText +="Energia: "+QString::number(energia)+"\n";
    currentText +="Namagnesowanie: "+QString::number(namagnesowanie)+"\n";
    currentText +="Ciepło właściwe: "+QString::number(Cp)+"\n";
    currentText +="Podatność magnetyczna: "+QString::number(podatnosc)+"\n";
    currentText +="Kumulanta Bindera: "+QString::number(kumulanta)+"\n";
    currentText +="----------------------------------------------- \n";


    return currentText;

}
