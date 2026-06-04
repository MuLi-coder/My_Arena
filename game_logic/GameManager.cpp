//
// Created by ml on 2026/5/30.
//

#include "GameManager.h"
#include <iostream>

GameManager::GameManager(const int r,const int c, const int pos) {
    board = std::make_unique<Board>(r,c,pos);
    currentState = GameState::Prepare;
    player = std::make_unique<Player>();
    timer.setInterval(1000);
    connect(&timer, &QTimer::timeout, this, &GameManager::handleCombat);
}

GameState GameManager::getCurrentState() const {
    return currentState;
}

int GameManager::getRow() const { return board->getRow(); }
int GameManager::getCol() const { return board->getCol(); }
int GameManager::getBen() const { return board->getBen(); }

Unit* GameManager::getUnitAt(const int r,const int c) const {
    return board->getUnitAt(r,c);
}
Unit* GameManager::getUnitAt(const int pos) const {
    return board->getUnitAt(pos);
}

bool GameManager::isEmpty(int r, int c) const {
    return board->isCellEmpty(r,c);
}

bool GameManager::isEmpty(int pos) const {
    return board->isCellEmpty(pos);
}

void GameManager::changeStateTo(GameState newGameState) {
    currentState = newGameState;
    Reset();
    std::cout<<"STATE CHANGED TO "<<static_cast<int>(currentState)<<std::endl;
    if (currentState == GameState::Prepare) {
        timer.stop();
    }else if (currentState == GameState::Combat) {
        timer.start();
    }
}

void GameManager::Reset() {
    switch (currentState) {
        case GameState::Prepare:
            handlePrepare();
            break;
        case GameState::Combat:
            handleCombat();
            break;
        case GameState::Resolve:
            handleResolve();
            break;
    }
}

void GameManager::removeUnitAt(const int r,const int c) {
    if (currentState == GameState::Prepare) {
        board->removeUnitAt(r,c);
    }
}

void GameManager::removeUnitAt(const int pos) {
    if (currentState == GameState::Prepare) {
        board->removeUnitAt(pos);
    }
}

void GameManager::placeUnitAt(const int r,const int c,Unit* unit) {
    if (currentState == GameState::Prepare && board->isCellEmpty(r,c)) {
        board->placeUnitAt(r,c,unit);
    }
}

void GameManager::placeUnitAt(const int pos,Unit* unit) {
    if (currentState == GameState::Prepare && board->isCellEmpty(pos)) {
        board->placeUnitAt(pos,unit);
    }
}


void GameManager::handlePrepare() {
    emit shouldUpdate();
    board->rePrepare();
}

void GameManager::handleCombat() {
    board->moveUnit();
    emit shouldUpdate();
}

void GameManager::handleResolve() {

}

void GameManager::timer_stop() {
    timer.stop();
}