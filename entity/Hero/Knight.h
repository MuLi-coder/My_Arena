//
// Created by ml on 2026/5/26.
//

#ifndef MY_ARENA_KNIGHT_H
#define MY_ARENA_KNIGHT_H

#include<iostream>
#include<string>
#include "../Unit.h"


class Knight : public Unit{
public:
    Knight(int owner = 0,int level = 1);


    void selfRefresh() override;
    void skillAttack(const std::vector<std::vector<Unit *>> &grid, int row, int col, int r, int c) override;
};


#endif //MY_ARENA_KNIGHT_H