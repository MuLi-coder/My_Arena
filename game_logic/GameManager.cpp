//
// Created by ml on 2026/5/30.
//

#include "GameManager.h"
#include <iostream>
void GameManager::changeState(GameState newGameState) {
    std::cout <<"GAMESTATE CHANGE FROM " << static_cast<int>(currentState) <<" TO "<< static_cast<int>(newGameState) <<std::endl;
}

void GameManager::updateGameState() {
    switch (currentState) {
        case GameState::Prepare:
            handlePrepare();
            break;
        case GameState::Combat:
            handleCombat();
            break;
    }
}


void GameManager::handlePrepare() {

}

void GameManager::handleCombat() {

}