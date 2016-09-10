#include "game.h"
#include "cell.h"

Game::Game(QWidget *parent)
    : QGraphicsView(parent)
    , oldCrossNum(-1)
{
    // приготавливаем наш вектор, хранящий информацию о клетках

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

    // создадим клетки и добавим на сцену
    fieldSize = 9;
    cellSize = 30;

    int w = scene->width();
    int h = scene->height();
    int startPosX = w/2 - (cellSize / 2) * fieldSize;
    int startPosY = h/2 - (cellSize / 2) * fieldSize;

    for(int i = 0; i < fieldSize; i++)
    {
        for(int j = 0; j < fieldSize; j++)
        {
            cell = new Cell(i * fieldSize + j,
                            this,
                            startPosX + j * cellSize,
                            startPosY + i * cellSize,
                            cellSize);
            cells << cell;
            scene->addItem(cell);
        }
    }
    // не помню зачем здесь этот show. Скорее всего не нужен
    show();
}

void Game::mouseMoveEvent(QMouseEvent *event)
{
    if(check(event->pos()))
    {
        auto curItem = itemAt(event->x(), event->y());
        Cell *cellItem = static_cast<Cell *>(curItem);

        if(oldCrossNum == -1) { oldCrossNum = cellItem->num; }

        if (oldCrossNum != cellItem->num) {
            QVector <Cell *> oldCrossNumCells = neighbours(oldCrossNum);
            oldCrossNumCells << cells.at(oldCrossNum);
            for (auto c : oldCrossNumCells) {
                c->setColor(Qt::white);
            }
        } else { if(cells.at(oldCrossNum)->color() != Qt::white) return; }

        QVector <Cell *> cross = neighbours(cellItem);
        cross << cellItem;

        if(turn % 2 == 0) {
            for (auto c : cross) {
                c->setColor(Qt::lightGray);
            }
        } else {
            for (auto c : cross) {
                c->setColor(Qt::lightGray);
            }
        }

        oldCrossNum = cellItem->num;

    } else {
        if (oldCrossNum == -1) return;
        QVector <Cell *> oldCrossNumCells = neighbours(oldCrossNum);
        oldCrossNumCells << cells.at(oldCrossNum);
        for (auto c : oldCrossNumCells) {
            c->setColor(Qt::white);
        }
        oldCrossNum = -1;
    }
}

void Game::mousePressEvent(QMouseEvent *event)
{
    QGraphicsItem *curItem;

    QRect r(scene->width()/2 - (cellSize / 2) * fieldSize,
            scene->height()/2 - (cellSize / 2) * fieldSize,
            fieldSize * cellSize,
            fieldSize * cellSize);
    if (r.contains(event->pos())) // это клетка!
    {
        if(std::all_of(cells.begin(),
                       cells.end(),
                       [](Cell* c) {
                       return c->color() != Qt::white; }))
        {
            for(auto c : cells) {
                c->setColor(Qt::white);
            }
        }
        else
        {
            curItem = itemAt(event->x(), event->y());
            if(curItem == NULL)// на левой и нижней границе не возвращает клетку на последнем пикселе
                return;
            Cell *cellItem = static_cast<Cell *>(curItem);
            if (check(cellItem)) {
                QVector <Cell *> cross = neighbours(cellItem);
                cross << cellItem;
                if(turn % 2 == 0) {
                    for (auto c : cross) {
                        c->setColor(Qt::red);
                    }
                } else {
                    for (auto c : cross) {
                        c->setColor(Qt::blue);
                    }
                }
                // костыль
                //cellItem->setPen(* new QPen(Qt::NoPen));
                //cross.at(1)->setPen(* new QPen(Qt::NoPen));
                //cross.at(3)->setPen(* new QPen(Qt::NoPen));

                if(gameIsOver()){}

                oldCrossNum = -1;
                turn = (turn + 1) % 2;
            }
        }
    }
}

bool Game::check(Cell *c)
{
    return check(c->num);
}

bool Game::check(QPoint p)
{
/*
    qDebug() << scene->width()/2 - (cellSize / 2) * fieldSize << " - "
             << scene->width()/2 - (cellSize / 2) * fieldSize + fieldSize * cellSize << endl;
    qDebug() << scene->height()/2 - (cellSize / 2) * fieldSize << " - "
             << scene->height()/2 - (cellSize / 2) * fieldSize + fieldSize * cellSize << endl;

    qDebug() << "point(" << p.x() << "," << p.y() << ")"<< endl;
    */
    QRect r(scene->width()/2 - (cellSize / 2) * fieldSize,
            scene->height()/2 - (cellSize / 2) * fieldSize,
            fieldSize * cellSize,
            fieldSize * cellSize);

    if (r.contains(p)) // это клетка!
    {
        QGraphicsItem *curItem = itemAt(p.x(), p.y());
        if(curItem == NULL)// на левой и нижней границе не возвращает клетку на последнем пикселе
            return false;
        Cell *c = static_cast<Cell *>(curItem);
        return check(c->num);
    } else {
        return false;
    }
}

bool Game::gameIsOver()
{
    return !std::any_of(cells.begin(), cells.end(), [&] (Cell *currCell){return check(currCell);});
}

bool Game::check(int num)
{
    if (num < fieldSize) return false;                    // top border
    if (num % fieldSize == 0) return false;               // left
    if (num % fieldSize == fieldSize - 1) return false;   // right
    if (num >= fieldSize * (fieldSize - 1)) return false; // bottom
    QVector <Cell *> nei = neighbours(num);
    return std::all_of(nei.begin(), nei.end(),
                    [](Cell *c) {return (c->color() == Qt::white) ||
                                        (c->color() == Qt::lightGray);});
}

QVector<Cell *> Game::neighbours(Cell *c)
{
    return neighbours(c->num);
}

QVector<Cell *> Game::neighbours(int num)
{
    QVector<Cell *> neigh;
    neigh << cells.at(num - 1)
          << cells.at(num + 1)
          << cells.at(num - fieldSize)
          << cells.at(num + fieldSize);
    return neigh;
}
