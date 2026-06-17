//
// Created by ml on 2026/6/17.
//

#include "Archer.h"

Archer::Archer(int owner, int level)
    :Unit(owner, level) {
    name = "Archer";
    trait = "Assassin";
    image = "../images/Archer.png";
    cost = 3;
    maxHp = 65;
    maxMana = 4;
    att = 15;
    attSpeed = 4;
    attArea = 3;
    moveSpeed = 4;
    //动态属性
    hp = maxHp;
    mana = maxMana;
    attCD = attSpeed;
    moveCD = moveSpeed;
}

void Archer::selfRefresh() {
    putOnEquipment(equipment);
    cost = 3 + (level-1)*4;
    maxHp = 65 + (level-1)*10 + equipHpBuff + traitHpBuff;
    maxMana = 4 - (level-1) - equipManaBuff - traitManaBuff >=1 ? 4 - (level-1) - equipManaBuff - traitManaBuff : 1;
    att = 15 + (level-1)*6 + equipAttBuff + traitAttBuff;
    attSpeed = 4 - (level-1) - equipAttSpeedBuff - traitAttSpeedBuff >=1 ? 4 - (level-1) - equipAttSpeedBuff - traitAttSpeedBuff : 1;
    attArea = 3 + equipAttAreaBuff + traitAttAreaBuff;
    moveSpeed = 4 - (level-1) - equipMoveSpeedBuff - traitMoveSpeedBuff >=1 ? 4 - (level-1) - equipMoveSpeedBuff - traitMoveSpeedBuff : 1;
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

//技能：穿刺箭，对目标造成1.8倍伤害，同时穿透到目标身后的一个敌人
void Archer::skillAttack(const std::vector<std::vector<Unit *>> &grid, int row, int col, int r, int c) {
    //先对主目标造成伤害
    int eX = action.targetX;
    int eY = action.targetY;
    grid[eX][eY]->takeDamage(att * 1.8);
    //计算穿透方向（从自身到目标的向量）
    int dx = eX - r;
    int dy = eY - c;
    //归一化为方向(-1,0,1)
    int dirX = (dx > 0) - (dx < 0);
    int dirY = (dy > 0) - (dy < 0);
    //沿方向穿透一格
    int pierceX = eX + dirX;
    int pierceY = eY + dirY;
    if (pierceX >= 0 && pierceX < row && pierceY >= 0 && pierceY < col) {
        if (grid[pierceX][pierceY] != nullptr && grid[pierceX][pierceY]->getOwner() != owner) {
            grid[pierceX][pierceY]->takeDamage(att * 1.2);
        }
    }
}
