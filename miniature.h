#ifndef MINIATURE_H
#define MINIATURE_H

#include <QObject>
#include <QWidget>
#include <latticedata.h>
#include <QPainter>

class Miniature : public QWidget
{
    Q_OBJECT
public:
    explicit Miniature(int latticeN, int xMin, int yMin, QWidget *parent = nullptr);

    LatticeData * latticeData;

protected:
    virtual void paintEvent(QPaintEvent *event) override;
public:
    //zmienne
    int latticeNumber;
    int xScale,yScale,xMiniature,yMiniature,xMove,yMove;
    // funkcje
    void drawMiniature(QPaintEvent * event);
    void drawLattice();
    void moveMin();

signals:

};

#endif // MINIATURE_H
