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
class GameWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit GameWindow(QWidget *parent = nullptr);
    // PieceWidget* getPieceWidget(int row, int col);
    // PieceWidget* getPieceWidget(int pos);
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    void updateBoardUI(); // 刷新场景显示

    //状态记录参数
    bool isDragging;
    int dragFromRow;
    int dragFromCol;
    int dragFromPos;
    Unit* dragHero;

    //场景数据
    std::unique_ptr<Board> board;

    // 保存界面上每个格子的指针，方便后续通过指针改颜色
    std::vector<std::vector<PieceWidget*>> gridWidgets;
    std::vector<PieceWidget*> benchWidgets;

    const int CELL_SIZE = 60;
};

#endif // GAMEWINDOW_H