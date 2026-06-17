//
// Created by ml on 2026/6/17.
//

#include "Warlock.h"

Warlock::Warlock(int owner, int level)
    :Unit(owner, level) {
    name = "Warlock";
    trait = "Mage";
    image = "../images/Warlock.png";
    cost = 4;
    maxHp = 80;
    maxMana = 4;
    att = 12;
    attSpeed = 5;
    attArea = 2;
    moveSpeed = 5;
    //动态属性
    hp = maxHp;
    mana = maxMana;
    attCD = attSpeed;
    moveCD = moveSpeed;
}

void Warlock::selfRefresh() {
    putOnEquipment(equipment);
    cost = 4 + (level-1)*5;
    maxHp = 80 + (level-1)*15 + equipHpBuff + traitHpBuff;
    maxMana = 4 - (level-1) - equipManaBuff - traitManaBuff >=1 ? 4 - (level-1) - equipManaBuff - traitManaBuff : 1;
    att = 12 + (level-1)*5 + equipAttBuff + traitAttBuff;
    attSpeed = 5 - (level-1) - equipAttSpeedBuff - traitAttSpeedBuff >=1 ? 5 - (level-1) - equipAttSpeedBuff - traitAttSpeedBuff : 1;
    attArea = 2 + equipAttAreaBuff + traitAttAreaBuff;
    moveSpeed = 5 - (level-1) - equipMoveSpeedBuff - traitMoveSpeedBuff >=1 ? 5 - (level-1) - equipMoveSpeedBuff - traitMoveSpeedBuff : 1;
    //动态属性重置
    hp = maxHp;
    mana = maxMana;
    moveCD = moveSpeed;
    attCD = attSpeed;
    //战斗参数重置
    state = UnitState::Idle;
    action.move = "Null";
    action.attack = "Null";
    action.targetX = -1;
    action.targetY = -1;
}

//技能：生命汲取，对目标造成1.5倍伤害，并将伤害的50%转化为自身生命回复
void Warlock::skillAttack(const std::vector<std::vector<Unit *>> &grid, int row, int col, int r, int c) {
    int dmg = att * 1.5;
    grid[action.targetX][action.targetY]->takeDamage(dmg);
    int heal = dmg * 0.5;
    hp += heal;
    if (hp > maxHp) {
        hp = maxHp;
    }
}
