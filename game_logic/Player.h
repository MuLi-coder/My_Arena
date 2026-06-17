//
// Created by ml on 2026/5/30.
//

#ifndef MY_ARENA_PLAYER_H
#define MY_ARENA_PLAYER_H

#include <vector>
#include <QDataStream>

#include "../entity/Equipment.h"

class Player {
    int refreshTimes;
    int money;
    int hp;
    int level;
    int maxUnits;
    int score;
public:
    Player();
    //获取信息
    void changeScore(int score);
    int getScore()const;
    int getMoney() const;
    int getHp() const;
    int getShopRefreshTimes() const;
    void changeShopRefreshTimes(int num);
    int getLevel()const;
    void changeLevel(int num);
    int getMaxUnits()const;
    void changeHp(int num);
    void changeMoney(int num);
    void reset();
    //存档
    void saveTo(QDataStream& out) const;
    void loadFrom(QDataStream& in);
};


#endif //MY_ARENA_PLAYER_H