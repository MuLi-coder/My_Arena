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
    void timer_stop();
    bool isComEnd() const;
    //棋盘状态
    int getRow() const;
    int getCol() const;
    int getBen() const;
    Unit* getUnitAt(int r,int c) const;
    Unit* getUnitAt(int pos) const;
    bool isCellEmpty(int r, int c) const;
    bool isEmpty(int pos) const;
    void removeUnitAt(int r, int c);
    void removeUnitAt(int pos);
    void placeUnitAt(int r, int c,Unit* unit) ;
    void placeUnitAt(int pos,Unit* unit) ;

    //向外发送的信号参数
    signals:
    void shouldUpdate();
};


#endif //MY_ARENA_GAMEMANAGER_H