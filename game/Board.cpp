//
// Created by serg on 11/5/16.
//

#include "Board.h"

Board::Board(unsigned long size) : _cells(size, std::vector<Mark>(size, Mark::UNMARKED)), _size(size) {
}

void Board::putMark(int row, int column, Mark mark) {
    _cells[row][column] = mark;
}

const Board::Cells &Board::getCells() const {
    return _cells;
}


Mark Board::getCell(unsigned long row, unsigned long column) const {
    return _cells[row][column];
}

Win Board::isWin(Mark mark) const {
    std::vector<int> marksInRow(_cells.size(), 0);
    int marksInCurrentColumn = 0;

    int marksInDiagonal1 = 0;
    int marksInDiagonal2 = 0;

    for (int i = 0; i < _size; ++i) {
        for (int j = 0; j < _size; ++j) {
            if (_cells[i][j] == mark) {
                marksInCurrentColumn++;
                marksInRow[j]++;

                if (i == j)
                    marksInDiagonal1++;

                if (_size - i - 1 == j)
                    marksInDiagonal2++;
            }
        }

        if (marksInCurrentColumn == _size)
            return Win(mark, WinType::ROW_EQUAL, i);

        marksInCurrentColumn = 0;
    }

    if (marksInDiagonal1 == _size)
        return Win(mark, WinType::DIAGONAL_EQUAL, 1);

    if (marksInDiagonal2 == _size)
        return Win(mark, WinType::DIAGONAL_EQUAL, 2);

    for (int i = 0; i < marksInRow.size(); i++)
        if (marksInRow[i] == _size)
            return Win(mark, WinType::COLUMN_EQUAL, i);

    return Win();
}

void Board::putRow(int num, std::vector<Mark> marks) {
    if (marks.size() != _size)
        throw std::invalid_argument("Invalid row number");

    _cells[num] = marks;
}

/*
 * {
 *  "board" : [
 *      [0, 1, 2],
 *      [0, 0, 2],
 *      [0, 0, 0]
 *  ]
 * }
*/
nlohmann::json Board::toJson() const {
    nlohmann::json json;
    nlohmann::json temp_json;

    json["board"] = nlohmann::json::array();

    for (const auto &row : _cells) {
        std::vector<int> json_row(_size);
        for (int i = 0; i < _size; ++i) {
            json_row[i] = static_cast<int>(row[i]);
        }
        json["board"].push_back(json_row);
    }
    return json;
}
