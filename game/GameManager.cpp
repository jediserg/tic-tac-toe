//
// Created by serg on 11/15/16.
//

#include "GameManager.h"
#include "Game.h"
#include "ServerType.h"

void GameManager::setHandlers() {
    ServerType::get().doOnClose(std::bind(&GameManager::onCloseConnection,
                                          this, std::placeholders::_1));
    _api_manager.setHandlerForApi({"1.0", "2.0"}, GET_GAMES_LIST, [](std::shared_ptr<User>, nlohmann::json &&request) {
        return true;
    });
}

void GameManager::_onShowGames(std::string user) {

}

void GameManager::_onNewGame(std::string user) {
    //_games.push_back();
}

void GameManager::_onJoinGame(std::string user) {

}

void GameManager::onCloseConnection(std::string user) {

}
