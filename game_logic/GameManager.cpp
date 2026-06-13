//
// Created by ml on 2026/5/30.
//

#include "GameManager.h"
#include <iostream>

GameManager::GameManager(const int r,const int c, const int pos) {
    preBoard = std::make_unique<PreBoard>(r,c,pos);
    comBoard = std::make_unique<ComBoard>(*preBoard);
    currentState = GameState::Prepare;//默认备战状态
    player = std::make_unique<Player>();
    shop = std::make_unique<Shop>();
    //初始化计时器，设置间隔和槽函数
    timer.setInterval(700);
    connect(&timer, &QTimer::timeout, this, &GameManager::handleCombat);
}

GameState GameManager::getCurrentState() const {
    return currentState;
}

int GameManager::getRow() const { return preBoard->getRow(); }
int GameManager::getCol() const { return preBoard->getCol(); }
int GameManager::getBen() const { return preBoard->getBen(); }

Unit* GameManager::getUnitAtGrid(const int r,const int c) const {
    if (currentState==GameState::Prepare) {
        return preBoard->getUnitAt(r,c);
    }
    if (currentState==GameState::Combat) {
        return comBoard->getUnitAt(r,c);
    }
}

Unit* GameManager::getUnitAtBench(const int pos) const {
    return preBoard->getUnitAt(pos);
}

Unit* GameManager::getUnitAtShop(const int s) const {
    return shop->getUnitAt(s);
}

bool GameManager::isCellEmptyGrid(int r, int c) const {
    if (currentState==GameState::Prepare) {
        return preBoard->isCellEmpty(r,c);
    }
    if (currentState==GameState::Combat) {
        return comBoard->isCellEmpty(r,c);
    }
    return true;
}

bool GameManager::isCellEmptyShop(const int s) const {
    return shop->isCellEmpty(s);
}

bool GameManager::isCellEmptyBench(int pos) const {
    return preBoard->isCellEmpty(pos);
}



void GameManager::removeUnitAtGrid(const int r,const int c) {
    if (currentState == GameState::Prepare) {
        preBoard->removeUnitAt(r,c);
    }
}

void GameManager::removeUnitAtBench(const int pos) {
    if (currentState == GameState::Prepare) {
        preBoard->removeUnitAt(pos);
    }
}

void GameManager::removeUnitAtShop(int s) {
    shop->removeUnitAt(s);
}

void GameManager::placeUnitAtShop(int s, Unit *unit) {
    shop->placeUnitAt(s,unit);
}

void GameManager::refreshShop() {
    std::vector<int> intShop = shop->refresh();
    //先重置
    for (int t=0;t<5;++t) {
        placeUnitAtShop(t,nullptr);
    }
    for (int s=0;s<5;++s) {
        switch (intShop[s]) {
            case 0:
                placeUnitAtShop(s,new Knight("Knight"));
                break;
            case 1:
                placeUnitAtShop(s,new Mage("Mage"));
            default:
                break;
        }
    }
}

void GameManager::placeUnitAtGrid(const int r,const int c,Unit* unit) {
    if (currentState == GameState::Prepare && preBoard->isCellEmpty(r,c)) {
        preBoard->placeUnitAt(r,c,unit);
    }
}

void GameManager::placeUnitAtBench(const int pos,Unit* unit) {
    if (currentState == GameState::Prepare && preBoard->isCellEmpty(pos)) {
        preBoard->placeUnitAt(pos,unit);
    }
}

//---------------------
void GameManager::changeStateTo(GameState newGameState) {
    currentState = newGameState;
    reset();
    std::cout<<"STATE CHANGED TO "<<static_cast<int>(currentState)<<std::endl;
}

void GameManager::reset() {
    switch (currentState) {
        case GameState::Prepare:
            handlePrepare();
            break;
        case GameState::Combat:
            comBoard->setComBoard(*preBoard);
            timer.start();
            if (isComEnd()) {
                timer.stop();
                handleResolve();
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


void GameManager::handlePrepare() {
    timer.stop();
    preBoard->rePrepare();
    emit shouldUpdate();
}

void GameManager::handleCombat() {
    std::cout<<"clock arrive"<<std::endl;
    //每一轮行动所有棋子之后都要考虑战斗有没有结束
    comBoard->marchEveryUnit();
    if (isComEnd()) {
        timer.stop();
        handleResolve();
    }
    emit shouldUpdate();
}

bool GameManager::isComEnd() const {
    int selfCount =0;
    int enemyCount =0;
    //敌我计数
    for (int r=0;r<preBoard->getRow();++r) {
        for (int c=0;c<preBoard->getCol();++c) {
            if (!isCellEmptyGrid(r,c)) {
                Unit* unit=getUnitAtGrid(r,c);
                if (unit->getOwner()==1) {
                    selfCount++;
                }else {
                    enemyCount++;
                }
            }
        }
    }
    //如果一方为0，战斗结束
    if (selfCount==0||enemyCount==0) {
        return true;
    }
    return false;
}

void GameManager::handleResolve() {
    std::cout<<"COMBAT END"<<std::endl;
}