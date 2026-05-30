//
// Created by ml on 2026/5/30.
//

#ifndef MY_ARENA_GAMEMANAGER_H
#define MY_ARENA_GAMEMANAGER_H

enum class GameState {
    Prepare,
    Combat,
    Resolve
};

class GameManager {
private:
    GameState currentState;
    void handlePrepare();
    void handleCombat();
public:
    static GameManager getCurrentState();
    void changeState(GameState newGameState);
    void updateGameState();
};


#endif //MY_ARENA_GAMEMANAGER_H