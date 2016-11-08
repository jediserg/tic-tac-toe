//
// Created by serg on 11/7/16.
//

#ifndef TIC_TAC_TOE_GAME_H
#define TIC_TAC_TOE_GAME_H

#include <memory>
#include <vector>
#include "Board.h"
#include <memory>
class User;

class PlayerBase;

class Board;

class Game {
public:
    std::shared_ptr<PlayerBase> getPlayer1() const;

    std::shared_ptr<PlayerBase> getPlayer2() const;

    bool joinAsPlayer(std::shared_ptr<User> user);

    void joinAsObserver(std::shared_ptr<User> user);

    const std::vector<std::shared_ptr<User>> getObservers() const;

    void onMove(Mark mark, unsigned long row, unsigned long column);

    void start();
private:
    std::vector<std::shared_ptr<User>> _observers;

    std::shared_ptr<PlayerBase> _player1;
    std::shared_ptr<PlayerBase> _player2;

    std::unique_ptr<Board> _board;

    Mark _current_turn;
};


#endif //TIC_TAC_TOE_GAME_H
