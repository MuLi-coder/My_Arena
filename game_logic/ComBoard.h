//
// Created by ml on 2026/6/8.
//

#ifndef MY_ARENA_COMBOARD_H
#define MY_ARENA_COMBOARD_H

#include <iostream>
#include <vector>
#include <map>
#include "../entity/Unit.h"
#include "PreBoard.h"

class ComBoard {
    int row;
    int col;
    std::vector<std::vector<Unit*>> grid;

public:

    ComBoard(const PreBoard& preBoard);
    void setComBoard(const PreBoard& preBoard);
    void reset(const PreBoard& preBoard);
    void clear();
    bool isCellEmpty(int r, int c)const;
    Unit* getUnitAt(int r,int c) const;
    void moveUnitFromTo(int ini_r,int ini_c,int target_r,int target_c,Unit* unit);
    //战斗过程操作
    void marchEveryUnit();
};


#endif //MY_ARENA_COMBOARD_H