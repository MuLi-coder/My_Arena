//
// Created by ml on 2026/5/30.
//

#include "Player.h"

Player::Player() {
    money = 5;
    hp = 50;
}

int Player::getMoney() const { return money; }
int Player::getHp() const { return hp; }
