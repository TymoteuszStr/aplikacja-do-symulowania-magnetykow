#include "isingsimulation.h"

IsingSimulation::IsingSimulation(int latticeN, int size,int W, int H, double temp, int FPS, int j,bool showOnL,bool showA,bool showL, bool showF, QWidget *parent) : QWidget(parent)
{


    latticeNumber = latticeN;
    latticeSize = size;
    width = W;
    height = H;
    T=temp;
    fps=FPS;
    J=j;
    xScale = width/latticeSize;
    yScale = height/latticeSize;

    showOnLattice=showOnL;
    showArrows = showA;
    showLattice =showL;
    showField = showF;
    this->setFixedSize(2000,1000);

    init();

    timer = new  QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(redraw()));
    timer->start(1000/fps);

}

IsingSimulation::~IsingSimulation()
{
    delete latticeData;
}


void IsingSimulation::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
   if(showOnLattice) drawOnLattice();
   if(showArrows) drawArrows();
   if(showLattice)  drawLattice();
   if(showField) drawField();

      drawNeighbour();
}

void IsingSimulation::init()
{
        latticeData = new LatticeData(latticeNumber,latticeSize);
        fieldDirection = new double[latticeData->getNumberOfNodes()];
        fieldValue = new double[latticeData->getNumberOfNodes()];

        for(int i = 0; i < latticeData->getNumberOfNodes(); i++)
        {
            fieldValue[i] = 0.0;
            latticeData->spin[i].theta = randomNumber();
        }

        for(int i = 0; i < latticeData->getNumberOfNodes(); i++)
        {
            fieldDirection[i] = 0.0;
            fieldValue[i] = 0.0;
        }
}

void IsingSimulation::stepMC()
{
    iterator++;
    double dE=0.0;
    int nodeNumber = rand() % (latticeData->getNumberOfNodes());
    double newAngle = toggleSpin(latticeData->spin[nodeNumber].theta) ;

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
        double rand = randomNumber(0.0,1.0);
        if(rand<exp(-dE/T)) {
            latticeData->spin[nodeNumber].theta = newAngle;
            sukces++;
        }
    }

    if( latticeData->spin[nodeNumber].theta>2.0*M_PI)  latticeData->spin[nodeNumber].theta-=2.0*M_PI;
    if( latticeData->spin[nodeNumber].theta<0*M_PI)  latticeData->spin[nodeNumber].theta+=2.0*M_PI;

  // if (iterator%50 == 0 ) qDebug()<<sukces*100/iterator;


}

void IsingSimulation::calculate()
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
         saveOutput("./save/IsingSimulation/default.txt");
     }


}
void IsingSimulation::uniformField()
{
    for(int i = 0; i < latticeData->getNumberOfNodes(); i++)
    {
        fieldDirection[i] = uniformFieldDirection;
        fieldValue[i] = uniformFieldValue;
    }
}

void IsingSimulation::nonUniformField()
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

void IsingSimulation::drawField()
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

QString IsingSimulation::generateMsg()
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

void IsingSimulation::turnOnField(double value, double direction)
{
    uniformFieldValue = value;
    uniformFieldDirection = direction;


    if(uniformFieldBool) uniformField();
    else nonUniformField();
}

void IsingSimulation::turnOffField()
{
    uniformFieldValue = 0;
    uniformField();
}

double IsingSimulation::randomNumber(double a, double b)
{
    double u = static_cast<double>(rand()%RAND_MAX)/RAND_MAX;
    return a+(b-a)*u; // losowe zmienne od a do b
}


void IsingSimulation::drawLattice()
{
    QPainter painter(this);

    for(int i =0; i < latticeData->getNumberOfNodes(); i++)
    {
        for(int k = 0; k < latticeData->getNumberOfNeighbour() ; k++ )
        {
            int neigbourNumber = latticeData->spin[i].neighbour[k];

            double l = (pow(( pow((latticeData->spin[i].x-latticeData->spin[neigbourNumber].x),2) + pow((latticeData->spin[i].y-latticeData->spin[neigbourNumber].y),2)),(0.5)));

               if(l <= 1.9) painter.drawLine(latticeData->spin[i].x*xScale,latticeData->spin[i].y*yScale,
                             latticeData->spin[neigbourNumber].x*xScale,latticeData->spin[neigbourNumber].y*yScale);
        }
    }

}

void IsingSimulation::drawOnLattice()
{
    QPainter painter(this);

    int xSize = xScale-xScale/10;
    int ySize = yScale-yScale/10;

    for(int i =0; i < latticeData->getNumberOfNodes(); i++)
    {
        if(latticeData->spin[i].theta == 0 )  color = QColor(Qt::blue);
        else if (latticeData->spin[i].theta == M_PI)   color = QColor(Qt::red);
        else color=QColor(Qt::white);

        painter.fillRect(latticeData->spin[i].x*xScale - xSize/2,latticeData->spin[i].y*yScale - ySize/2,
                         xSize,ySize,color);

    }
}
void IsingSimulation::drawArrows()
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

void IsingSimulation::setArrow(double x0, double y0, double x1, double y1, double angle)
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


void IsingSimulation::drawNeighbour()
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



double IsingSimulation::randomNumber()
{
    double nr;
    double u=rand()%RAND_MAX/(RAND_MAX-1.0);
    if(u<0.5)nr=0;
    else nr = M_PI;

    return nr;
}

void IsingSimulation::saveOutput(QString path)
{

    QString output= generateMsg();

    QFile file(path);
       if (!file.open(QIODevice::Append | QIODevice::Text))
           return;

    QTextStream out(&file);
    out << output;
    file.close();
}

double IsingSimulation::toggleSpin(double s)
{
    if(s) return 0.0;
    else return M_PI;
}

void IsingSimulation::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    xClick = event->x();
    yClick = event->y();
    update();
}


void IsingSimulation::redraw()
{
    for(int i = 0; i < latticeData->getNumberOfNodes(); i++) stepMC();

    counterCalc++;
    calculate();
    update();
}
