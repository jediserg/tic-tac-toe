//
// Created by serg on 11/7/16.
//
#include "Game.h"
#include "PlayerBase.h"

std::shared_ptr<PlayerBase> Game::getPlayer1() const {
    return _player1;
}

std::shared_ptr<PlayerBase> Game::getPlayer2() const {
    return _player2;
}

bool Game::joinAsPlayer(std::shared_ptr<User> user) {
    return false;
}

void Game::joinAsObserver(std::shared_ptr<User> user) {

}

const std::vector<std::shared_ptr<User>> Game::getObservers() const {
    return _observers;
}

void Game::onMove(Mark mark, unsigned long row, unsigned long column) {
    // if ()
}

void Game::start() {
    using namespace std::placeholders;

    _player1->onMove(std::bind(&Game::onMove, this, Mark::X, _1, _2));
    _player2->onMove(std::bind(&Game::onMove, this, Mark::O, _1, _2));

    _current_turn = Mark::X;
    //_player1->yourTurn(_board);
}
