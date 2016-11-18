//
// Created by serg on 11/15/16.
//

#ifndef TIC_TAC_TOE_GAMEMANAGER_H
#define TIC_TAC_TOE_GAMEMANAGER_H

#include "ApiManager.h"

class Game;

class GameManager {
public:
    static constexpr const char *GET_GAMES_LIST = "get_game_list";

    GameManager(ApiManager &api_manager) : _api_manager(api_manager) {};

    GameManager(GameManager &) = delete;

    GameManager() = delete;

    void setHandlers();

private:
    void _onShowGames(std::string user);

    void _onNewGame(std::string user);

    void _onJoinGame(std::string user);

    void onCloseConnection(std::string user);

    ApiManager &_api_manager;
    std::vector<std::shared_ptr<Game>> _games;
};


#endif //TIC_TAC_TOE_GAMEMANAGER_H
