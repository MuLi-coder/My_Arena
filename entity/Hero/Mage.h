//
// Created by ml on 2026/5/26.
//

#ifndef MY_ARENA_MAGE_H
#define MY_ARENA_MAGE_H
#include <iostream>
#include <QString>
#include "../Unit.h"

class Mage : public Unit{
public:
    Mage(std::string n,int owner=0);
};


#endif //MY_ARENA_MAGE_H