#include "game.h"
#include "cell.h"

Game::Game(QWidget *parent)
    : QGraphicsView(parent)
    , oldCross(-1)
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
    fieldSize = 4;
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
    //show();
}

void Game::mouseMoveEvent(QMouseEvent *event)
{
    if(check(event->pos()))
    {
        auto curItem = itemAt(event->x(), event->y());
        Cell *cellItem = static_cast<Cell *>(curItem);

        if(oldCross == -1) { oldCross = cellItem->num; }

        if (oldCross != cellItem->num) {
            QVector <Cell *> oldCrossCells = neighbours(oldCross);
            oldCrossCells << cells.at(oldCross);
            for (auto c : oldCrossCells) {
                c->setColor(Qt::white);
            }
        } else { if(cells.at(oldCross)->color() != Qt::white) return; }

        oldCross = cellItem->num;

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
    } else {
        if (oldCross == -1) return;
        QVector <Cell *> oldCrossCells = neighbours(oldCross);
        oldCrossCells << cells.at(oldCross);
        for (auto c : oldCrossCells) {
            c->setColor(Qt::white);
        }
        oldCross = -1;
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
                cellItem->setPen(* new QPen(Qt::NoPen));
                cross.at(1)->setPen(* new QPen(Qt::NoPen));
                cross.at(3)->setPen(* new QPen(Qt::NoPen));
                oldCross = -1;
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

    QRect r(scene->width()/2 - (cellSize / 2) * fieldSize,
            scene->height()/2 - (cellSize / 2) * fieldSize,
            fieldSize * cellSize,
            fieldSize * cellSize);
    if (r.contains(p)) // это клетка!
    {
        QGraphicsItem *curItem = itemAt(p.x(), p.y());
        Cell *c = static_cast<Cell *>(curItem);
        return check(c->num);
    } else {
        return false;
    }
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
