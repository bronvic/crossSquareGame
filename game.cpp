#include "game.h"
#include "cell.h"

Game::Game(QWidget *parent)
{
    // приготавливаем наш вектор, хранящий информацию о клетках
    for(int i = 0; i < FILDE_SIZE; i++)
        cellMap.push_back(0);

    // новая сцена
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,800,600); // установить размер 800х600

    // установить сцену и настроить
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(800,600);

    // следим за мышью
    setMouseTracking(true);

    // создадим клетки
    cell0 = new Cell(0);
    cell1 = new Cell(1);
    cell2 = new Cell(2);
    cell3 = new Cell(3);
    cell4 = new Cell(4);
    cell5 = new Cell(5);
    cell6 = new Cell(6);
    cell7 = new Cell(7);
    cell8 = new Cell(8);


    // добавим на сцену клетки
    scene->addItem(cell0);
    scene->addItem(cell1);
    scene->addItem(cell2);
    scene->addItem(cell3);
    scene->addItem(cell4);
    scene->addItem(cell5);
    scene->addItem(cell6);
    scene->addItem(cell7);
    scene->addItem(cell8);

    show();
}

void
Game::mousePressEvent(QMouseEvent *event)
{
    QGraphicsItem *curItem;
    Cell *cellItem;

    // проверим не закончилась ли игра
    int sum = 0;
    for(int i = 0; i < FILDE_SIZE; i++)
        sum += cellMap.at(i);

    if(sum >= FILDE_SIZE)
    {
        cell0->setBrush(* new QBrush(Qt::white));
        cell1->setBrush(* new QBrush(Qt::white));
        cell2->setBrush(* new QBrush(Qt::white));
        cell3->setBrush(* new QBrush(Qt::white));
        cell4->setBrush(* new QBrush(Qt::white));
        cell5->setBrush(* new QBrush(Qt::white));
        cell6->setBrush(* new QBrush(Qt::white));
        cell7->setBrush(* new QBrush(Qt::white));
        cell8->setBrush(* new QBrush(Qt::white));

        cellMap.clear();
        for(int i = 0; i < FILDE_SIZE; i++)
            cellMap.push_back(0);
    }
    else
    {
        // получаем item из под курсора
        curItem = itemAt(event->x(), event->y());

        // если там что-то есть
        if(curItem)
        {// и если это преобразуется в клетку
            if(cellItem = static_cast<Cell *>(curItem));
            {// и если место свободно
                if(!cellMap.at(cellItem->num))
                {// красим и говорим, что место занято
                    if(turn == 0)
                    {
                        cellItem->setBrush(* new QBrush(Qt::red));
                        turn++;
                    }
                    else if(turn == 1)
                    {
                        cellItem->setBrush(* new QBrush(Qt::blue));
                        turn--;
                    }
                    cellMap.at(cellItem->num) = 1;
                }
            }
            qDebug() << curItem;
        }
    }
}
