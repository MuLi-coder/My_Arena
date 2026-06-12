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
    hp = 100;
    armor = 50;
    att = 8;
    image = "../images/Knight.png";
    attArea =1;
}


void Knight::attack(Unit* target) {
    target->takeDamage(att);
}

QString Knight::getImage() const {
    return image;
}