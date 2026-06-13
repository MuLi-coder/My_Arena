//
// Created by ml on 2026/5/26.
//

#include "Mage.h"

Mage::Mage(const std::string n,int owner):Unit(n,owner) {
    hp=70;
    maxHp = 70;
    att=15;
    image = "../images/Mage.png";
    attArea = 2;
}
