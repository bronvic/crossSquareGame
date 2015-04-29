#ifndef CELL_H
#define CELL_H

#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QBrush>
#include <QDebug>
#include <QObject>

class Cell : public QGraphicsRectItem, public QObject
{
    //Q_OBJECT
public:
    Cell(int number, QGraphicsItem *parent = 0);
    int num;
};

#endif // CELL_H
