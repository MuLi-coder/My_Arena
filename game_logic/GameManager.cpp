//
// Created by ml on 2026/5/30.
//

#include "GameManager.h"
#include <iostream>
#include <QMessageBox>
#include <QFile>

#include "../entity/Equipment/Sword.h"
#include "../entity/Equipment/ChainMail.h"
#include "../entity/Equipment/RapidGloves.h"
#include "../entity/Equipment/BlueCrystal.h"

// void sleepMs(int msec)
// {
//     QEventLoop loop;
//     QTimer::singleShot(msec, &loop, &QEventLoop::quit);
//     loop.exec();
// }


GameManager::GameManager(const int r,const int c, const int pos) {
    preBoard = std::make_unique<PreBoard>(r,c,pos);
    comBoard = std::make_unique<ComBoard>(*preBoard);
    player = std::make_unique<Player>();
    shop = std::make_unique<Shop>();
    equipment.resize(5,nullptr);
    currentState = GameState::Ready;
    currentLevel = GameLevel::ONE;
    result = ComResult::Draw;
    //初始化计时器，设置间隔和槽函数
    timer.setInterval(100);
    connect(&timer, &QTimer::timeout, this, &GameManager::handleCombat);
}
//----------棋盘状态--------------//
int GameManager::getRow() const {
    return preBoard->getRow();
}
int GameManager::getCol() const {
    return preBoard->getCol();
}
int GameManager::getBen() const {
    return preBoard->getBen();
}
Unit* GameManager::getUnitAtGrid(const int r,const int c) const {
    if (currentState==GameState::Combat) {
        return comBoard->getUnitAt(r,c);
    }else {
        return preBoard->getUnitAt(r,c);
    }
}
Unit* GameManager::getUnitAtBench(const int pos) const {
    return preBoard->getUnitAt(pos);
}
bool GameManager::isCellEmptyGrid(int r, int c) const {
    if (currentState==GameState::Combat) {
        return comBoard->isCellEmpty(r,c);
    }else {
        return preBoard->isCellEmpty(r,c);
    }
}
bool GameManager::isCellEmptyBench(int pos) const {
    return preBoard->isCellEmpty(pos);
}
void GameManager::removeUnitAtGrid(const int r,const int c) {
    preBoard->removeUnitAt(r,c);
}
void GameManager::removeUnitAtBench(const int pos) {
    preBoard->removeUnitAt(pos);
}
void GameManager::placeUnitAtGrid(const int r,const int c,Unit* unit) {
    if (preBoard->isCellEmpty(r,c)) {
        preBoard->placeUnitAt(r,c,unit);
    }
}
void GameManager::placeUnitAtBench(const int pos,Unit* unit) {
    if (preBoard->isCellEmpty(pos)) {
        preBoard->placeUnitAt(pos,unit);
    }
}

//装备库
Equipment* GameManager::getEquipmentAt(int s) const{
    if (s<0||s>=5){return nullptr;}
    return equipment[s];
}
bool GameManager::isEquipmentEmpty(int s) const {
    if (s>=0&&s<5&&equipment[s]==nullptr) {
        return true;
    }
    return false;
}

void GameManager::placeEquipmentAt(int s, Equipment *equip) {
    if (s>=0&&s<5) {
        equipment[s] = equip;
    }
}

void GameManager::removeEquipmentAt(int s) {
    if (s>=0&&s<5) {
        equipment[s] = nullptr;
    }
}

//-------------Shop-------------//
Unit* GameManager::getUnitAtShop(const int s) const {
    return shop->getUnitAt(s);
}
bool GameManager::isCellEmptyShop(const int s) const {
    return shop->isCellEmpty(s);
}
void GameManager::removeUnitAtShop(int s) {
    shop->removeUnitAt(s);
}
void GameManager::placeUnitAtShop(int s, Unit *unit) {
    shop->placeUnitAt(s,unit);
}
void GameManager::refreshShop() {
    //先重置
    for (int t=0;t<5;++t) {
        placeUnitAtShop(t,nullptr);
    }
    for (int t=0;t<5;++t) {
        int ratio = randomNum();
        if (ratio<=16) {
            placeUnitAtShop(t,new Knight());
        }else if (ratio<=33) {
            placeUnitAtShop(t,new Mage());
        }else if (ratio<=50) {
            placeUnitAtShop(t,new Archer());
        }else if (ratio<=66) {
            placeUnitAtShop(t,new Guardian());
        }else if (ratio<=83) {
            placeUnitAtShop(t,new Assassin());
        }else {
            placeUnitAtShop(t,new Warlock());
        }
    }
}

void GameManager::autoMergeToBench(const int k) {
    bool isEnd= false;
    while (!isEnd) {
        //首先遍历备战区，然后遍历棋盘，统计到三个之后自动合并
        int count = 0;
        Position arr[2];
        Unit* unit = getUnitAtBench(k);
        bool isReady = false;

        for (int p=0;p<preBoard->getBen();++p) {
            Unit* item = getUnitAtBench(p);
            if (item!=nullptr&&item->getOwner()==0&&item!=unit) {
                if (item->getName()==unit->getName()&&item->getLevel()==unit->getLevel()&&unit->getLevel()!=3) {
                    Position pos;
                    pos.x = p;
                    pos.y = -1;
                    arr[count] = pos;
                    count++;
                    if (count==2) {
                        isReady=true;
                        break;
                    }
                }
            }
        }

        //下面来遍历我方半场，寻找合并者
        if (!isReady) {
            for (int r=preBoard->getRow()/2;r<preBoard->getRow();++r) {
                for (int c = 0;c<preBoard->getCol();++c) {
                    Unit* item = getUnitAtGrid(r,c);
                    if (item!=nullptr&&item->getOwner()==0&&item!=unit) {
                        if (item->getName()==unit->getName()&&item->getLevel()==unit->getLevel()&&unit->getLevel()!=3) {
                            Position pos;
                            pos.x = r;
                            pos.y = c;
                            arr[count] = pos;
                            count++;
                            if (count==2) {
                                isReady=true;
                                break;
                            }
                        }
                    }
                }
                if (isReady) {break;}
            }
        }

        //合并前处理装备：被合并的棋子如果有装备，退回到装备区
        if (isReady) {
            for (int s = 0; s < 2; ++s) {
                Unit* mergeUnit = nullptr;
                if (arr[s].y == -1) {
                    mergeUnit = getUnitAtBench(arr[s].x);
                } else {
                    mergeUnit = getUnitAtGrid(arr[s].x, arr[s].y);
                }
                if (mergeUnit != nullptr && mergeUnit->isWearingEquipment()) {
                    //找装备区空位
                    for (int k = 0; k < 5; ++k) {
                        if (isEquipmentEmpty(k)) {
                            Equipment* equip = mergeUnit->takeOffEquipment(true);
                            placeEquipmentAt(k, equip);
                            break;
                        }
                    }
                }
            }
        }

        //当可以自动合并时
        if (isReady) {
            for (int s = 0;s<2;++s) {
                if (arr[s].y ==-1) {
                    removeUnitAtBench(arr[s].x);
                }else {
                    removeUnitAtGrid(arr[s].x,arr[s].y);
                }
            }
            unit->changeLevel(+1);
            unit->selfRefresh();
        }
        isEnd = !isReady;
    }

}

//------------Player--------------//
int GameManager::getPlayerScore() const {
    return player->getScore();
}
int GameManager::getPlayerLevel() const {
    return player->getLevel();
}
Player* GameManager::getPlayer() const {
    return player.get();
}
int GameManager::getPlayerMoney() const {
    return player->getMoney();
}
void GameManager::changePlayerMoney(int num) {
    player->changeMoney(num);
}
int GameManager::getPlayerShopRefreshTimes() const {
    return player->getShopRefreshTimes();
}
void GameManager::levelUp() {
    player->changeLevel(1);
}
int GameManager::getMaxUnit() const {
    return player->getMaxUnits();
}
void GameManager::changeShopRefreshTimes(const int num) {
    player->changeShopRefreshTimes(num);
}

//-------------游戏流程--------------//
GameState GameManager::getCurrentState() const {
    return currentState;
}
int GameManager::selfUnitCount()const {
    int count = 0;
    for (int r=preBoard->getRow()/2;r<preBoard->getRow();++r) {
        for (int c =0 ; c <preBoard->getCol();++c) {
            if (!isCellEmptyGrid(r,c)) {
                if (preBoard->getUnitAt(r,c)->getOwner()==0){count++;}
            }
        }
    }
    return count;
}
void GameManager::setEnemy() {
    switch (currentLevel) {
        case GameLevel::ONE:
            clearEnemyGrid();
            placeUnitAtGrid(2,4,new Knight(1));
            break;
        case GameLevel::TWO:
            clearEnemyGrid();
            placeUnitAtGrid(2,3,new Knight(1));
            placeUnitAtGrid(2,2,new Knight(1));
            break;
        case GameLevel::THREE:
            clearEnemyGrid();
            placeUnitAtGrid(2,3,new Knight(1));
            placeUnitAtGrid(1,3,new Mage(1));
            break;
        case GameLevel::FOUR:
            if (result==ComResult::Success) {
                clearEnemyGrid();
                setEnemyRandomly();
            }
            break;
    }
}
//辅助：创建一个随机敌方英雄，等级根据难度浮动
static Unit* createRandomEnemy(int difficulty) {
    //随机选择英雄类型（6种等概率）
    int heroRoll = randomNum();
    Unit* unit = nullptr;
    if (heroRoll <= 16) {
        unit = new Knight(1);
    } else if (heroRoll <= 33) {
        unit = new Mage(1);
    } else if (heroRoll <= 50) {
        unit = new Archer(1);
    } else if (heroRoll <= 66) {
        unit = new Guardian(1);
    } else if (heroRoll <= 83) {
        unit = new Assassin(1);
    } else {
        unit = new Warlock(1);
    }

    //根据难度决定等级
    int levelRoll = randomNum();
    int targetLevel = 1;
    if (difficulty == 1) {
        //低难度：90% 1级，10% 2级
        if (levelRoll > 90) targetLevel = 2;
    } else if (difficulty ==2) {
        //中难度：55% 1级，40% 2级，5% 3级
        if (levelRoll > 95) targetLevel = 3;
        else if (levelRoll > 55) targetLevel = 2;
    } else {
        //高难度：20% 1级，50% 2级，30% 3级
        if (levelRoll > 70) targetLevel = 3;
        else if (levelRoll > 20) targetLevel = 2;
    }

    //升到目标等级
    if (unit) {
        unit->changeLevel(targetLevel - 1);
        unit->selfRefresh();
    }
    return unit;
}

void GameManager::setEnemyRandomly() {
    int score = player->getScore();

    //根据积分决定生成概率（每格生成敌人的概率）
    int difficulty;
    if (score <= 70) {
        difficulty = 1;
    } else if (score <= 140) {
        difficulty = 2;
    } else {
        difficulty = 3;
    }

    for (int r = 0; r < preBoard->getRow()/2; ++r) {
        for (int c = 0; c < preBoard->getCol(); ++c) {
            removeUnitAtGrid(r, c);
            int prob = randomNum();
            if (difficulty == 1 && prob < 20) {
                placeUnitAtGrid(r, c, createRandomEnemy(difficulty));
            }
            else if (difficulty == 2 && prob < 30) {
                placeUnitAtGrid(r,c,createRandomEnemy(difficulty));
            }
            else if(difficulty == 3 && prob < 40) {
                placeUnitAtGrid(r,c,createRandomEnemy(difficulty));
            }
        }
    }
}

void GameManager::goToNextLevel() {
    if (result==ComResult::Success) {
        switch (currentLevel) {
            case GameLevel::ONE:
                currentLevel = GameLevel::TWO;
                std::cout<<"NEXT LEVEL"<<std::endl;
                break;
            case GameLevel::TWO:
                currentLevel = GameLevel::THREE;
                std::cout<<"NEXT LEVEL"<<std::endl;
                break;
            case GameLevel::THREE:
                currentLevel = GameLevel::FOUR;
                std::cout<<"NEXT LEVEL"<<std::endl;
                break;
            case GameLevel::FOUR:
                currentLevel = GameLevel::FOUR;
                std::cout<<"NEXT LEVEL"<<std::endl;
                break;
        }
    }
}

void GameManager::clearEnemyGrid() {
    for (int r=0;r<preBoard->getRow()/2;++r) {
        for ( int c=0;c<preBoard->getCol();++c) {
            preBoard->removeUnitAt(r,c);
        }
    }
}

void GameManager::resetTheGame() {
    for (int r=0;r<preBoard->getRow();++r) {
        for (int c=0;c<preBoard->getCol();++c) {
            preBoard->removeUnitAt(r,c);
        }
    }
    for (int k =0;k<preBoard->getBen();++k) {
        preBoard->removeUnitAt(k);
    }
    for (int r=0;r<5;++r) {
        shop->removeUnitAt(r);
    }
    result = ComResult::Draw;
    currentLevel = GameLevel::ONE;
    player->reset();
}

void GameManager::applyTraitBuffs() {
    //统计我方场上各职业数量
    int warriorCount = 0;  // 战士：Knight + Guardian
    int mageCount = 0;     // 法师：Mage + Warlock
    int assassinCount = 0; // 刺客：Assassin + Archer

    for (int r = 0; r < preBoard->getRow(); ++r) {
        for (int c = 0; c < preBoard->getCol(); ++c) {
            Unit* unit = preBoard->getUnitAt(r,c);
            if (unit != nullptr && unit->getOwner() == 0) {
                QString name = unit->getName();
                if (name == "Knight" || name == "Guardian") {
                    warriorCount++;
                } else if (name == "Mage" || name == "Warlock") {
                    mageCount++;
                } else if (name == "Assassin" || name == "Archer") {
                    assassinCount++;
                }
            }
        }
    }

    //计算羁绊buff值
    //战士：2人触发，全体+15攻击
    int tHp=0, tAtt=0, tAttSpd=0, tAttArea=0, tMoveSpd=0, tMana=0;
    if (warriorCount >= 2) {
        tAtt += 15;
    }
    //法师：2人触发，全体攻速CD-1
    if (mageCount >= 2) {
        tAttSpd += 1;
    }

    //先清除旧的羁绊buff，再应用到所有我方棋子
    for (int r = 0; r < preBoard->getRow(); ++r) {
        for (int c = 0; c < preBoard->getCol(); ++c) {
            Unit* unit = preBoard->getUnitAt(r,c);
            if (unit != nullptr && unit->getOwner() == 0) {
                unit->applyTraitBuffs(tHp, tAtt, tAttSpd, tAttArea, tMoveSpd, tMana);
                unit->selfRefresh();
            }
        }
    }
    //机制类buff
    //刺客：2人触发，连击
    if (assassinCount >= 2) {
        for (int r = 0; r < preBoard->getRow(); ++r) {
            for (int c = 0; c < preBoard->getCol(); ++c) {
                Unit* unit = preBoard->getUnitAt(r,c);
                if (unit != nullptr && unit->getOwner() == 0) {
                    QString name = unit->getName();
                    if (name == "Assassin" || name == "Archer") {
                        unit->setTraitFlag(true);
                    }
                }
            }
        }
    }
}

void GameManager::changeStateTo(GameState newGameState) {
    currentState = newGameState;
    switch (currentState) {
        case GameState::Ready:
            handleReady();
            break;
        case GameState::Prepare:
            handlePrepare();
            break;
        case GameState::Combat:
            comBoard->setComBoard(*preBoard);
            applyTraitBuffs();
            timer.start();
            //战斗前检验战斗能否开始
            if (selfUnitCount()==0) {
                timer.stop();
                emit failToCombat();
                changeStateTo(GameState::Prepare);
                break;
            }
            handleCombat();
            break;
        case GameState::Resolve:
            handleResolve();
            break;
    }
}

bool GameManager::isTimerActive() const {
    return timer.isActive();
}

void GameManager::timerStart() {
    timer.start();
}

void GameManager::timerStop() {
    timer.stop();
}

//--------------

void GameManager::handleReady() {
    timer.stop();
}

void GameManager::handlePrepare() {
    timer.stop();
    //刷新所有内容
    for (int r =0;r<preBoard->getRow();++r) {
        for (int c=0;c<preBoard->getCol();++c) {
            Unit* unit = preBoard->getUnitAt(r,c);
            if (unit) {
                //先处理装备的事情
                if (unit->isWearingEquipment()) {
                    bool hasCell = false;
                    for (int k = 0;k<5;k++) {
                        if (isEquipmentEmpty(k)) {
                            hasCell = true;
                            Equipment* equip = unit->takeOffEquipment(hasCell);
                            placeEquipmentAt(k,equip);
                        }
                    }
                }
                //下面脱过装备之后清除羁绊buff再刷新
                unit->clearTraitBuffs();
                unit->selfRefresh();
            }
        }
    }
    setEnemy();
    emit shouldUpdate();
}

void GameManager::handleCombat() {
    std::cout<<"-------------clock arrive--------------"<<std::endl;
    //每一轮行动所有棋子之后都要考虑战斗有没有结束
    comBoard->marchEveryUnit();
    if (isComEnd()) {
        timer.stop();
        emit shouldUpdate();
        // sleepMs(1000);
        handleResolve();
    }
    emit shouldUpdate();
}

void GameManager::handleResolve() {
    switch (result) {
        case ComResult::Success:
            player->changeMoney(20);
            player->changeScore(10);
            break;
        case ComResult::Failure:
            player->changeMoney(10);
            player->changeHp(-20);
            break;
        case ComResult::Draw:
            player->changeMoney(15);
            player->changeHp(-10);
            player->changeScore(5);
            break;
    }
    emit enterResolve(result,player->getHp());

    //随机获得装备：20%概率，四种装备等可能，装备区满则不添加
    int equipRoll = randomNum();
    if (equipRoll <= 40) {
        int equipType = randomNum();
        Equipment* newEquip = nullptr;
        if (equipType <= 25) {
            newEquip = new Sword();
        } else if (equipType <= 50) {
            newEquip = new ChainMail();
        } else if (equipType <= 75) {
            newEquip = new RapidGloves();
        } else {
            newEquip = new BlueCrystal();
        }
        //找第一个空位放入
        for (int k = 0; k < 5; ++k) {
            if (isEquipmentEmpty(k)) {
                placeEquipmentAt(k, newEquip);
                break;
            }
            //如果到最后一个还没空位，丢弃这件装备
            if (k == 4) {
                delete newEquip;
            }
        }
    }

    if (player->getHp()>0) {
        player->changeShopRefreshTimes(+5);
        refreshShop();
        goToNextLevel();
        changeStateTo(GameState::Prepare);
    }else {
        resetTheGame();
        emit shouldUpdate();
        changeStateTo(GameState::Ready);
    }
}

//判断战斗是否结束，同时记录战斗结果
bool GameManager::isComEnd(){
    int selfCount =0;
    int enemyCount =0;
    //敌我计数
    for (int r=0;r<preBoard->getRow();++r) {
        for (int c=0;c<preBoard->getCol();++c) {
            if (!isCellEmptyGrid(r,c)) {
                Unit* unit=getUnitAtGrid(r,c);
                if (unit->getOwner()==0) {
                    selfCount++;
                }else {
                    enemyCount++;
                }
            }
        }
    }

    if (selfCount==0&&enemyCount!=0) {
        result = ComResult::Failure;
    }
    else if (selfCount!=0&&enemyCount==0) {
        result = ComResult::Success;
    }
    else {
        result = ComResult::Draw;
    }

    //如果一方为0，战斗结束
    if (selfCount==0||enemyCount==0) {
        return true;
    }
    return false;
}

//-------------存档系统--------------//

//辅助：根据名字创建英雄
static Unit* createUnitByName(const QString& name, int owner, int level) {
    Unit* unit = nullptr;
    if (name == "Knight") unit = new Knight(owner, level);
    else if (name == "Mage") unit = new Mage(owner, level);
    else if (name == "Archer") unit = new Archer(owner, level);
    else if (name == "Guardian") unit = new Guardian(owner, level);
    else if (name == "Assassin") unit = new Assassin(owner, level);
    else if (name == "Warlock") unit = new Warlock(owner, level);
    return unit;
}

//辅助：根据名字创建装备
static Equipment* createEquipByName(const QString& name) {
    if (name == "Sword") return new Sword();
    if (name == "ChainMail") return new ChainMail();
    if (name == "RapidGloves") return new RapidGloves();
    if (name == "BlueCrystal") return new BlueCrystal();
    return nullptr;
}

//辅助：保存一个棋子
static void saveUnit(QDataStream& out, Unit* unit) {
    if (unit == nullptr) {
        out << false;
    } else {
        out << true;
        out << unit->getName() << unit->getOwner() << unit->getLevel();
        if (unit->isWearingEquipment()) {
            out << unit->takeOffEquipment(true)->getName();
            //注意：takeOff会触发selfRefresh，需要重新穿上
            //所以改用另一个方式：直接通过isWearingEquipment判断后手动获取
        } else {
            out << QString("None");
        }
    }
}

void GameManager::saveGame(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        std::cout << "fail" << std::endl;
        return;
    }
    QDataStream out(&file);

    //保存关卡进度
    int levelInt = static_cast<int>(currentLevel);
    out << levelInt;

    //保存Player
    player->saveTo(out);

    //保存棋盘 grid
    int row = preBoard->getRow();
    int col = preBoard->getCol();
    out << row << col;
    for (int r = 0; r < row; ++r) {
        for (int c = 0; c < col; ++c) {
            Unit* unit = preBoard->getUnitAt(r, c);
            if (unit == nullptr) {
                out << false;
            } else {
                out << true;
                out << unit->getName() << unit->getOwner() << unit->getLevel();
                //这里简单处理，写装备名或"None"
                if (unit->isWearingEquipment()) {
                    //临时脱下记录名字再穿回去
                    Equipment* eq = unit->getWearingEquipment();
                    QString eqName = eq->getName();
                    out << eqName;
                } else {
                    out << QString("None");
                }
            }
        }
    }

    //保存备战区 bench
    int ben = preBoard->getBen();
    out << ben;
    for (int k = 0; k < ben; ++k) {
        Unit* unit = preBoard->getUnitAt(k);
        if (unit == nullptr) {
            out << false;
        } else {
            out << true;
            out << unit->getName() << unit->getOwner() << unit->getLevel();
            if (unit->isWearingEquipment()) {
                Equipment* eq = unit->getWearingEquipment();
                QString eqName = eq->getName();
                out << eqName;
            } else {
                out << QString("None");
            }
        }
    }

    //保存商店
    for (int s = 0; s < 5; ++s) {
        Unit* unit = shop->getUnitAt(s);
        if (unit == nullptr) {
            out << false;
        } else {
            out << true;
            out << unit->getName() << unit->getOwner() << unit->getLevel();
            out << QString("None");
        }
    }

    //保存装备库
    for (int s = 0; s < 5; ++s) {
        Equipment* eq = equipment[s];
        if (eq == nullptr) {
            out << QString("None");
        } else {
            out << eq->getName();
        }
    }

    file.close();
    std::cout << "success" << std::endl;
}

void GameManager::loadGame(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cout << "fail" << std::endl;
        return;
    }
    QDataStream in(&file);

    //先清空当前状态
    resetTheGame();

    //读取关卡进度
    int levelInt;
    in >> levelInt;
    currentLevel = static_cast<GameLevel>(levelInt);

    //读取Player
    player->loadFrom(in);

    //读取棋盘 grid
    int row, col;
    in >> row >> col;
    for (int r = 0; r < row; ++r) {
        for (int c = 0; c < col; ++c) {
            bool hasUnit;
            in >> hasUnit;
            if (hasUnit) {
                QString name;
                int owner, level;
                QString eqName;
                in >> name >> owner >> level >> eqName;
                Unit* unit = createUnitByName(name, owner, level);
                if (unit && eqName != "None") {
                    unit->putOnEquipment(createEquipByName(eqName));
                }
                preBoard->placeUnitAt(r, c, unit);
            }
        }
    }

    //读取备战区 bench
    int ben;
    in >> ben;
    for (int k = 0; k < ben; ++k) {
        bool hasUnit;
        in >> hasUnit;
        if (hasUnit) {
            QString name;
            int owner, level;
            QString eqName;
            in >> name >> owner >> level >> eqName;
            Unit* unit = createUnitByName(name, owner, level);
            if (unit && eqName != "None") {
                unit->putOnEquipment(createEquipByName(eqName));
            }
            preBoard->placeUnitAt(k, unit);
        }
    }

    //读取商店
    for (int s = 0; s < 5; ++s) {
        bool hasUnit;
        in >> hasUnit;
        if (hasUnit) {
            QString name;
            int owner, level;
            QString eqName;
            in >> name >> owner >> level >> eqName;
            Unit* unit = createUnitByName(name, owner, level);
            shop->placeUnitAt(s, unit);
        }
    }

    //读取装备库
    for (int s = 0; s < 5; ++s) {
        QString eqName;
        in >> eqName;
        if (eqName != "None") {
            equipment[s] = createEquipByName(eqName);
        }
    }

    file.close();
    //切换到准备状态
    currentState = GameState::Prepare;
    emit shouldUpdate();
    std::cout << "success" << std::endl;
}

