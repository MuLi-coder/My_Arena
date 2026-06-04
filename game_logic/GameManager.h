//
// Created by ml on 2026/5/30.
//

#ifndef MY_ARENA_GAMEMANAGER_H
#define MY_ARENA_GAMEMANAGER_H

#include <memory>
#include <QTimer>
#include <QObject>
#include "Board.h"
#include "Player.h"
enum class GameState {
    Prepare,
    Combat,
    Resolve
};

class GameManager : public QObject{
    Q_OBJECT
    QTimer timer;
    GameState currentState;
    std::unique_ptr<Board> board;
    std::unique_ptr<Player> player;
    void handlePrepare();
    void handleCombat();
    void handleResolve();

public:
    GameManager(int r=8, int c=8,int pos=8);
    //获取
    GameState getCurrentState() const;
    int getRow() const;
    int getCol() const;
    int getBen() const;
    Unit* getUnitAt(int r,int c) const;
    Unit* getUnitAt(int pos) const;
    //判断
    bool isEmpty(int r, int c) const;
    bool isEmpty(int pos) const;
    //操作
    void removeUnitAt(int r, int c);
    void removeUnitAt(int pos);
    void placeUnitAt(int r, int c,Unit* unit) ;
    void placeUnitAt(int pos,Unit* unit) ;
    //状态操作接口
    void changeStateTo(GameState newGameState);
    void Reset();
    void timer_stop();
signals:
    void shouldUpdate();
};


#endif //MY_ARENA_GAMEMANAGER_H