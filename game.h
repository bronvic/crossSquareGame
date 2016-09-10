#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QVector>
#include <QPointF>
#include <algorithm>

class Cell;

class Game : public QGraphicsView
{
public:
    Game(QWidget *parent = 0);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    QGraphicsScene * scene;
    Cell *cell;
    QPointF null_point;

protected:
    static int a;

private:
    int oldCrossNum;
    bool check(int num);
    bool check(Cell *c);
    bool check(QPoint p);
    bool gameIsOver();
    QVector <Cell *> neighbours(Cell *c);
    QVector <Cell *> neighbours(int num);
    int fieldSize;
    int cellSize;
    QVector<Cell*> cells;
    int turn = 0;    
};

#endif // GAME_H
