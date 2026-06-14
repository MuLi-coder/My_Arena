// unit.h
#ifndef MY_ARENA_UNIT_H
#define MY_ARENA_UNIT_H


#include <string>
#include <QString>
#include <random>

static int randomNum() {
    std::random_device rd;
    // 2. 初始化梅森旋转算法引擎
    std::mt19937 gen(rd());
    // 3. 定义分布范围：[1, 100] 的均匀整数分布
    std::uniform_int_distribution<int> dis(1, 100);
    // 4. 生成并输出随机数
    int random_num = dis(gen);
    return random_num;
}



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

    int x, y, pos;      //坐标
    int hp;             //生命值
    int maxHp;          //最大生命值
    int owner;          //所属
    QString image;
    int att;
    int attArea;
    int cost;
    Action action;

public:
    Unit(int owner = 0);
    virtual ~Unit() = default;



    // virtual void attack(Unit* target) = 0;
    void takeDamage(int dmg);
    void setX(int new_x);
    void setY(int new_y);
    void setHp(int new_hp);
    void resetAction();
    // 获取状态: 名字，生命,横纵坐标，图片路径


    int getHp()const;
    int getX() const;
    int getY() const;
    int getOwner() const;
    int getMaxHp() const;
    int getAtt() const;
    int getCost()const;
    QString getImage()const;

    //combat
    void searchNewTarget(const std::vector<std::vector<Unit*>>& grid,int row,int col,int r,int c);
    Action march(const std::vector<std::vector<Unit*>>& grid,int row,int col,int r,int c);

};
#endif //MY_ARENA_UNIT_H