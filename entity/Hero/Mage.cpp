//
// Created by ml on 2026/5/26.
//

#include "Mage.h"

Mage::Mage(int owner):Unit(owner) {
    hp=70;
    maxHp = 70;
    att=15;
    image = "../images/Mage.png";
    attArea = 2;
    cost=2;
}
