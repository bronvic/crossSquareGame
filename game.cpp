#include "game.h"
#include "cell.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
// всё для qdialog
#include <QDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

#include <QDebug>

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)/*,
      colorOrder(Qt::red, Qt::darkRed, Qt::green, Qt::darkGreen, Qt::blue, Qt::darkBlue,
                 Qt::cyan, Qt::darkCyan, Qt::magenta, Qt::darkMagenta, Qt::yellow, Qt::darkYellow)*/
{
    // основные параметры
    setFixedSize(800,600);
    setWindowTitle("Крестики на квадратах");

    colorOrder.reserve(12);
    colorOrder = {Qt::red, Qt::darkRed, Qt::green, Qt::darkGreen, Qt::blue, Qt::darkBlue,
                  Qt::cyan, Qt::darkCyan, Qt::magenta, Qt::darkMagenta, Qt::yellow, Qt::darkYellow};

    // задать menubar
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // задать turn label
    turnLabel = new QLabel(this);
    turnLabel->resize(this->width(), menuBar->height());
    turnLabel->move(turnLabel->pos().x(), turnLabel->pos().y() + menuBar->height());
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::red);
    turnLabel->setPalette(palette);
    turnLabel->setText("Ход игрока 1");
    turnLabel->setAlignment(Qt::AlignHCenter);
    QFont f("Mono space");
    f.setBold(true);
    f.setPixelSize(20);
    turnLabel->setFont(f);

    // создать сцену
    view = new GameView(this);
    view->move(view->pos().x(), view->pos().y() + menuBar->height() + turnLabel->height());

    // настроить menubar
    QMenu *gameMenu = new QMenu("Игра", this);
    menuBar->addMenu(gameMenu);
    __attribute__((unused))
            QAction *restartAction = gameMenu->addAction("Заново", view, &GameView::clearField, QKeySequence("F2"));

    __attribute__((unused))
            QAction *settingsAction = gameMenu->addAction("Настройки", this, &GameWindow::createSettingsWindow, QKeySequence("F3"));
    gameMenu->addSeparator();

    __attribute__((unused))
            QAction *exitAction = gameMenu->addAction("Выход", &QApplication::quit, QKeySequence("Ctrl+Q"));

    QMenu *helpMenu = new QMenu("Справка", this);
    menuBar->addMenu(helpMenu);

    __attribute__((unused))
            QAction *aboutGameAction = helpMenu->addAction("Об игре", this, &GameWindow::createAboutGameWindow, QKeySequence("F1"));

    __attribute__((unused))
            QAction *aboutDevsAction = helpMenu->addAction("О разработчиках", this, &GameWindow::createAboutDevsWindow, QKeySequence("F12"));


    connect(this, &GameWindow::changeColoures, view, &GameView::changeColoures);
    connect(view, &GameView::changeTurnLabel, this, &GameWindow::changeTurnLabel);
}

void GameWindow::createSettingsWindow()
{
    settings = new QDialog(this);
    settings->resize(300,100);
    settings->setFixedSize(300,100);
    settings->setWindowTitle("Настройки");

    QGridLayout *layout = new QGridLayout(settings);
    QLabel *colorLabel = new QLabel("Цвет:");
    QLabel *colorFirstPlayerLabel = new QLabel("Игрок 1");
    QLabel *colorSecondPlayerLabel = new QLabel("Игрок 2");
    QComboBox *colorFirstPlayerCBox = new QComboBox();
    QComboBox *colorSecondPlayerCBox = new QComboBox();

    QPushButton *okButton = new QPushButton("Принять");
    QPushButton *noButton = new QPushButton("Отклонить");


    for(int i = 0; i < colorOrder.size(); i++)
    {
        colorFirstPlayerCBox->addItem("");
        colorFirstPlayerCBox->setItemData(i,QColor(colorOrder.at(i)), Qt::BackgroundColorRole);


        colorSecondPlayerCBox->addItem("");
        colorSecondPlayerCBox->setItemData(i,QColor(colorOrder.at(i)), Qt::BackgroundColorRole);
    }


    setComboboxesColor(colorFirstPlayerCBox, view->colores.first);
    setComboboxesColor(colorSecondPlayerCBox, view->colores.second);
    colorFirstPlayerCBox->setCurrentIndex(colorOrder.indexOf(view->colores.first));
    colorSecondPlayerCBox->setCurrentIndex(colorOrder.indexOf(view->colores.second));


    layout->addWidget(colorLabel,0,0);
    layout->addWidget(colorFirstPlayerLabel,1,0);
    layout->addWidget(colorSecondPlayerLabel,1,2);
    layout->addWidget(colorFirstPlayerCBox,1,1);
    layout->addWidget(colorSecondPlayerCBox,1,3);
    layout->addWidget(okButton,2,3);
    layout->addWidget(noButton,2,0);

    QObject::connect(colorFirstPlayerCBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                     [=] (int index){
        setComboboxesColor(colorFirstPlayerCBox, colorOrder.at(index));

    });
    QObject::connect(colorSecondPlayerCBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                     [=] (int index){
        setComboboxesColor(colorSecondPlayerCBox, colorOrder.at(index));
    });

    QObject::connect(okButton, &QPushButton::pressed,
                     [=] (){
        QPair<Qt::GlobalColor, Qt::GlobalColor> currentColoures( colorOrder.at(colorFirstPlayerCBox->currentIndex()), colorOrder.at(colorSecondPlayerCBox->currentIndex()) );
        emit changeColoures(currentColoures);
    });
    QObject::connect(okButton, &QPushButton::pressed, settings, &QDialog::close);

    QObject::connect(noButton, &QPushButton::pressed, settings, &QDialog::close);

    settings->show();
}

void GameWindow::createAboutGameWindow()
{
    QDialog *aboutGame = new QDialog(this);
    aboutGame->resize(500,150);
    aboutGame->setFixedSize(500,150);
    aboutGame->setWindowTitle("Об игре");

    QVBoxLayout *layout = new QVBoxLayout(aboutGame);
    QLabel *aboutGameLabel = new QLabel();
    QPushButton *aboutGameOkButton = new QPushButton("Ок");


    aboutGameLabel->setText("Правила игры:\n"
                          "Ставьте кресты по очереди. Игрок, сделавший ход последним, проигрывает.\nИгра заканчивается когда на поле нет места для очередного креста");
    aboutGameLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(aboutGameLabel);
    layout->addWidget(aboutGameOkButton);

    connect(aboutGameOkButton, &QPushButton::clicked, aboutGame, &QDialog::close);

    aboutGame->show();
}

void GameWindow::createAboutDevsWindow()
{
    QDialog *aboutDevs = new QDialog(this);
    aboutDevs->resize(550,150);
    aboutDevs->setFixedSize(550,150);
    aboutDevs->setWindowTitle("О разработчиках");

    QVBoxLayout *layout = new QVBoxLayout(aboutDevs);
    QLabel *aboutDevsLabel = new QLabel();
    QLabel *nameLabel = new QLabel("Voronwe");
    QPushButton *aboutDevsOkButton = new QPushButton("Ок");


    aboutDevsLabel->setText("Спасибо что скачали мою игру! Для связи со мной используйте сарафанное радио!\n"
                          "Вы можете помочь проекту предложив свои правки на:\nhttps://github.com/bronvic/crossSquareGame");
    //aboutDevsLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setAlignment(Qt::AlignRight);

    layout->addWidget(aboutDevsLabel);
    layout->addWidget(nameLabel);
    layout->addWidget(aboutDevsOkButton);

    connect(aboutDevsOkButton, &QPushButton::clicked, aboutDevs, &QDialog::close);

    aboutDevs->show();
}

void GameWindow::setComboboxesColor(QComboBox *box, Qt::GlobalColor color)
{
    //qDebug() << static_cast<int> (color);
    switch (color) {
    case Qt::red:
        box->setStyleSheet("QComboBox { background-color: red; }");
        break;
    case Qt::darkRed:
        box->setStyleSheet("QComboBox { background-color: darkRed; }");
        break;
    case Qt::green:
        box->setStyleSheet("QComboBox { background-color: green; }");
        break;
    case Qt::darkGreen:
        box->setStyleSheet("QComboBox { background-color: darkGreen; }");
        break;
    case Qt::blue:
        box->setStyleSheet("QComboBox { background-color: blue; }");
        break;
    case Qt::darkBlue:
        box->setStyleSheet("QComboBox { background-color: darkBlue; }");
        break;
    case Qt::cyan:
        box->setStyleSheet("QComboBox { background-color: cyan; }");
        break;
    case Qt::darkCyan:
        box->setStyleSheet("QComboBox { background-color: darkCyan; }");
        break;
    case Qt::magenta:
        box->setStyleSheet("QComboBox { background-color: magenta; }");
        break;
    case Qt::darkMagenta:
        box->setStyleSheet("QComboBox { background-color: darkMagenta; }");
        break;
    case Qt::yellow:
        box->setStyleSheet("QComboBox { background-color: yellow; }");
        break;
    case Qt::darkYellow:
        box->setStyleSheet("QComboBox { background-color: darkYellow; }");
        break;
    default:
        box->setStyleSheet("QComboBox { background-color: white; }");
        break;
    }
}

void GameWindow::changeTurnLabel(int turn)
{
    turnLabel->setText(QString("Ход игрока " + QString::number(turn + 1)));

    QPalette palette;
    if(!turn % 2)
        palette.setColor(QPalette::WindowText, view->colores.first);
    else
        palette.setColor(QPalette::WindowText, view->colores.second);

    turnLabel->setPalette(palette);

}

GameView::GameView(QWidget *parent)
    : QGraphicsView(parent)
    , oldCrossNum(-1)
{
    // новая сцена
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,800,600); // установить размер 800х600

    // установить сцену и настроить
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(scene->width(), scene->height());

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

    loadPlayersColores();
}

void GameView::mouseMoveEvent(QMouseEvent *event)
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

void GameView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsItem *curItem;

    QRect r(scene->width()/2 - (cellSize / 2) * fieldSize,
            scene->height()/2 - (cellSize / 2) * fieldSize,
            fieldSize * cellSize,
            fieldSize * cellSize);
    if (r.contains(event->pos())) // это клетка!
    {// непонятный кусок кода
        /*
        if(std::all_of(cells.begin(),
                       cells.end(),
                       [](Cell* c) {
                       return c->color() != Qt::white; }))
        {
            for(auto c : cells) {
                c->setColor(Qt::white);
            }
        }
        else*/
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
                        c->setColor(colores.first);
                    }
                } else {
                    for (auto c : cross) {
                        c->setColor(colores.second);
                    }
                }
                // костыль
                //cellItem->setPen(* new QPen(Qt::NoPen));
                //cross.at(1)->setPen(* new QPen(Qt::NoPen));
                //cross.at(3)->setPen(* new QPen(Qt::NoPen));

                if(gameIsOver()){
                    QDialog *gameOver = new QDialog(this);
                    gameOver->resize(300,100);
                    gameOver->setMinimumSize(300,100);
                    gameOver->setWindowTitle("Конец игры");

                    QVBoxLayout *layout = new QVBoxLayout(gameOver);
                    QString gameOverMassege("Игра окончена!\nПобедил игрок №");
                    QLabel *endGameLabel = new QLabel();
                    QPushButton *endGameOkButton = new QPushButton("Ок");
                    QPalette palette;

                    if(turn % 2)
                    {
                        gameOverMassege += "1";
                        palette.setColor(QPalette::WindowText, colores.first);
                    }
                    else
                    {
                        gameOverMassege += "2";
                        palette.setColor(QPalette::WindowText, colores.second);
                    }

                    endGameLabel->setText(gameOverMassege);
                    endGameLabel->setPalette(palette);
                    endGameLabel->setAlignment(Qt::AlignCenter);

                    layout->addWidget(endGameLabel);
                    layout->addWidget(endGameOkButton);

                    connect(endGameOkButton, &QPushButton::clicked,
                            [=](){
                        clearField();
                        gameOver->close();
                    });

                    gameOver->show();
                }
                else
                    turn = (turn + 1) % 2;

                emit(changeTurnLabel(turn));
                oldCrossNum = -1;
            }
        }
    }
}

bool GameView::check(Cell *c)
{
    return check(c->num);
}

bool GameView::check(QPoint p)
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

bool GameView::gameIsOver()
{
    return !std::any_of(cells.begin(), cells.end(), [&] (Cell *currCell){return check(currCell);});
}

bool GameView::check(int num)
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

QVector<Cell *> GameView::neighbours(Cell *c)
{
    return neighbours(c->num);
}

QVector<Cell *> GameView::neighbours(int num)
{
    QVector<Cell *> neigh;
    neigh << cells.at(num - 1)
          << cells.at(num + 1)
          << cells.at(num - fieldSize)
          << cells.at(num + fieldSize);
    return neigh;
}

void GameView::loadPlayersColores()
{
    colores.first = Qt::red;
    colores.second = Qt::blue;
}

void GameView::changeColoures(QPair<Qt::GlobalColor, Qt::GlobalColor> newColoures)
{
    if(newColoures.first == newColoures.second)
        return;

    if(newColoures.first != colores.first)
    {
        foreach (auto c, cells) {
            if(c->color() == colores.first)
                c->setColor(newColoures.first);
        }
    }
    if(newColoures.second != colores.second)
    {
        foreach (auto c, cells) {
            if(c->color() == colores.second)
                c->setColor(newColoures.second);
        }
    }

    colores = newColoures;
}

void GameView::clearField()
{
    foreach (auto c, cells) {
        if(c->color() != Qt::white || c->color() != Qt::lightGray)
            c->setColor(Qt::white);
    }

    turn = 0;
    emit(changeTurnLabel(turn));
}
