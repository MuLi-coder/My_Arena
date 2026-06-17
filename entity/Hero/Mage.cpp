//
// Created by ml on 2026/5/26.
//

#include "Mage.h"


Mage::Mage(int owner,int level)
    :Unit(owner,level) {
    name="Mage";
    image = "../images/Mage.png";
    cost = 6;
    maxHp = 70;
    maxMana = 5;
    att=20;
    attSpeed = 7;
    attArea = 2;
    moveSpeed = 5;
    //动态属性
    hp = maxHp;
    mana = maxMana;
    attCD = attSpeed;
    moveCD = moveSpeed;
}


void Mage::selfRefresh() {
    putOnEquipment(equipment);
    cost = 6 + (level-1)*8;
    maxHp = 70 + (level-1)*10 + hpBuff;
    maxMana = 5 - (level-1) - manaBuff >=1 ? 5 - (level-1) - manaBuff : 1;
    att= 15 + (level-1)*8 + attBuff;
    attSpeed = 7 - (level-1) - attSpeedBuff >=1 ? 7 - (level-1) - attSpeedBuff : 1;
    attArea = 2 + attAreaBuff;
    moveSpeed = 5-(level-1)-moveSpeedBuff>=1 ? 5-(level-1)-moveSpeedBuff : 1;
    //动态属性重置
    hp = maxHp;
    mana = maxMana;
    moveCD = moveSpeed;
    attCD = attSpeed;
    //战斗参数重置
    state=UnitState::Idle;
    action.move= "Null";
    action.attack="Null";
    action.targetX = -1;
    action.targetY = -1;
}

void Mage::skillAttack(const std::vector<std::vector<Unit *> > &grid, int row, int col, int r, int c) {
    for (int x = action.targetX-1; x <= action.targetX+1; ++x) {
        for (int y = action.targetY-1; y <= action.targetY+1; ++y) {
            if (x >= 0 && x < row && y >= 0 && y < col) {
                if (grid[x][y]!=nullptr && grid[x][y]->getOwner()!=owner) {
                    grid[x][y]->takeDamage(att*1.2);
                }
            }
        }
    }
}

