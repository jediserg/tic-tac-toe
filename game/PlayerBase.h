//
// Created by serg on 11/5/16.
//

#ifndef TIC_TAC_TOE_PLAYERBASE_H
#define TIC_TAC_TOE_PLAYERBASE_H

#include <functional>
#include "Board.h"

class Position;

class PlayerBase {
public:
    PlayerBase() : _mark(Mark::UNMARKED) {}

    virtual ~PlayerBase() = default;


//Player should make a move after this command
    void yourTurn(const Board &board) {
        _is_waiting_for_move = true;
        _moveRequest(board);
    };

    void onMove(std::function<void(unsigned long, unsigned long)> on_move) { _on_move = on_move; }

    void makeMove(unsigned long row, unsigned long column) {
        _is_waiting_for_move = false;
        _on_move(row, column);
    }

protected:
    bool isWaitingForMove() const { return _is_waiting_for_move; }

    virtual void _moveRequest(const Board &board) = 0;
private:
    std::function<void(unsigned long, unsigned long)> _on_move;
    Mark _mark;
    bool _is_waiting_for_move;
};

#endif //TIC_TAC_TOE_PLAYERBASE_H
