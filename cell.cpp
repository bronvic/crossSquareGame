#include "cell.h"

Cell::Cell(int number,
           GameView *curr_game,
           int px,
           int py,
           int cellSize,
           QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
    , game(curr_game)
    , num(number)
{
    _color = Qt::white;
    setRect(0,0,cellSize, cellSize);
    setPos(px, py);
}

void Cell::setColor(Qt::GlobalColor c)
{
    setBrush(* new QBrush(c));
    _color = c;
}

QColor Cell::color()
{
    return _color;
}
