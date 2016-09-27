#ifndef CELL_H
#define CELL_H

#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QBrush>
#include <QDebug>
#include <QObject>

#include "game.h"

class Cell : public QGraphicsRectItem
{
public:
    Cell(int number,
         GameView *curr_game,
         int px, int py,
         int cellSize,
         QGraphicsItem *parent = 0);
    GameView *game;
    int num;
    void setColor(Qt::GlobalColor c);
    QColor color();
private:
    QColor _color;
};

#endif // CELL_H
