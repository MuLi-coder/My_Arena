//
// Created by ml on 2026/5/26.
//
#include "../Unit.h"
#include "Knight.h"
#include<iostream>
#include<string>
#include <QString>

Knight::Knight(const std::string n, const int pos,int owner)
    :Unit(n,pos, owner) {
    hp = 70;
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