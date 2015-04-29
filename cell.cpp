#include "cell.h"

Cell::Cell(int number, QGraphicsItem * parent) : QGraphicsRectItem(parent)
{
    // дефолтный размер у всех будет таким
    setRect(0,0,50,50);

    // устанавливаем номер
    num = number;

    // позиционируемс
    switch(num)
    {
        case 0:
        {
        setPos(400 - 75, 300 + 25); break;
        }
        case 1:
        {
        setPos(400 - 25, 300 + 25); break;
        }
        case 2:
        {
        setPos(400 + 25, 300 + 25); break;
        }
        case 3:
        {
        setPos(400 - 75, 300 - 25); break;
        }
        case 4:
        {
        setPos(400 - 25, 300 - 25); break;
        }
        case 5:
        {
        setPos(400 + 25, 300 - 25); break;
        }
        case 6:
        {
        setPos(400 - 75, 300 - 75); break;
        }
        case 7:
        {
        setPos(400 - 25, 300 - 75); break;
        }
        case 8:
        {
        setPos(400 + 25, 300 - 75); break;
        }
    }
}

