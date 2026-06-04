#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <memory>
#include "../game_logic/board.h"
#include "PieceWidget.h"
#include "../game_logic/GameManager.h"

//这个类我们专门用来显示GUI界面，同时负责向逻辑层的GameManager类传递信息
class GameWindow : public QMainWindow {
    Q_OBJECT
    const int CELL_SIZE = 60;
    //拥有一个管理者，向管理者要数据
    std::unique_ptr<GameManager> gameManager;
    //拖拽状态记录参数
    bool isDragging;
    int dragFromRow;
    int dragFromCol;
    int dragFromPos;
    Unit* dragHero;
    // 容器存储界面上每个格子的指针，方便后续通过指针改颜色
    std::vector<std::vector<PieceWidget*>> gridWidgets;
    std::vector<PieceWidget*> benchWidgets;
private slots:
    void updateBoardUI(); // 刷新场景显示
public:
    explicit GameWindow(QWidget *parent = nullptr);
    PieceWidget* getPieceWidget(int r, int c);
    PieceWidget* getPieceWidget(int pos);
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // GAMEWINDOW_H