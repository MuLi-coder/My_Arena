// unit.h
#ifndef MY_ARENA_UNIT_H
#define MY_ARENA_UNIT_H


#include <string>
#include <QString>
#include <random>

#include "Equipment.h"

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

enum class UnitState {
    Idle,
    Done,
    Moving,
    Attacking
};

class Unit {
protected:
    QString name;
    QString trait;
    QString image;
    int owner;  //
    int level;  //

    //固定属性，受等级和buff控制
    int cost;
    int maxHp;
    int maxMana;
    int att;
    int attSpeed;
    int moveSpeed;
    int attArea;
    //主动装备及 buff 加成     //
    Equipment* equipment;
    int equipHpBuff;
    int equipAttBuff;
    int equipAttSpeedBuff;
    int equipAttAreaBuff;
    int equipMoveSpeedBuff;
    int equipManaBuff;
    //羁绊 buff 加成
    int traitHpBuff;
    int traitAttBuff;
    int traitAttSpeedBuff;
    int traitAttAreaBuff;
    int traitMoveSpeedBuff;
    int traitManaBuff;
    //动态属性，战斗中不断变化
    int hp;
    int mana;
    int moveCD;
    int attCD;
    UnitState state;    //
    Action action;      //

public:
    Unit(int owner = 0,int level = 1);
    virtual ~Unit() = default;
    //参数的获取和更改
    QString getName()const;
    QString getTrait()const;
    QString getImage()const;
    int getOwner() const;
    int getLevel()const;
    void changeLevel(int num);
    int getCost()const;
    int getHp()const;
    void setHp(int newHp);
    int getMaxHp() const;
    int getMana()const;
    int getMaxMana()const;
    int getAtt() const;
    int getAttSpeed()const;
    int getAttArea()const;
    int getMoveSpeed()const;
    UnitState getState()const;
    //数据自更新
    virtual void selfRefresh() = 0;
    //穿装备
    void putOnEquipment(Equipment* equip);
    Equipment* takeOffEquipment(bool hasCell);
    bool isWearingEquipment();
    //change Buff
    void changeHpBuff(int num);
    void changeAttBuff(int num);
    void changeAttSpeedBuff(int num);
    void changeAttAreaBuff(int num);
    void changeMoveSpeedBuff(int num);
    void changeManaBuff(int num);
    //羁绊 buff
    void applyTraitBuffs(int hp, int att, int attSpd, int attArea, int moveSpd, int mana);
    void clearTraitBuffs();
    //combat
    void takeDamage(int dmg);
    void resetAction();
    void changeStateTo(UnitState newState);
    void searchNewTarget(const std::vector<std::vector<Unit*>>& grid,int row,int col,int r,int c);
    void searchEnemy(const std::vector<std::vector<Unit*>>& grid,int row,int col,int r,int c);
    virtual void skillAttack(const std::vector<std::vector<Unit*>>& grid,int row,int col,int r,int c) = 0;
    Action march(const std::vector<std::vector<Unit*>>& grid,int row,int col,int r,int c);

};
#endif //MY_ARENA_UNIT_H