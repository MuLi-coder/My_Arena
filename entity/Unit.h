// unit.h
#ifndef MY_ARENA_UNIT_H
#define MY_ARENA_UNIT_H

#include <iostream>
#include <string>
#include <QString>

class Unit {
protected:
    //-----四个信息-----//
    std::string name;   //名字
    int x, y, pos;           //坐标
    int hp;             //生命值
    int maxHp;          //最大生命值
    int owner;          //所属

public:
    // 构造函数
    Unit(std::string n, int pos, int owner = 1);
    virtual ~Unit() = default;


    // 移动
    void move(int new_x,int new_y);
    // --攻击
    virtual void attack(Unit* target) = 0;
    // 受到伤害
    void takeDamage(int dmg);
    //设置坐标
    void setX(int new_x);
    void setY(int new_y);
    // 获取状态: 名字，生命,横纵坐标，图片路径
    std::string getName() const;
    int getHp()const;
    int getX() const;
    int getY() const;
    int getOwner() const;
    int getMaxHp() const;
    virtual QString getImage()const = 0;

    //combat
    void march();
};
#endif //MY_ARENA_UNIT_H