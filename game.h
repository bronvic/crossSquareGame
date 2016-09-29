#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QAction>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVector>
#include <QPointF>
#include <QPair>

#include <QComboBox>

#include <QWidget>

class Cell;
class GameView;
class GameWindow;

class GameWindow : public QMainWindow
{
    Q_OBJECT
public:
    GameWindow(QWidget *parent = 0);

    GameView *view;

    void createSettingsWindow();
    void createAboutGameWindow();
    void createAboutDevsWindow();


private:
    void setComboboxesColor(QComboBox *box, Qt::GlobalColor color);
    QVector<Qt::GlobalColor> colorOrder;

    QDialog *settings;

signals:
    void changeColoures(QPair<Qt::GlobalColor, Qt::GlobalColor>);
};

class GameView : public QGraphicsView
{
    Q_OBJECT
public:
    GameView(QWidget *parent = 0);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    QGraphicsScene *scene;
    Cell *cell;
    QPointF null_point;

    void clearField();

    friend void GameWindow::createSettingsWindow();

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
    void loadPlayersColores();
    int fieldSize;
    int cellSize;
    QVector<Cell*> cells;
    int turn = 0;
    QPair<Qt::GlobalColor, Qt::GlobalColor> colores;


public slots:
    void changeColoures(QPair<Qt::GlobalColor, Qt::GlobalColor>);
};

#endif // GAME_H
