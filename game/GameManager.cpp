//
// Created by serg on 11/15/16.
//

#include "GameManager.h"
#include "Game.h"

void GameManager::_onShowGameStatus() {

}

void GameManager::setHandlers() {
    _api_manager.setHandlerForApi({"1.0", "2.0"}, GET_GAMES_LIST, []() {

    });
}
