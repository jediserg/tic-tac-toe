//
// Created by serg on 11/15/16.
//

#include "Player.h"

Player::Player(std::string name, Mark mark) : PlayerBase(name, mark) {}

void Player::_moveRequest(const Board &board) {
    //_send_to_player(board.toJson());
}

