//
// Created by ml on 2026/5/30.
//

#include "Player.h"

#include <algorithm>
#include <iostream>
#include <ostream>

Player::Player() {
    refreshTimes = 3;
    money = 10;
    hp = 100;
    level = 1;
    maxUnits = level;
    score = 0;
}

int Player::getMoney() const { return money; }
int Player::getHp() const { return hp; }
int Player::getShopRefreshTimes() const { return refreshTimes; }

void Player::changeHp(const int num) {
    hp += num;
    if (hp < 0 ) {
        hp = 0;
    }
}

void Player::changeScore(const int num) { score += num; }

int Player::getScore() const { return score; }

void Player::changeShopRefreshTimes(int num) {
    refreshTimes += num;
    if (refreshTimes > 3 || refreshTimes < 0) {
        refreshTimes = 3;
    }
}

int Player::getLevel() const { return level;}
int Player::getMaxUnits() const { return maxUnits; }

void Player::changeLevel(int num) {
    level+=num;
    if (level>7) {
        level = 7;
    }
    maxUnits = level;
}

void Player::reset() {
    refreshTimes = 3;
    money = 10;
    hp = 100;
    level = 1;
    maxUnits = 1;
    score = 0;
}

void Player::changeMoney(const int num) {
    money += num;
    if (money < 0) {
        money = 0;
    }
}

void Player::saveTo(QDataStream& out) const {
    out << money << hp << level << maxUnits << score << refreshTimes;
}

void Player::loadFrom(QDataStream& in) {
    in >> money >> hp >> level >> maxUnits >> score >> refreshTimes;
}
