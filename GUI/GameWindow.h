#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <memory>
#include "PieceWidget.h"
#include "ParaWidget.h"
#include "EquipWidget.h"
#include "../game_logic/GameManager.h"


//这个类我们专门用来显示GUI界面，同时负责向逻辑层的GameManager类传递信息
class GameWindow : public QMainWindow {
    Q_OBJECT
    const int CELL_SIZE = 60;
    //只需要拥有一个管理者，向管理者要数据，管理者拥有所有的数据
    std::unique_ptr<GameManager> gameManager;

    //鼠标拖拽事件，拖拽状态记录参数
    bool isDragging;
    bool isEquipping;
    int dragFromRow;
    int dragFromCol;
    int dragFromPos;
    int dragFromEquip;
    Unit* dragHero;
    Unit* shopHero;
    Equipment* dragEquipment;

    // 页面，需要维护的数据
    std::vector<std::vector<PieceWidget*>> gridWidgets;
    std::vector<PieceWidget*> benchWidgets;
    std::vector<PieceWidget*> shopBenchWidgets;
    std::vector<EquipWidget*> equipWidgets;
    ParaWidget* paraPart;


    //信号槽函数
private slots:
    void updateBoardUI(); // 刷新场景显示
    void resolveMessageShow(ComResult result,int hp);
    void failToCombatMessage();

public:
    explicit GameWindow(QWidget *parent = nullptr);
    PieceWidget* getPieceWidget(int r, int c);
    PieceWidget* getPieceWidget(int pos);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#endif // GAMEWINDOW_H