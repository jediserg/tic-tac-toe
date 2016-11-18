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
    static constexpr size_t BOARD_SIZE = 3;

    Game(std::string player_name);
    std::shared_ptr<PlayerBase> getPlayer1() const;

    std::shared_ptr<PlayerBase> getPlayer2() const;

    bool joinAsPlayer(std::string new_user);

    void joinAsObserver(std::shared_ptr<User> user);

    const std::vector<std::string> &getObservers() const;

    void onMove(std::string name, int row, int column);

    void start();
private:
    std::vector<std::string> _observers;

    std::shared_ptr<PlayerBase> _player1;
    std::shared_ptr<PlayerBase> _player2;

    Board _board;

    Mark _current_turn;
};


#endif //TIC_TAC_TOE_GAME_H
