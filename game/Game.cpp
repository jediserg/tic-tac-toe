//
// Created by serg on 11/7/16.
//
#include "Game.h"
#include "PlayerBase.h"
#include "Player.h"
#include "ServerType.h"

std::shared_ptr<PlayerBase> Game::getPlayer1() const {
    return _player1;
}

std::shared_ptr<PlayerBase> Game::getPlayer2() const {
    return _player2;
}

bool Game::joinAsPlayer(std::string new_user) {
    if (_player1 && _player2)
        return false;

    if (_player1) {
        _player2 = std::make_shared<Player>(new_user, Mark::O);
    } else {
        _player1 = std::make_shared<Player>(new_user, Mark::X);
    }
}

void Game::joinAsObserver(std::shared_ptr<User> user) {

}

const std::vector<std::string> &Game::getObservers() const {
    return _observers;
}

void Game::onMove(std::string name, int row, int column) {
    if (name == _player1->getName()) {
        _board.putMark(row, column, Mark::X);
        Win win = _board.isWin(Mark::X);
        if (win.type != WinType::NONE) {
            ServerType::get().sendMessageToUser(name, {
                    {"command", "win"}
                    { "type", static_cast<int>(win.type) },
                    { "value", win.value },
            });
        }
        return;
    } else if (name == _player2->getName()) {
        _board.putMark(row, column, Mark::O);
        return;
    }

    ServerType::get().sendMessageToUser(name, {{"error", "You are not in the game"}});
}

void Game::start() {
    using namespace std::placeholders;

    //_player1->onMove(std::bind(&Game::onMove, this, Mark::X, _1, _2));
    //_player2->onMove(std::bind(&Game::onMove, this, Mark::O, _1, _2));

    _current_turn = Mark::X;

    _player1->yourTurn(_board);
}

Game::Game(std::string player_name) : _board(BOARD_SIZE) {
    _player1 = std::make_shared<Player>(player_name, Mark::X);
}
