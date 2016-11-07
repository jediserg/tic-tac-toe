//
// Created by serg on 11/5/16.
//

#include "Board.h"

Board::Board(size_t size) : _cells(size, size, Mark::UNMARKED) {

}

void Board::putMark(size_t row, size_t column, Mark mark) {
    _cells(row, column) = mark;
}

const Board::Cells &Board::getCells() const {
    return _cells;
}

Mark Board::getPosition(size_t row, size_t column) const {
    return Mark::MARK2;
}
