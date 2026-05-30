#include <iostream>
#include <string>
#include "Unit.h"

Unit::Unit(std::string n, const int init_x,const int init_y,int owner)
        : name(std::move(n)), x(init_x), y(init_y) ,owner(owner){
    maxHp = 100;
    hp = 100;
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

std::string Unit::getName()const{ return name;}

int Unit::getHp() const { return hp; }

int Unit::getX() const { return x; }

int Unit::getY() const { return y; }

int Unit::getOwner() const { return owner; }

int Unit::getMaxHp() const { return maxHp; }