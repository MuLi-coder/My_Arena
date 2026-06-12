// unit.h
#ifndef MY_ARENA_UNIT_H
#define MY_ARENA_UNIT_H

#include <iostream>
#include <string>
#include <QString>

struct Action {
    std::string move;
    std::string attack;
    int targetX;
    int targetY;
};
//其中：
//move:  "Null","Up","Down","Left","Right"
//attack:"Null","CommonAtt","SkillAtt"

class Unit {
protected:
    std::string name;   //名字
    int x, y, pos;      //坐标
    int hp;             //生命值
    int maxHp;          //最大生命值
    int owner;          //所属
    int att;
    int attArea;
    Action action;

public:
    Unit(std::string n, int pos, int owner = 1);
    virtual ~Unit() = default;



    virtual void attack(Unit* target) = 0;
    void takeDamage(int dmg);
    void setX(int new_x);
    void setY(int new_y);
    void setHp(int new_hp);
    void resetAction();
    // 获取状态: 名字，生命,横纵坐标，图片路径

    std::string getName() const;
    int getHp()const;
    int getX() const;
    int getY() const;
    int getOwner() const;
    int getMaxHp() const;
    int getAtt() const;
    virtual QString getImage()const = 0;

    //combat
    void searchNewTarget(const std::vector<std::vector<Unit*>>& grid,int row,int col,int r,int c);
    Action march(const std::vector<std::vector<Unit*>>& grid,int row,int col,int r,int c);
};
#endif //MY_ARENA_UNIT_H