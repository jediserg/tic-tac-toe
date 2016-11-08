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
    PlayerBase
    //Player should make a move after this command
    virtual void yourTurn(Board &&) = 0;

    void onMove(std::function<void(unsigned long, unsigned long)> on_move) { _on_move = on_move; }

    void makeMove(unsigned long row, unsigned long column) { _on_move(row, column); }
private:
    std::function<void(unsigned long, unsigned long)> _on_move;
};


#endif //TIC_TAC_TOE_PLAYERBASE_H
