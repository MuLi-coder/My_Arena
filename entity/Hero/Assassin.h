//
// Created by ml on 2026/6/17.
//

#ifndef MY_ARENA_ASSASSIN_H
#define MY_ARENA_ASSASSIN_H

#include "../Unit.h"

class Assassin : public Unit{
public:
    Assassin(int owner=0, int level=1);

    void selfRefresh() override;
    void skillAttack(const std::vector<std::vector<Unit *>> &grid, int row, int col, int r, int c) override;
};


#endif //MY_ARENA_ASSASSIN_H
