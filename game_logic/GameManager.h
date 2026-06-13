//
// Created by ml on 2026/5/30.
//

#ifndef MY_ARENA_GAMEMANAGER_H
#define MY_ARENA_GAMEMANAGER_H

#include <memory>
#include <QTimer>
#include <QObject>

#include "ComBoard.h"
#include "PreBoard.h"
#include "Player.h"
#include "Shop.h"

#include "../entity/Hero/Knight.h"
#include "../entity/Hero/Mage.h"

enum class GameState {
    Prepare,
    Combat,
    Resolve
};

class GameManager : public QObject{
    Q_OBJECT

    //拥有一个计时器
    QTimer timer;
    //拥有当前状态，控制游戏核心
    GameState currentState;
    //拥有棋盘数据
    std::unique_ptr<PreBoard> preBoard;
    std::unique_ptr<ComBoard> comBoard;
    //拥有玩家数据
    std::unique_ptr<Player> player;
    //拥有商店数据
    std::unique_ptr<Shop> shop;

    //状态对应的处理函数
    void handlePrepare();
    void handleCombat();
    void handleResolve();

public:
    GameManager(int r=8, int c=8,int pos=8);
    //游戏状态
    GameState getCurrentState() const;
    void changeStateTo(GameState newGameState);
    void reset();
    bool isTimerActive() const;
    void timerStart();
    void timerStop();
    bool isComEnd() const;
    //棋盘状态
    int getRow() const;
    int getCol() const;
    int getBen() const;
    Unit* getUnitAtGrid(int r,int c) const;
    Unit* getUnitAtBench(int pos) const;
    bool isCellEmptyGrid(int r, int c) const;
    bool isCellEmptyBench(int pos) const;
    void removeUnitAtGrid(int r, int c);
    void removeUnitAtBench(int pos);
    void placeUnitAtGrid(int r, int c,Unit* unit) ;
    void placeUnitAtBench(int pos,Unit* unit) ;

    //SHOP
    bool isCellEmptyShop(int s)const;
    Unit* getUnitAtShop(int s)const;
    void placeUnitAtShop(int s,Unit* unit);
    void removeUnitAtShop(int s);
    void refreshShop();
    //向外发送的信号参数
    signals:
    void shouldUpdate();
};


#endif //MY_ARENA_GAMEMANAGER_H