//
// Created by serg on 11/15/16.
//

#include "GameManager.h"

GameManager &GameManager::getInstance() {
    static GameManager game_manager;
    return game_manager;
}

GameManager::GameManager() {

}
