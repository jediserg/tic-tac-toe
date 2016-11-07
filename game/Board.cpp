//
// Created by serg on 11/5/16.
//

#include "Board.h"

Board::Board(unsigned long size) : _cells(size, size, Mark::UNMARKED) {

}

void Board::putMark(unsigned long row, unsigned long column, Mark mark) {
    _cells(row, column) = mark;
}

const Board::Cells &Board::getCells() const {
    return _cells;
}


Mark Board::getCell(unsigned long row, unsigned long column) const {
    if (row > _cells.size1())
        throw std::invalid_argument("Invalid row number");

    if (row > _cells.size2())
        throw std::invalid_argument("Invalid row number");

    return _cells(row, column);
}

Win Board::isWin(Mark mark) const {
    std::vector<unsigned long> marksInRow(_cells.size1(), 0);
    unsigned long marksInCurrentColumn = 0;

    unsigned long marksInDiagonal1 = 0;
    unsigned long marksInDiagonal2 = 0;

    for (unsigned long i = 0; i < _cells.size1(); ++i) {
        for (unsigned long j = 0; j < _cells.size2(); ++j) {
            if (_cells(i, j) == mark) {
                marksInCurrentColumn++;
                marksInRow[j]++;

                if (i == j)
                    marksInDiagonal1++;

                if (_cells.size1() - i - 1 == j)
                    marksInDiagonal2++;
            }
        }

        if (marksInCurrentColumn == _cells.size2())
            return Win(mark, WinType::ROW_EQUAL, i);

        marksInCurrentColumn = 0;
    }

    if (marksInDiagonal1 == _cells.size1())
        return Win(mark, WinType::DIAGONAL_EQUAL, 1);

    if (marksInDiagonal2 == _cells.size1())
        return Win(mark, WinType::DIAGONAL_EQUAL, 2);

    for (unsigned long i = 0; i < marksInRow.size(); i++)
        if (marksInRow[i] == _cells.size2())
            return Win(mark, WinType::COLUMN_EQUAL, i);

    return Win();
}

void Board::putRow(unsigned long num, std::vector<Mark> marks) {
    if (marks.size() != _cells.size1())
        throw std::invalid_argument("Invalid row number");

    for (unsigned long i = 0; i < marks.size(); i++)
        _cells(num, i) = marks[i];
}
