//
// Created by ml on 2026/6/17.
//

#include "Guardian.h"

Guardian::Guardian(int owner, int level)
    :Unit(owner, level) {
    name = "Guardian";
    trait = "Warrior";
    image = "../images/Guardian.png";
    cost = 5;
    maxHp = 130;
    maxMana = 6;
    att = 6;
    attSpeed = 6;
    attArea = 1;
    moveSpeed = 6;
    //动态属性
    hp = maxHp;
    mana = maxMana;
    attCD = attSpeed;
    moveCD = moveSpeed;
}

void Guardian::selfRefresh() {
    putOnEquipment(equipment);
    cost = 5 + (level-1)*6;
    maxHp = 130 + (level-1)*30 + equipHpBuff + traitHpBuff;
    maxMana = 6 - (level-1) - equipManaBuff - traitManaBuff >=1 ? 6 - (level-1) - equipManaBuff - traitManaBuff : 1;
    att = 6 + (level-1)*3 + equipAttBuff + traitAttBuff;
    attSpeed = 6 - (level-1) - equipAttSpeedBuff - traitAttSpeedBuff >=1 ? 6 - (level-1) - equipAttSpeedBuff - traitAttSpeedBuff : 1;
    attArea = 1 + equipAttAreaBuff + traitAttAreaBuff;
    moveSpeed = 6 - (level-1) - equipMoveSpeedBuff - traitMoveSpeedBuff >=1 ? 6 - (level-1) - equipMoveSpeedBuff - traitMoveSpeedBuff : 1;
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

//技能：守护之盾，对目标造成少量伤害，同时回复自身30%最大生命值
void Guardian::skillAttack(const std::vector<std::vector<Unit *>> &grid, int row, int col, int r, int c) {
    grid[action.targetX][action.targetY]->takeDamage(att * 1.5);
    int heal = maxHp * 0.3;
    hp += heal;
    if (hp > maxHp) {
        hp = maxHp;
    }
}
