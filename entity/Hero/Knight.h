//
// Created by ml on 2026/5/26.
//

#ifndef MY_ARENA_KNIGHT_H
#define MY_ARENA_KNIGHT_H

#include<iostream>
#include<string>
#include "../Unit.h"


class Knight : public Unit{
    int armor;
public:
    Knight(std::string n,int owner = 0);
};


#endif //MY_ARENA_KNIGHT_H