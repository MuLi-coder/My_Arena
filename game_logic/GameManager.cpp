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

Unit* GameManager::getUnitAt(const int r,const int c) const {
    if (currentState==GameState::Prepare) {
        return preBoard->getUnitAt(r,c);
    }
    if (currentState==GameState::Combat) {
        return comBoard->getUnitAt(r,c);
    }
}

Unit* GameManager::getUnitAt(const int pos) const {
    return preBoard->getUnitAt(pos);
}

bool GameManager::isCellEmpty(int r, int c) const {
    if (currentState==GameState::Prepare) {
        return preBoard->isCellEmpty(r,c);
    }
    if (currentState==GameState::Combat) {
        return comBoard->isCellEmpty(r,c);
    }
}

bool GameManager::isEmpty(int pos) const {
    return preBoard->isCellEmpty(pos);
}



void GameManager::removeUnitAt(const int r,const int c) {
    if (currentState == GameState::Prepare) {
        preBoard->removeUnitAt(r,c);
    }
}

void GameManager::removeUnitAt(const int pos) {
    if (currentState == GameState::Prepare) {
        preBoard->removeUnitAt(pos);
    }
}

void GameManager::placeUnitAt(const int r,const int c,Unit* unit) {
    if (currentState == GameState::Prepare && preBoard->isCellEmpty(r,c)) {
        preBoard->placeUnitAt(r,c,unit);
    }
}

void GameManager::placeUnitAt(const int pos,Unit* unit) {
    if (currentState == GameState::Prepare && preBoard->isCellEmpty(pos)) {
        preBoard->placeUnitAt(pos,unit);
    }
}

//---------------------
void GameManager::changeStateTo(GameState newGameState) {
    currentState = newGameState;
    std::cout<<"ready to change"<<std::endl;
    reset();
    std::cout<<"STATE CHANGED TO "<<static_cast<int>(currentState)<<std::endl;
}

void GameManager::reset() {
    switch (currentState) {
        case GameState::Prepare:
            handlePrepare();
            break;
        case GameState::Combat:
            std::cout<<"reset"<<std::endl;
            comBoard->setComBoard(*preBoard);
            std::cout<<"copy"<<std::endl;
            handleCombat();
            break;
        case GameState::Resolve:
            handleResolve();
            break;
    }
}

void GameManager::timer_stop() {
    timer.stop();
}


void GameManager::handlePrepare() {
    timer.stop();
    preBoard->rePrepare();
    emit shouldUpdate();
}

void GameManager::handleCombat() {
    timer.start();
    std::cout<<"clock arrive"<<std::endl;
    comBoard->marchEveryUnit();
    emit shouldUpdate();

    //每一轮行动所有棋子之后都要考虑战斗有没有结束
    if (isComEnd()) {
        timer.stop();
        handleResolve();
    }

}

bool GameManager::isComEnd() const {
    int selfCount =0;
    int enemyCount =0;
    //敌我计数
    for (int r=0;r<preBoard->getRow();++r) {
        for (int c=0;c<preBoard->getCol();++c) {
            if (!isCellEmpty(r,c)) {
                Unit* unit=getUnitAt(r,c);
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