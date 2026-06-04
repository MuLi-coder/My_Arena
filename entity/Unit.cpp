#include <iostream>
#include <string>
#include "Unit.h"

Unit::Unit(std::string n,int pos,int owner)
        : name(std::move(n)) ,owner(owner),pos(pos){
    maxHp = 100;
    hp = 10;
    x = 0;
    y = 0;
}


void Unit::move(const int new_x,const int new_y){
    x = new_x;
    y = new_y;
    std::cout << name << " moved to (" << x << ", " << y << ")" << std::endl;
}

void Unit::takeDamage(const int dmg){
    hp -= dmg;
    if (hp < 0) hp = 0;
    std::cout << name << " took " << dmg << " dmg. Remaining HP: " << hp << std::endl;
}

void Unit::setX(int new_x) { x = new_x; }
void Unit::setY(int new_y) { y = new_y; }

std::string Unit::getName()const{ return name;}

int Unit::getHp() const { return hp; }

int Unit::getX() const { return x; }

int Unit::getY() const { return y; }

int Unit::getOwner() const { return owner; }

int Unit::getMaxHp() const { return maxHp; }

//寻路和攻击逻辑就在这里了
void Unit::march() {
    if (this->getOwner()==0) {
        this->setX(x-1);
    }else {
        this->setX(x+1);
    }
}