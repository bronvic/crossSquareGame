#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QVector>
#include <vector>

#define FILDE_SIZE 9

#include "cell.h"

class Game : public QGraphicsView
{
public:
    Game(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *event);

    QGraphicsScene * scene;
    Cell * cell0;
    Cell * cell1;
    Cell * cell2;
    Cell * cell3;
    Cell * cell4;
    Cell * cell5;
    Cell * cell6;
    Cell * cell7;
    Cell * cell8;

    std::vector<int> cellMap;
private:
    //QVector<bool> cellMap;
    int turn = 0;
};

#endif // GAME_H
