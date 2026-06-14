//
// Created by ml on 2026/5/30.
//

#include "GameManager.h"
#include <iostream>
#include <QMessageBox>

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
    currentState = GameState::Ready;
    currentLevel = GameLevel::ONE;
    result = ComResult::Draw;
    //初始化计时器，设置间隔和槽函数
    timer.setInterval(300);
    connect(&timer, &QTimer::timeout, this, &GameManager::handleCombat);
}


GameState GameManager::getCurrentState() const {
    return currentState;
}

int GameManager::getRow() const { return preBoard->getRow(); }
int GameManager::getCol() const { return preBoard->getCol(); }
int GameManager::getBen() const { return preBoard->getBen(); }

int GameManager::getPlayerMoney() const { return player->getMoney(); }
void GameManager::changePlayerMoney(int num) {
    player->changeMoney(num);
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

Unit* GameManager::getUnitAtShop(const int s) const {
    return shop->getUnitAt(s);
}

bool GameManager::isCellEmptyGrid(int r, int c) const {
    if (currentState==GameState::Combat) {
        return comBoard->isCellEmpty(r,c);
    }else {
        return preBoard->isCellEmpty(r,c);
    }

}

bool GameManager::isCellEmptyShop(const int s) const {
    return shop->isCellEmpty(s);
}

bool GameManager::isCellEmptyBench(int pos) const {
    return preBoard->isCellEmpty(pos);
}

int GameManager::getPlayerShopRefreshTimes() const {
    return player->getShopRefreshTimes();
}

void GameManager::levelUp() {
    player->changeLevel(1);
}

void GameManager::removeUnitAtGrid(const int r,const int c) {
    preBoard->removeUnitAt(r,c);
}

void GameManager::removeUnitAtBench(const int pos) {
    preBoard->removeUnitAt(pos);
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
        if (ratio<50) {
            placeUnitAtShop(t,new Knight());
        }else {
            placeUnitAtShop(t,new Mage());
        }
    }
}

void GameManager::changeShopRefreshTimes(const int num) {
    player->changeShopRefreshTimes(num);
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

int GameManager::getPlayerScore() const {
    return player->getScore();
}

int GameManager::getPlayerLevel() const {
    return player->getLevel();
}

Player* GameManager::getPlayer() const {
    return player.get();
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

int GameManager::getMaxUnit() const {
    return player->getMaxUnits();
}

void GameManager::setEnemy() {
    switch (currentLevel) {
        case GameLevel::ONE:
            clearEnemyGrid();
            placeUnitAtGrid(2,4,new Knight(1));
            break;
        case GameLevel::TWO:
            clearEnemyGrid();
            placeUnitAtGrid(2,1,new Knight(1));
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

void GameManager::setEnemyRandomly() {
    for (int r=0;r<preBoard->getRow()/2;++r) {
        for (int c=0;c<preBoard->getCol();++c) {
            int ratio = randomNum();
            if (ratio<80) {
                removeUnitAtGrid(r,c);
            }
            else if (ratio<90) {
                removeUnitAtGrid(r,c);
                placeUnitAtGrid(r,c,new Knight(1));
            }
            else {
                removeUnitAtGrid(r,c);
                placeUnitAtGrid(r,c,new Mage(1));
            }
        }
    }
}

void GameManager::goToNextLevel() {
    if (result==ComResult::Success) {
        switch (currentLevel) {
            case GameLevel::ONE:
                currentLevel = GameLevel::TWO;
                break;
            case GameLevel::TWO:
                currentLevel = GameLevel::THREE;
                break;
            case GameLevel::THREE:
                currentLevel = GameLevel::FOUR;
                break;
            case GameLevel::FOUR:
                currentLevel = GameLevel::FOUR;
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

//---------------------
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

void GameManager::handleReady() {
    timer.stop();
}

void GameManager::handlePrepare() {
    timer.stop();
    preBoard->rePrepare();
    setEnemy();
    emit shouldUpdate();
}

void GameManager::handleCombat() {
    std::cout<<"clock arrive"<<std::endl;
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
            player->changeMoney(15);
            player->changeScore(10);
            break;
        case ComResult::Failure:
            player->changeMoney(5);
            player->changeHp(-20);
            break;
        case ComResult::Draw:
            player->changeMoney(10);
            player->changeHp(-10);
            player->changeScore(5);
            break;
    }
    emit enterResolve(result,player->getHp());
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

