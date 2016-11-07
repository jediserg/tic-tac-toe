//
// Created by serg on 11/7/16.
//

#ifndef TIC_TAC_TOE_GAME_H
#define TIC_TAC_TOE_GAME_H

#include <memory>
#include <vector>

class User;

class PlayerBase;

class Board;

class Game {
public:
private:
    std::vector<std::shared_ptr<User>> _observers;

    std::unique_ptr<PlayerBase> _player1;
    std::unique_ptr<PlayerBase> _player2;

    std::unique_ptr<Board> _board;
};


#endif //TIC_TAC_TOE_GAME_H
