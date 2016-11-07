//
// Created by serg on 11/5/16.
//

#ifndef TIC_TAC_TOE_PLAYERBASE_H
#define TIC_TAC_TOE_PLAYERBASE_H

#include <functional>

class Position;

class Board;

class PlayerBase {
public:
    //Player should make a move after this command
    void yourTurn(Board &&) = 0;

    void onMove(std::function<void(std::Position &&)> on_move) { _on_move = on_move; }

    void makeMove(std::Position &&position) { _on_move(std::move(position)); }

private:
    std::function<void(std::Position &&)>)
    _on_move;
};


#endif //TIC_TAC_TOE_PLAYERBASE_H
