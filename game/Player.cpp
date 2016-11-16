//
// Created by serg on 11/15/16.
//

#include "Player.h"

Player::Player(Api::Callback response) : PlayerBase(), _send_to_player(response) {}

void Player::_moveRequest(const Board &board) {
    _send_to_player(board.toJson());
}

