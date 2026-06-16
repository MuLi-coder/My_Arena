//
// Created by ml on 2026/6/8.
//

#ifndef MY_ARENA_PREBOARD_H
#define MY_ARENA_PREBOARD_H


#include <vector>
#include "../entity/Unit.h"
class PreBoard {
    //场上整个board实例，包括棋盘grid和备战区bench两个部分
    int row, col, ben;
    std::vector<std::vector<Unit*>> grid;
    std::vector<Unit*> bench;
public:
    PreBoard(int r = 8, int c = 8,int pos = 8 );
    ~PreBoard();
    //获取棋盘和备战区大小
    int getRow() const;
    int getCol() const;
    int getBen() const;
    // 获取指定格子的元素
    Unit* getUnitAt(int r, int c) const;
    Unit* getUnitAt(int pos) const;
    //判断某个坐标是否合法
    bool isPositionValid(int r, int c) const;
    bool isPositionValid(int pos) const;
    // 判断某个格子是否为空,空返回 true,已占用返回false,里面需包含合法性检验
    bool isCellEmpty(int r, int c) const;
    bool isCellEmpty(int pos) const;
    // 在指定格子放置一个指定棋子
    void placeUnitAt(int r, int c, Unit* hero);
    void placeUnitAt(int pos,Unit* hero);
    // 悬空指定格子
    void removeUnitAt(int r, int c);
    void removeUnitAt(int pos);

};

#endif //MY_ARENA_PREBOARD_H