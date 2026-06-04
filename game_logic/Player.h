//
// Created by ml on 2026/5/30.
//

#ifndef MY_ARENA_PLAYER_H
#define MY_ARENA_PLAYER_H


class Player {
    int money;
    int hp;
public:
    Player();
    //获取信息
    int getMoney() const;
    int getHp() const;
};


#endif //MY_ARENA_PLAYER_H