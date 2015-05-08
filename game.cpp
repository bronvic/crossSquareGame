#include "game.h"
#include "cell.h"

Game::Game(QWidget *parent)
    : QGraphicsView(parent)
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
    fieldSize = 15;
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

void
Game::mousePressEvent(QMouseEvent *event)
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
                turn = (turn + 1) % 2;
            }
        }
    }
}

bool Game::check(Cell *c)
{
    return check(c->num);
}

bool Game::check(int num)
{
    if (num < fieldSize) return false;                    // top border
    if (num % fieldSize == 0) return false;               // left
    if (num % fieldSize == fieldSize - 1) return false;   // right
    if (num >= fieldSize * (fieldSize - 1)) return false; // bottom
    QVector <Cell *> nei = neighbours(num);
    return std::all_of(nei.begin(), nei.end(),
                    [](Cell *c) {return c->color() == Qt::white;});
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
