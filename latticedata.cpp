#include "latticedata.h"
#include <QDebug>


LatticeData::LatticeData(int latticeN ,int latticeS)
{
    latticeNumber = latticeN;
    latticeSize = latticeS ;
    init();
}

void LatticeData::init()
{
    switch (latticeNumber) {

    case 0: lattice0(); break;
    case 1: lattice1(); break;
    case 2: lattice2(); break;
    case 3: lattice3(); break;
    case 4: lattice4(); break;
    case 5: lattice5(); break;


    }
}

void LatticeData::lattice0()
{
    preSpin = new node[4*latticeSize*latticeSize];

    numberOfNeighbour = 4;
    int nodeNr = 0;
    int xTrans{0};
    int yTrans{0};


    for(int x = 0 ; x < latticeSize ; x++)
    {
        for(int y = 0 ; y < latticeSize ; y ++ )
        {
            for( int k = 0 ; k < 4 ; k++)
            {
                preSpin[nodeNr+k].xInLattice = x;
                preSpin[nodeNr+k].yInLattice = y;
            }

            preSpin[nodeNr].x = xTrans;
            preSpin[nodeNr].y = yTrans;

            preSpin[nodeNr+1].x = xTrans+1;
            preSpin[nodeNr+1].y = yTrans;

            preSpin[nodeNr+2].x = xTrans;
            preSpin[nodeNr+2].y = yTrans+1;

            preSpin[nodeNr+3].x = xTrans+1;
            preSpin[nodeNr+3].y = yTrans+1;

            nodeNr+=4;   
            yTrans++;

        }
        xTrans++;
        yTrans = 0;
    }

    list.append(&preSpin[0]);
    bool appendNode{true};

    for(int i = 1; i<latticeSize*latticeSize*4; i++)
    {
        appendNode = true;

        for(int j = 0; j < list.size(); j++)
        {
            if(preSpin[i].x == list[j]->x && preSpin[i].y == list[j]->y)
            {
                appendNode = false;
                break;
            }
        }

        if(appendNode) list.append(&preSpin[i]);
    }

    numberOfNodes = list.size();
    spin = new node [numberOfNodes];
    for(int i= 0; i<numberOfNodes; i++)
    {
        spin[i].x = list[i]->x;
        spin[i].y = list[i]->y;

        spin[i].xInLattice = list[i]->xInLattice;
        spin[i].yInLattice = list[i]->yInLattice;
    }


    // dodanie sąsiadów:
    for(int i = 0; i < numberOfNodes; i ++)
    {
        int nr = 0;

        for(int j = 0; j <numberOfNodes; j ++)
        {
            double l = pow(( ((spin[i].x - spin[j].x)*(spin[i].x - spin[j].x)) + ((spin[i].y - spin[j].y)*(spin[i].y - spin[j].y)) ),0.5);

            if(l == 1 )
            {
                spin[i].neighbour[nr] = j;
                nr++;
            }
            else if( spin[i].y == spin[j].y && (spin[i].x == spin[j].x-latticeSize || spin[i].x - latticeSize  == spin[j].x))
            {
                spin[i].neighbour[nr] = j;
                nr++;
            }
            else if( spin[i].x == spin[j].x && (spin[i].y== spin[j].y-latticeSize  || spin[i].y - latticeSize == spin[j].y) )
            {
                spin[i].neighbour[nr] = j;
                nr++;
            }

        }

    }


}

void LatticeData::lattice1()
{

    preSpin = new node[4*latticeSize*latticeSize];

    numberOfNeighbour = 6;
    int nodeNr = 0;
    double pow = std::pow(3,0.5)/2;
    double yTrans{pow*latticeSize};
    double xTrans{0.0};

    for(int x = 0 ; x < latticeSize ; x++)
    {
        xTrans = x;
        yTrans = pow*latticeSize;

        for(int y = 0 ; y < latticeSize ; y ++ )
        {

            for( int k = 0 ; k < 4 ; k++)
            {
                preSpin[nodeNr+k].xInLattice = x;
                preSpin[nodeNr+k].yInLattice = y;
            }

            preSpin[nodeNr].x = xTrans;
            preSpin[nodeNr].y = yTrans;

            preSpin[nodeNr+1].x = xTrans+1;
            preSpin[nodeNr+1].y = yTrans;

            preSpin[nodeNr+2].x = xTrans+0.5;
            preSpin[nodeNr+2].y = yTrans-pow;

            preSpin[nodeNr+3].x = xTrans+1.5;
            preSpin[nodeNr+3].y = yTrans-pow;

            nodeNr+=4;

            yTrans -= pow;
            xTrans += 0.5;

        }

    }

    list.append(&preSpin[0]);
    bool appendNode{true};

    for(int i = 1; i<latticeSize*latticeSize*4; i++)
    {
        appendNode = true;

        for(int j = 0; j < list.size(); j++)
        {
            if(std::abs(preSpin[i].x - list[j]->x) < 0.01 && std::abs(preSpin[i].y - list[j]->y) < 0.01)
            {
                appendNode = false;
                break;
            }
        }

        if(appendNode) list.append(&preSpin[i]);
    }

    numberOfNodes = list.size();
    spin = new node [numberOfNodes];
    for(int i= 0; i<numberOfNodes; i++)
    {
        spin[i].x = list[i]->x;
        spin[i].y = list[i]->y;
        spin[i].xInLattice = list[i]->xInLattice;
        spin[i].yInLattice = list[i]->yInLattice;
    }

    int buff;
    for(int i = 0; i < numberOfNodes; i ++)
    {
        int nr = 0;
        for(int j = 0; j <numberOfNodes; j ++)
        {
            double l = std::pow(( ((spin[i].x - spin[j].x)*(spin[i].x - spin[j].x)) + ((spin[i].y - spin[j].y)*(spin[i].y - spin[j].y)) ),0.5);

            // znajdywanie i dodawanie sąsiadów
            if(l > 0.99 && l < 1.01 )
            {
                spin[i].neighbour[nr] = j;
                nr++;
            }
            //dodawanie warunków brzegowych
            else if((l > latticeSize- 0.01 && l < latticeSize+0.01 )|| (l < 2 * latticeSize*pow+0.01 && l > 2 * latticeSize*pow-0.01 ))
            {
                spin[i].neighbour[nr] = j;
                nr++;
                buff=j;
            }
        }

        while (nr < 6)
        {
            spin[i].neighbour[nr] = buff;
            nr++;
        }

    }

}



void LatticeData::lattice2()
{
    numberOfNeighbour = 3;

    preSpin = new node[6*latticeSize*latticeSize];

    int nodeNr = 0;
    double pow = std::pow(3,0.5)/2;
    double yTrans;
    double xTrans;

    for(int x = 0 ; x < latticeSize ; x++)
    {
        xTrans = x*2*pow;
        yTrans = 3*latticeSize/2;

        for(int y = 0 ; y < latticeSize ; y ++ )
        {
            for( int k = 0 ; k < 6 ; k++)
            {
                preSpin[nodeNr+k].xInLattice = x;
                preSpin[nodeNr+k].yInLattice = y;
            }

            preSpin[nodeNr].x = xTrans;
            preSpin[nodeNr].y = yTrans;

            preSpin[nodeNr+1].x = xTrans;
            preSpin[nodeNr+1].y = yTrans-1;

            preSpin[nodeNr+2].x = xTrans+pow;
            preSpin[nodeNr+2].y = yTrans-1.5;

            preSpin[nodeNr+3].x = xTrans+2*pow;
            preSpin[nodeNr+3].y = yTrans-1;

            preSpin[nodeNr+4].x = xTrans+2*pow;
            preSpin[nodeNr+4].y = yTrans;

            preSpin[nodeNr+5].x = xTrans+pow;
            preSpin[nodeNr+5].y = yTrans+0.5;

            nodeNr+=6;

            yTrans -= 1.5;
            xTrans += pow;

        }

    }

    list.append(&preSpin[0]);
    bool appendNode{true};

    for(int i = 1; i<latticeSize*latticeSize*6; i++)
    {
        appendNode = true;

        for(int j = 0; j < list.size(); j++)
        {
            if(std::abs(preSpin[i].x - list[j]->x) < 0.01 && std::abs(preSpin[i].y - list[j]->y) < 0.01)
            {
                appendNode = false;
                break;
            }
        }

        if(appendNode) list.append(&preSpin[i]);
    }

    numberOfNodes = list.size();
    spin = new node [numberOfNodes];
    for(int i= 0; i<numberOfNodes; i++)
    {
        spin[i].x = list[i]->x;
        spin[i].y = list[i]->y;
        spin[i].xInLattice = list[i]->xInLattice;
        spin[i].yInLattice = list[i]->yInLattice;
    }

    for(int i = 0; i < numberOfNodes; i ++)
    {
        int nr = 0;
        for(int j = 0; j <numberOfNodes; j ++)
        {
            double l = std::pow(( ((spin[i].x - spin[j].x)*(spin[i].x - spin[j].x)) + ((spin[i].y - spin[j].y)*(spin[i].y - spin[j].y)) ),0.5);

            // znajdywanie i dodawanie sąsiadów
            if(l > 0.9 && l < 1.1 )
            {
                spin[i].neighbour[nr] = j;
                nr++;
            }

        }
        if(nr < 3)
        {
            for(int j = 0; j <numberOfNodes; j ++)
            {
                double l = std::pow(( ((spin[i].x - spin[j].x)*(spin[i].x - spin[j].x)) + ((spin[i].y - spin[j].y)*(spin[i].y - spin[j].y)) ),0.5);

                //dodawanie warunków brzegowych
                if((l > 2*latticeSize*pow - 0.1 && l < 2*latticeSize*pow+0.1 ) || (l < 2*latticeSize*pow+0.1 && l > 2*latticeSize*pow-0.1 ))
                {
                    spin[i].neighbour[nr] = j;
                }

            }
        }

    }


}

void LatticeData::lattice3()
{
    numberOfNeighbour = 5;
    double xTrans{0},yTrans{0};
    double pow = std::pow(3,0.5)/2;
    int nodeNr = 0;

    preSpin = new node[6*latticeSize*latticeSize];
    for(int x = 0; x < latticeSize; x++)
    {

        xTrans=x+0.5;
        yTrans = latticeSize+latticeSize*pow+pow;

        for(int y = 0; y < latticeSize; y++)
        {
            for( int k = 0 ; k < 6 ; k++)
            {
                preSpin[nodeNr+k].xInLattice = x;
                preSpin[nodeNr+k].yInLattice = y;
            }

            preSpin[nodeNr].x  = xTrans;
            preSpin[nodeNr].y  = yTrans;

            preSpin[nodeNr+1].x= xTrans-0.5;
            preSpin[nodeNr+1].y= yTrans-pow;

            preSpin[nodeNr+2].x= xTrans+0.5;
            preSpin[nodeNr+2].y= yTrans-pow;

            preSpin[nodeNr+3].x= xTrans-0.5;
            preSpin[nodeNr+3].y= yTrans-1-pow;

            preSpin[nodeNr+4].x= xTrans+0.5;
            preSpin[nodeNr+4].y= yTrans-1-pow;

            preSpin[nodeNr+5].x= xTrans;
            preSpin[nodeNr+5].y= yTrans-1-2*pow;

            nodeNr+=6;

            xTrans += 0.5;
            yTrans -= 1+pow;

        }

    }

    list.append(&preSpin[0]);
    bool appendNode{true};

    for(int i = 1; i<latticeSize*latticeSize*6; i++)
    {
        appendNode = true;

        for(int j = 0; j < list.size(); j++)
        {
            if(std::abs(preSpin[i].x - list[j]->x) < 0.01 && std::abs(preSpin[i].y - list[j]->y) < 0.01)
            {
                appendNode = false;
                break;
            }
        }

        if(appendNode) list.append(&preSpin[i]);
    }

    numberOfNodes = list.size();
    spin = new node [numberOfNodes];
    for(int i= 0; i<numberOfNodes; i++)
    {
        spin[i].x = list[i]->x;
        spin[i].y = list[i]->y;
        spin[i].xInLattice = list[i]->xInLattice;
        spin[i].yInLattice = list[i]->yInLattice;
    }

    int buff{0};

    // dodawanie sąsiadów wewnątrz sieci
    for(int i = 0; i < numberOfNodes; i ++)
    {
        int nr = 0;

        for(int j = 0; j <numberOfNodes; j ++)
        {
            double l = std::pow((((spin[i].x - spin[j].x)*(spin[i].x - spin[j].x)) + ((spin[i].y - spin[j].y)*(spin[i].y - spin[j].y)) ),0.5);

            if(l > 0.9 && l < 1.1)
            {
                spin[i].neighbour[nr] = j;
                nr++;
            }
            else if( std::abs(spin[i].y - spin[j].y) < 0.01 && static_cast<int>(std::abs(spin[i].x - spin[j].x)) == latticeSize )
            {
                buff = j;
            }
            else if( std::abs(spin[i].y - spin[j].y) > (latticeSize + latticeSize*pow) && static_cast<int>(10*std::abs(spin[i].x - spin[j].x) == 10*latticeSize/2 - 5) )
            {
                buff = j;
            }
        }

        while (nr < 5)
        {
            spin[i].neighbour[nr] = buff;
            nr++;
        }
    }
}

void LatticeData::lattice4()
{

    numberOfNeighbour = 4;
    double xTrans{0},yTrans{0};
    double pow = std::pow(3,0.5)/2;
    int nodeNr = 0;

    preSpin = new node[8*latticeSize*latticeSize];
    for(int x = 0; x < latticeSize; x++)
    {

        xTrans = 2*x;
        yTrans = 2*latticeSize*pow;

        for(int y = 0; y < latticeSize; y++)
        {

            for( int k = 0 ; k < 8 ; k++)
            {
                preSpin[nodeNr+k].xInLattice = x;
                preSpin[nodeNr+k].yInLattice = y;
            }

            preSpin[nodeNr].x  = xTrans;
            preSpin[nodeNr].y  = yTrans;

            preSpin[nodeNr+1].x= xTrans + 0.5;
            preSpin[nodeNr+1].y= yTrans - pow;

            preSpin[nodeNr+2].x= xTrans + 1.5;
            preSpin[nodeNr+2].y= yTrans - pow;

            preSpin[nodeNr+3].x= xTrans + 2.5;
            preSpin[nodeNr+3].y= yTrans - pow;

            preSpin[nodeNr+4].x= xTrans + 2;
            preSpin[nodeNr+4].y= yTrans;

            preSpin[nodeNr+5].x= xTrans + 2.5;
            preSpin[nodeNr+5].y= yTrans + pow;

            preSpin[nodeNr+6].x= xTrans + 1.5;
            preSpin[nodeNr+6].y= yTrans + pow;

            preSpin[nodeNr+7].x= xTrans + 0.5;
            preSpin[nodeNr+7].y= yTrans + pow;

            nodeNr+=8;

            xTrans ++;
            yTrans -= 2*pow;

        }

    }

    list.append(&preSpin[0]);
    bool appendNode{true};

    for(int i = 1; i<latticeSize*latticeSize*8; i++)
    {
        appendNode = true;

        for(int j = 0; j < list.size(); j++)
        {
            if(std::abs(preSpin[i].x - list[j]->x) < 0.01 && std::abs(preSpin[i].y - list[j]->y) < 0.01)
            {
                appendNode = false;
                break;
            }
        }

        if(appendNode) list.append(&preSpin[i]);
    }

    numberOfNodes = list.size();
    spin = new node [numberOfNodes];
    for(int i= 0; i<numberOfNodes; i++)
    {
        spin[i].x = list[i]->x;
        spin[i].y = list[i]->y;
        spin[i].xInLattice = list[i]->xInLattice;
        spin[i].yInLattice = list[i]->yInLattice;
    }

    int buff{0};

    // dodawanie sąsiadów wewnątrz sieci
    for(int i = 0; i < numberOfNodes; i ++)
    {
        int nr = 0;

        for(int j = 0; j <numberOfNodes; j ++)
        {
            double l = std::pow((((spin[i].x - spin[j].x)*(spin[i].x - spin[j].x)) + ((spin[i].y - spin[j].y)*(spin[i].y - spin[j].y)) ),0.5);

            if(l > 0.9 && l < 1.1)
            {
                spin[i].neighbour[nr] = j;
                nr++;
            }

        }
        if( nr <4 )
            for(int j = 0; j <numberOfNodes; j ++)
            {
                // dodawanie warunków brzegowych
                if( std::abs(spin[i].y - spin[j].y) < 0.1 && static_cast<int>(std::abs(spin[i].x - spin[j].x)) == 2*latticeSize )
                {
                    spin[i].neighbour[nr] = j;
                    nr++;
                    buff = j;
                }
                else if( static_cast<int>(std::abs(spin[i].x - spin[j].x)) == latticeSize-1 && std::abs(spin[i].y - spin[j].y)+0.1 > 2*latticeSize*pow )
                {
                    spin[i].neighbour[nr] =j;
                    nr++;
                    buff = j;
                }

            }
        while (nr < 4)
        {
            spin[i].neighbour[nr] = buff;
            nr++;
        }
    }

}

void LatticeData::lattice5()
{
    numberOfNeighbour = 3;
    double xTrans{0},yTrans{0};
    double pow = 1/std::pow(2,0.5);
    int nodeNr = 0;

    preSpin = new node[10*latticeSize*latticeSize];
    for(int x = 0; x < latticeSize; x++)
    {

        xTrans = x*(2+2*pow);
        yTrans = latticeSize*pow + latticeSize;

        for(int y = 0; y < latticeSize; y++)
        {

            for( int k = 0 ; k < 10 ; k++)
            {
                preSpin[nodeNr+k].xInLattice = x;
                preSpin[nodeNr+k].yInLattice = y;
            }

            preSpin[nodeNr].x  = xTrans;
            preSpin[nodeNr].y  = yTrans;

            preSpin[nodeNr+1].x= xTrans ;
            preSpin[nodeNr+1].y= yTrans - 1;

            preSpin[nodeNr+2].x= xTrans + pow;
            preSpin[nodeNr+2].y= yTrans -1 - pow;

            preSpin[nodeNr+3].x= xTrans + pow + 1;
            preSpin[nodeNr+3].y= yTrans -1 - pow;

            preSpin[nodeNr+4].x= xTrans + 2*pow + 1 ;
            preSpin[nodeNr+4].y= yTrans - 1;

            preSpin[nodeNr+5].x= xTrans + 2*pow + 2;
            preSpin[nodeNr+5].y= yTrans - 1 ;

            preSpin[nodeNr+6].x= xTrans + 2*pow + 1;
            preSpin[nodeNr+6].y= yTrans ;

            preSpin[nodeNr+7].x= xTrans + 2*pow + 2;
            preSpin[nodeNr+7].y= yTrans ;

            preSpin[nodeNr+8].x= xTrans + pow;
            preSpin[nodeNr+8].y= yTrans + pow;

            preSpin[nodeNr+9].x= xTrans +pow + 1;
            preSpin[nodeNr+9].y= yTrans +pow;

            nodeNr+=10;
            xTrans += 1+pow;
            yTrans -= 1+pow;

        }

    }

    list.append(&preSpin[0]);
    bool appendNode{true};

    for(int i = 1; i<latticeSize*latticeSize*10; i++)
    {
        appendNode = true;

        for(int j = 0; j < list.size(); j++)
        {
            if(std::abs(preSpin[i].x - list[j]->x) < 0.01 && std::abs(preSpin[i].y - list[j]->y) < 0.01)
            {
                appendNode = false;
                break;
            }
        }

        if(appendNode) list.append(&preSpin[i]);
    }

    numberOfNodes = list.size();
    spin = new node [numberOfNodes];
    for(int i= 0; i<numberOfNodes; i++)
    {
        spin[i].x = list[i]->x;
        spin[i].y = list[i]->y;
        spin[i].xInLattice = list[i]->xInLattice;
        spin[i].yInLattice = list[i]->yInLattice;
    }

    int buff{0};
    // dodawanie sąsiadów wewnątrz sieci
    for(int i = 0; i < numberOfNodes; i ++)
    {
        int nr = 0;

        for(int j = 0; j <numberOfNodes; j ++)
        {
            double l = std::pow((((spin[i].x - spin[j].x)*(spin[i].x - spin[j].x)) + ((spin[i].y - spin[j].y)*(spin[i].y - spin[j].y)) ),0.5);

            if(l > 0.9 && l < 1.1)
            {
                spin[i].neighbour[nr] = j;
                nr++;
            }

        }

        if (nr < 3)
        {
            // dodawanie warunków brzegowych
            for(int j = 0; j <numberOfNodes; j ++)
            {
                if( std::abs(spin[i].y - spin[j].y) < 0.01 &&  std::abs(spin[i].x - spin[j].x) - 0.01 > 2*latticeSize + 2*latticeSize*pow -1)
                {
                    spin[i].neighbour[nr] =j;
                    nr++;
                    buff = j;
                }
                else if( std::abs(spin[i].x - spin[j].x) + 0.1 > (latticeSize-1)*pow + latticeSize-1 &&  std::abs(spin[i].x - spin[j].x) - 0.1 < (latticeSize-1)*pow + latticeSize-1
                         &&  std::abs(spin[i].y - spin[j].y) + 0.1 > latticeSize*pow + latticeSize + pow )
                {
                    spin[i].neighbour[nr] =j;
                    nr++;
                    buff = j;
                }
            }
        }
    }

}



int LatticeData::getNumberOfNodes()
{
    return numberOfNodes;
}

int LatticeData::getNumberOfNeighbour()
{
    return numberOfNeighbour;
}

