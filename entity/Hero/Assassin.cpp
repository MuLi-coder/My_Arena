//
// Created by ml on 2026/6/17.
//

#include "Assassin.h"

Assassin::Assassin(int owner, int level)
    :Unit(owner, level) {
    name = "Assassin";
    trait = "Assassin";
    image = "../images/Assassin.png";
    cost = 5;
    maxHp = 60;
    maxMana = 3;
    att = 18;
    attSpeed = 3;
    attArea = 1;
    moveSpeed = 3;
    //动态属性
    hp = maxHp;
    mana = maxMana;
    attCD = attSpeed;
    moveCD = moveSpeed;
}

void Assassin::selfRefresh() {
    putOnEquipment(equipment);
    cost = 5 + (level-1)*7;
    maxHp = 60 + (level-1)*10 + equipHpBuff + traitHpBuff;
    maxMana = 3 - (level-1) - equipManaBuff - traitManaBuff >=1 ? 3 - (level-1) - equipManaBuff - traitManaBuff : 1;
    att = 18 + (level-1)*8 + equipAttBuff + traitAttBuff;
    attSpeed = 3 - (level-1) - equipAttSpeedBuff - traitAttSpeedBuff >=1 ? 3 - (level-1) - equipAttSpeedBuff - traitAttSpeedBuff : 1;
    attArea = 1 + equipAttAreaBuff + traitAttAreaBuff;
    moveSpeed = 3 - (level-1) - equipMoveSpeedBuff - traitMoveSpeedBuff >=1 ? 3 - (level-1) - equipMoveSpeedBuff - traitMoveSpeedBuff : 1;
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
    //
    traitFlag = false;
}

//技能：暗影突袭，对目标造成2.5倍暴击伤害，50%概率闪避本次反击（通过回复等量伤害模拟）
void Assassin::skillAttack(const std::vector<std::vector<Unit *>> &grid, int row, int col, int r, int c) {
    int critDmg = att * 2.5;
    grid[action.targetX][action.targetY]->takeDamage(critDmg);
    //50%概率闪避：回复等量于自身最大生命20%的血量
    int num = randomNum();
    if (num > 50) {
        int dodge = maxHp * 0.2;
        hp += dodge;
        if (hp > maxHp) {
            hp = maxHp;
        }
    }
}
