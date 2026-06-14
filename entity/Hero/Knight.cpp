//
// Created by ml on 2026/5/26.
//
#include "../Unit.h"
#include "Knight.h"
#include<iostream>
#include<string>
#include <QString>

Knight::Knight(int owner)
    :Unit( owner) {
    hp = 100;
    maxHp = 100;
    armor = 50;
    att = 8;
    image = "../images/Knight.png";
    attArea =1;
    cost = 1;
}

