//
// Created by ml on 2026/5/26.
//
#include "../Unit.h"
#include "Knight.h"
#include <QString>

Knight::Knight(int owner,int level)
    :Unit(owner,level) {
    name="Knight";
    trait = "Warrior";
    image = "../images/Knight.png";
    cost = 4;
    maxHp = 100;
    maxMana = 5 ;
    att = 8;
    attSpeed = 5;
    attArea =1 ;
    moveSpeed = 5;
    //动态属性初始化
    hp = maxHp;
    mana = maxMana;
    moveCD = moveSpeed;
    attCD = attSpeed;
}


void Knight::selfRefresh() {
    //备战状态根据等级和buff对属性进行重置
    putOnEquipment(equipment);
    cost  = 4+(level-1)*5;
    maxHp = 100+(level-1)*20 + equipHpBuff + traitHpBuff;
    maxMana = 5 - (level-1) - equipManaBuff - traitManaBuff >= 1 ?  5 - (level-1) - equipManaBuff - traitManaBuff:1;
    att = 8+(level-1)*5 + equipAttBuff + traitAttBuff;
    attSpeed = 5 - (level-1) - equipAttSpeedBuff - traitAttSpeedBuff >= 1 ? 5 - (level-1) - equipAttSpeedBuff - traitAttSpeedBuff : 1;
    attArea = 1 + equipAttAreaBuff + traitAttAreaBuff;
    moveSpeed = 5 - (level-1) - equipMoveSpeedBuff - traitMoveSpeedBuff >= 1 ? 5 - (level-1) - equipMoveSpeedBuff - traitMoveSpeedBuff : 1;
    //动态属性重置
    hp = maxHp;
    mana = maxMana;
    moveCD = moveSpeed;
    attCD = attSpeed;
    //战斗参数重置
    state = UnitState::Idle;
    action.move="Null";
    action.attack="Null";
    action.targetX=-1;
    action.targetY=-1;
    //羁绊标志
    traitFlag = false;
}

void Knight::skillAttack(const std::vector<std::vector<Unit *> > &grid, int row, int col, int r, int c) {
    int heavyAtt = att;
    int num = randomNum();
    if (num>70) {
        heavyAtt = att*2;
    }
    else if (num>30) {
        heavyAtt = att*1.7;
    }
    else {
        heavyAtt = att*1.4;
    }
    grid[action.targetX][action.targetY]->takeDamage(heavyAtt);
}

