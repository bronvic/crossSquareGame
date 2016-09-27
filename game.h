#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QVector>
#include <QPointF>
#include <algorithm>

#include <QMenuBar>

class Cell;

class GameView : public QGraphicsView
{
public:
    GameView(QWidget *parent = 0);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    QGraphicsScene *scene;
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

class GameLogic
{
public:
    GameLogic();
};

class GameWindow : public QMainWindow
{
public:
    GameWindow(QWidget *parent = 0);

    GameView *view;
};


#endif // GAME_H
