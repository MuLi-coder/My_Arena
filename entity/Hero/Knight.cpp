//
// Created by ml on 2026/5/26.
//
#include "../Unit.h"
#include "Knight.h"
#include<iostream>
#include<string>
#include <QString>

Knight::Knight(const std::string n, const int init_x, const int init_y,int owner)
    :Unit(n,init_x,init_y, owner) {
    hp = 100;
    armor = 50;
    att = 50;
    image = "../images/Knight.png";
}

void Knight::attack(Unit* target) {
    target->takeDamage(att);
}

QString Knight::getImage() const {
    return image;
}