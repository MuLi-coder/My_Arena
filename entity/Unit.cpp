#include <iostream>
#include <string>
#include "Unit.h"



Unit::Unit(int owner)
        : owner(owner){
    hp = 10;
    x = 0;
    y = 0;
    action.move="Null";
    action.attack ="Null";
    action.targetX = -1;
    action.targetY = -1;
}




void Unit::takeDamage(const int dmg){
    hp -= dmg;
    if (hp < 0) hp = 0;
}

void Unit::setX(int new_x) { x = new_x; }
void Unit::setY(int new_y) { y = new_y; }

void Unit::setHp(int new_hp) { hp = new_hp; }

int Unit::getHp() const { return hp; }

int Unit::getX() const { return x; }

int Unit::getY() const { return y; }

int Unit::getOwner() const { return owner; }

int Unit::getMaxHp() const { return maxHp; }

int Unit::getAtt() const { return att; }

QString Unit::getImage()const{ return image;}

void Unit::resetAction() {
    action.move="Null";
    action.attack="Null";
    action.targetX = -1;
    action.targetY = -1;
}

int Unit::getCost() const {
    return cost;
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


void Unit::searchNewTarget(const std::vector<std::vector<Unit*>>& grid,const int row,const int col,const int r,const int c) {
    //遍历四周四个格子，找到距离最近的空位，定为最终目标
    std::cout<<"search MoveTarget"<<std::endl;
    int enemyX = action.targetX;
    int enemyY = action.targetY;
    int newTargetX = -1;
    int newTargetY = -1;
    int newDistance = 0;
    int minDistance = 100;
    //左
    newTargetX = enemyX;
    newTargetY = enemyY-attArea;
    if (isPositionValid(newTargetX,newTargetY,row,col)) {
        if (grid[newTargetX][newTargetY]==nullptr) {
            newDistance = turnPositive(newTargetX-r)+turnPositive(newTargetY-c);
            if (newDistance<=minDistance) {
                minDistance = newDistance;
                action.targetX = newTargetX;
                action.targetY = newTargetY;
            }
        }
    }
    //右
    newTargetX = enemyX;
    newTargetY = enemyY+attArea;
    if (isPositionValid(newTargetX,newTargetY,row,col)) {
        if (grid[newTargetX][newTargetY]==nullptr) {
            newDistance = turnPositive(newTargetX-r)+turnPositive(newTargetY-c);
            if (newDistance<=minDistance) {
                minDistance = newDistance;
                action.targetX = newTargetX;
                action.targetY = newTargetY;
            }
        }
    }
    //上
    newTargetX = enemyX-attArea;
    newTargetY = enemyY;
    if (isPositionValid(newTargetX,newTargetY,row,col)) {
        if (grid[newTargetX][newTargetY]==nullptr) {
            newDistance = turnPositive(newTargetX-r)+turnPositive(newTargetY-c);
            if (newDistance<=minDistance) {
                minDistance = newDistance;
                action.targetX = newTargetX;
                action.targetY = newTargetY;
            }
        }
    }
    //下
    newTargetX = enemyX+attArea;
    newTargetY = enemyY;
    if (isPositionValid(newTargetX,newTargetY,row,col)) {
        if (grid[newTargetX][newTargetY]==nullptr) {
            newDistance = turnPositive(newTargetX-r)+turnPositive(newTargetY-c);
            if (newDistance<=minDistance) {
                minDistance = newDistance;
                action.targetX = newTargetX;
                action.targetY = newTargetY;
            }
        }
    }

}

//寻路和攻击逻辑就在这里了
Action Unit::march(const std::vector<std::vector<Unit*>>& grid,const int row,const int col,const int r,const int c) {

    //首先是索敌逻辑
    bool isFind = false;
    int loop = 1;
    int target_x = -1;
    int target_y = -1;
    while (!isFind && loop < 15) {
        for (int k = 0; k <= loop; k++) {
            //上
            if (isPositionValid(r-loop,c-k,row,col)) {
                if (grid[r-loop][c-k]!=nullptr&&grid[r-loop][c-k]->getOwner()!=owner) {
                    isFind = true;
                    target_x = r-loop;
                    target_y = c-k;
                    break;
                }

            }
            if (isPositionValid(r-loop,c+k,row,col)) {
                if (grid[r-loop][c+k]!=nullptr&&grid[r-loop][c+k]->getOwner()!=owner) {
                    isFind = true;
                    target_x = r-loop;
                    target_y = c+k;
                    break;
                }
            }
            //下
            if (isPositionValid(r+loop,c-k,row,col)) {
                if (grid[r+loop][c-k]!=nullptr&&grid[r+loop][c-k]->getOwner()!=owner) {
                    isFind = true;
                    target_x = r+loop;
                    target_y = c-k;
                    break;
                }
            }
            if (isPositionValid(r+loop,c+k,row,col)) {
                if (grid[r+loop][c+k]!=nullptr&&grid[r+loop][c+k]->getOwner()!=owner) {
                    isFind = true;
                    target_x = r+loop;
                    target_y = c+k;
                    break;
                }
            }
            //左
            if (isPositionValid(r-k,c-loop,row,col)) {
                if (grid[r-k][c-loop]!=nullptr&&grid[r-k][c-loop]->getOwner()!=owner) {
                    isFind = true;
                    target_x = r-k;
                    target_y = c-loop;
                    break;
                }
            }
            if (isPositionValid(r+k,c-loop,row,col)) {
                if (grid[r+k][c-loop]!=nullptr&&grid[r+k][c-loop]->getOwner()!=owner) {
                    isFind = true;
                    target_x = r+k;
                    target_y = c-loop;
                    break;
                }
            }
            //右
            if (isPositionValid(r+k,c+loop,row,col)) {
                if (grid[r+k][c+loop]!=nullptr&&grid[r+k][c+loop]->getOwner()!=owner) {
                    isFind = true;
                    target_x = r+k;
                    target_y = c+loop;
                    break;
                }
            }
            if (isPositionValid(r-k,c+loop,row,col)) {
                if (grid[r-k][c+loop]!=nullptr&&grid[r-k][c+loop]->getOwner()!=owner) {
                    isFind = true;
                    target_x = r-k;
                    target_y = c+loop;
                    break;
                }
            }
        }
        loop +=1 ;
    }
    //确定最近敌人的位置：target_x,target_y
    //先直接写到状态中
    action.targetX = target_x;
    action.targetY = target_y;

    //下面进行战斗决策，移动还是攻击，根据距离判断
    int distance = turnPositive(action.targetX-r)+turnPositive(action.targetY-c) ;
    if (distance<=attArea) {
        //距离合适，返回攻击信号和攻击目标
        action.attack = "CommonAtt";
        return action;
    }

    else{
        //距离太远，返回移动信号和移动方向
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
        return action;
    }
}