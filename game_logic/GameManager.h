//
// Created by ml on 2026/5/30.
//

#ifndef MY_ARENA_GAMEMANAGER_H
#define MY_ARENA_GAMEMANAGER_H

#include <memory>
#include <QTimer>
#include <QObject>

#include "ComBoard.h"
#include "PreBoard.h"
#include "Player.h"
#include "Shop.h"

#include "../entity/Hero/Knight.h"
#include "../entity/Hero/Mage.h"
#include "../entity/Hero/Archer.h"
#include "../entity/Hero/Guardian.h"
#include "../entity/Hero/Assassin.h"
#include "../entity/Hero/Warlock.h"

enum class GameLevel {
    ONE,
    TWO,
    THREE,
    FOUR
};

enum class GameState {
    Ready,
    Prepare,
    Combat,
    Resolve
};

enum class ComResult {
    Success,
    Failure,
    Draw
};

struct Position {
    int x;
    int y;
};

class GameManager : public QObject{
    Q_OBJECT

    //拥有一个计时器
    QTimer timer;
    //拥有当前状态，控制游戏核心
    GameState currentState;
    //拥有战斗结果
    ComResult result;
    //拥有关卡进度
    GameLevel currentLevel;
    //拥有棋盘数据
    std::unique_ptr<PreBoard> preBoard;
    std::unique_ptr<ComBoard> comBoard;
    //拥有玩家数据
    std::unique_ptr<Player> player;
    //拥有商店数据
    std::unique_ptr<Shop> shop;
    //拥有装备库
    std::vector<Equipment*> equipment;


    //状态对应的处理函数
    void handleReady();
    void handlePrepare();
    void handleCombat();
    void handleResolve();
    //羁绊检测
    void applyTraitBuffs();

public:
    GameManager(int r=8, int c=8,int pos=8);
    //游戏流程
    GameState getCurrentState() const;
    void changeStateTo(GameState newGameState);
    bool isTimerActive() const;
    void timerStart();
    void timerStop();
    bool isComEnd();
    void resetTheGame();
    int selfUnitCount() const;
    void setEnemy();
    void setEnemyRandomly();
    void goToNextLevel();
    void clearEnemyGrid();
    //棋盘状态
    int getRow() const;
    int getCol() const;
    int getBen() const;
    Unit* getUnitAtGrid(int r,int c) const;
    Unit* getUnitAtBench(int pos) const;
    bool isCellEmptyGrid(int r, int c) const;
    bool isCellEmptyBench(int pos) const;
    void removeUnitAtGrid(int r, int c);
    void removeUnitAtBench(int pos);
    void placeUnitAtGrid(int r, int c,Unit* unit) ;
    void placeUnitAtBench(int pos,Unit* unit) ;

    //装备库
    Equipment* getEquipmentAt(int s) const;
    bool isEquipmentEmpty(int s) const;
    void placeEquipmentAt(int s,Equipment* equipment);
    void removeEquipmentAt(int s);
    //SHOP
    bool isCellEmptyShop(int s)const;
    Unit* getUnitAtShop(int s)const;
    void placeUnitAtShop(int s,Unit* unit);
    void removeUnitAtShop(int s);
    void refreshShop();
    void autoMergeToBench(int k);

    //PLAYER
    int getPlayerMoney() const;
    int getPlayerLevel() const;
    int getPlayerShopRefreshTimes() const;
    int getMaxUnit() const;
    void levelUp();
    void changePlayerMoney(int num);
    int getPlayerScore() const;
    void changeShopRefreshTimes(int num);
    Player* getPlayer() const;

    //存档系统
    void saveGame(const QString& path);
    void loadGame(const QString& path);

    //向外发送的信号参数
    signals:
    void shouldUpdate();
    void enterResolve(ComResult result,int hp);
    void failToCombat();
};


#endif //MY_ARENA_GAMEMANAGER_H