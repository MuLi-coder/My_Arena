#include <iostream>
#include <string>
#include "Unit.h"

Unit::Unit(const int owner,const int level)
        : owner(owner),level(level){
    action.move="Null";
    action.attack ="Null";
    action.targetX = -1;
    action.targetY = -1;
    state = UnitState::Idle;
    //装备buff初始化
    equipment = nullptr;
    hpBuff = 0;
    attBuff = 0;
    attSpeedBuff = 0;
    attAreaBuff = 0;
    moveSpeedBuff = 0;
    manaBuff = 0;
}
QString Unit::getName()const {
    return name;
}
QString Unit::getTrait()const {
    return trait;
}
QString Unit::getImage()const {
    return image;
}
int Unit::getOwner() const {
    return owner;
}
int Unit::getLevel() const {
    return level;
}
void Unit::changeLevel(int num) {
    level+=num;
    if (level<=0) {
        level = 1;
    }
    if (level>3) {
        level = 3;
    }
}
int Unit::getCost() const {
    return cost;
}
int Unit::getHp() const {
    return hp;
}
void Unit::setHp(int newHp) {
    hp = newHp;
}
int Unit::getMaxHp() const {
    return maxHp;
}
int Unit::getMana()const {
    return mana;
}
int Unit::getMaxMana() const {
    return maxMana;
}
int Unit::getAtt() const {
    return att;
}
int Unit::getAttSpeed() const {
    return attSpeed;
}
int Unit::getAttArea() const {
    return attArea;
}
int Unit::getMoveSpeed()const {
    return moveSpeed;
}
UnitState Unit::getState() const {
    return state;
}
//装备
void Unit::putOnEquipment(Equipment* equip) {
    equipment = equip;
    if (equip == nullptr ) {
        hpBuff = 0;
        attBuff = 0;
        attSpeedBuff=0;
        attAreaBuff=0;
        moveSpeedBuff=0;
        manaBuff=0;
    }else {
        hpBuff = equip->getHpBuff();
        attBuff = equip->getAttBuff();
        attSpeedBuff = equip->getAttSpeedBuff();
        attAreaBuff = equip->getAttAreaBuff();
        moveSpeedBuff = equip->getMoveSpeedBuff();
        manaBuff = equip->getManaBuff();
    }
}

Equipment* Unit::takeOffEquipment(bool hasCell) {
    if (hasCell) {
        Equipment* equip = equipment;
        equipment = nullptr;
        selfRefresh();
        return equip;
    }
    return nullptr;
}

bool Unit::isWearingEquipment() {
    if (equipment != nullptr) {
        return true;
    }
    return false;
}

//buff change
void Unit::changeHpBuff(int num) {
    hpBuff+=num;
    if (hpBuff<0) {
        hpBuff = 0;
    }
}

void Unit::changeAttBuff(int num) {
    attBuff+=num;
    if (attBuff<0) {
        attBuff = 0;
    }
}

void Unit::changeAttSpeedBuff(int num) {
    attSpeedBuff+=num;
    if (attSpeedBuff<0) {
        attSpeedBuff = 0;
    }
}

void Unit::changeAttAreaBuff(int num) {
    attAreaBuff+=num;
    if (attAreaBuff<0) {
        attAreaBuff = 0;
    }
}
void Unit::changeMoveSpeedBuff(int num) {
    moveSpeedBuff+=num;
    if (moveSpeedBuff<0) {
        moveSpeedBuff = 0;
    }
}
void Unit::changeManaBuff(int num) {
    manaBuff+=num;
    if (manaBuff<0) {
        manaBuff = 0;
    }
}

//combat
void Unit::takeDamage(const int dmg){
    hp -= dmg;
    if (hp < 0) hp = 0;
}
void Unit::resetAction() {
    action.move="Null";
    action.attack="Null";
    action.targetX = -1;
    action.targetY = -1;
}

void Unit::changeStateTo(UnitState newState) {
    state = newState;
}

//辅助函数1：验证坐标合法
bool isPositionValid(int x,int y,int row,int col) {
    if (x>=0&&x<row&&y>=0&&y<col) {
        return true;
    }
    return false;
}
//辅助函数2：绝对值转换
int turnPositive(int num) {
    return num>=0?num:-num;
}
//辅助函数3：曼哈顿距离
int getDistance(const int x1,const int y1,const int x2,const int y2) {
    return turnPositive(x1-x2)+turnPositive(y1-y2);
}

void Unit::searchEnemy(const std::vector<std::vector<Unit *>> &grid, int row, int col, int r, int c) {
    //首先是索敌逻辑
    bool isFind = false;
    int area = 1;
    while (!isFind && area < 16) {
        for (int k = 0; k < area; k++) {
            //按照曼哈顿距离，四向搜寻
            //左-右下
            if (isPositionValid(r+k,c-area+k,row,col)) {
                if (grid[r+k][c-area+k]!=nullptr && grid[r+k][c-area+k]->getOwner()!=owner) {
                    isFind=true;
                    action.targetX = r+k;
                    action.targetY = c-area+k;
                    break;
                }
            }
            //下-右上
            if (isPositionValid(r+area-k,c+k,row,col)) {
                if (grid[r+area-k][c+k]!=nullptr && grid[r+area-k][c+k]->getOwner()!=owner) {
                    isFind=true;
                    action.targetX = r+area-k;
                    action.targetY = c+k;
                    break;
                }
            }
            //右-左上
            if (isPositionValid(r-k,c+area-k,row,col)) {
                if (grid[r-k][c+area-k]!=nullptr && grid[r-k][c+area-k]->getOwner()!=owner) {
                    isFind=true;
                    action.targetX = r-k;
                    action.targetY = c+area-k;
                    break;
                }
            }
            //上-左下
            if (isPositionValid(r-area+k,c-k,row,col)) {
                if (grid[r-area+k][c-k]!=nullptr && grid[r-area+k][c-k]->getOwner()!=owner) {
                    isFind=true;
                    action.targetX = r-area+k;
                    action.targetY = c-k;
                    break;
                }
            }
        }
        area +=1 ;
    }
}

void Unit::searchNewTarget(const std::vector<std::vector<Unit*>>& grid,const int row,const int col,const int r,const int c) {
    //遍历四周，找到攻击范围内最近的空位，定为最终目标
    int k = attArea;
    int eX = action.targetX;
    int eY = action.targetY;
    int minDistance = 50;
    while (k!=0) {
        //进行四向遍历
        for (int s = 0; s < k; s++) {
            //左-右下
            if (isPositionValid(eX+s,eY-k+s,row,col)) {
                if (grid[eX+s][eY-k+s]==nullptr && getDistance(eX+s,eY-k+s,r,c)<=minDistance) {
                    action.targetX = eX+s;
                    action.targetY = eY-k+s;
                    minDistance = getDistance(eX+s,eY-k+s,r,c);
                }
            }
            //下-右上
            if (isPositionValid(eX+k-s,eY+s,row,col)) {
                if (grid[eX+k-s][eY+s]==nullptr && getDistance(eX+k-s,eY+s,r,c)<=minDistance) {
                    action.targetX = eX+k-s;
                    action.targetY = eY+s;
                    minDistance = getDistance(eX+k-s,eY+s,r,c);
                }
            }
            //右-左上
            if (isPositionValid(eX-s,eY+k-s,row,col)) {
                if (grid[eX-s][eY+k-s]==nullptr && getDistance(eX-s,eY+k-s,r,c)<=minDistance) {
                    action.targetX = eX-s;
                    action.targetY = eY+k-s;
                    minDistance = getDistance(eX-s,eY+k-s,r,c);
                }
            }
            //上-左下
            if (isPositionValid(eX-k+s,eY-s,row,col)) {
                if (grid[eX-k+s][eY-s]==nullptr && getDistance(eX-k+s,eY-s,r,c)<=minDistance) {
                    action.targetX = eX-k+s;
                    action.targetY = eY-s;
                    minDistance = getDistance(eX-k+s,eY-s,r,c);
                }
            }
        }
        k-=1;
    }
}

//寻路和攻击逻辑就在这里了
Action Unit::march(const std::vector<std::vector<Unit*>>& grid,const int row,const int col,const int r,const int c) {
    //索敌，确定target坐标
    std::cout<<"search"<<std::endl;
    searchEnemy(grid,row,col,r,c);
    std::cout <<"getTarget"<<std::endl;
    //下面进行战斗决策，移动还是攻击，根据距离，攻速，移速判断
    int distance = turnPositive(action.targetX-r)+turnPositive(action.targetY-c) ;
    if (distance<=attArea) {
        //如果达到攻击条件，攻击（普攻或技能），在这里实现多态，CD清零；否则没有攻击行动，保持不动，CD自增
        if (attCD==attSpeed) {
            if (mana == maxMana) {
                action.attack = "SkillAtt";
                skillAttack(grid,row,col,r,c);
                mana = 0;
            }else {
                action.attack = "CommonAtt";
                grid[action.targetX][action.targetY]->takeDamage(att);
                mana+=1;
            }
            attCD = 0;
        }else {
            action.attack = "Null";
            attCD+=1;
        }
    }
    else{
        //满足移动条件则移动，否则不动
        if (moveCD==moveSpeed) {
            //注意选择移动的时候要注意，目标已经不是敌人本身了，应该是敌人四周的空位
            searchNewTarget(grid,row,col,r,c);
            //确定移动方向
            int dx = action.targetX-r;
            int dy = action.targetY-c;
            int Dx = turnPositive(dx);
            int Dy = turnPositive(dy);
            //总共八种可能
            //先确定直接的四个
            if (dx==0) {
                action.move = dy>0 ? "Right":"Left";
            }
            if (dy==0) {
                action.move = dx>0 ? "Down":"Up";
            }
            //再确定复杂的四个
            if (dx>0&&dy>0) {
                action.move = Dx<=Dy?"Down":"Right";
            }
            if (dx<0&&dy>0) {
                action.move = Dx<=Dy?"Up":"Right";
            }
            if (dx<0&&dy<0) {
                action.move = Dx<=Dy?"Up":"Left";
            }
            if (dx>0&&dy<0) {
                action.move = Dx<=Dy?"Down":"Left";
            }
            //行动完，CD清零
            moveCD=0;
        }else {
            action.move = "Null";
            moveCD+=1;
        }
    }
    return action;
}