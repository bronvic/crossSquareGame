#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QApplication>
#include <QGraphicsView>
#include <QTimer>
#include <QMouseEvent>

#include "game.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GameWindow *gameWindow = new GameWindow();
    gameWindow->show();

    __attribute__((unused))
            GameLogic *gameLogic = new GameLogic();

    return a.exec();
}

