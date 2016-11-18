//
// Created by serg on 11/5/16.
//

#ifndef TIC_TAC_TOE_BOARD_H
#define TIC_TAC_TOE_BOARD_H

#include <cstddef>
//#include <boost/numeric/ublas/matrix.hpp>
#include <json.hpp>

enum class Mark {
    UNMARKED = 0, X = 1, O = 2
};

enum class WinType {
    NONE, ROW_EQUAL, COLUMN_EQUAL, DIAGONAL_EQUAL
};

struct Win {
    Win(Mark m, WinType t, int v) : mark(m), type(t), value(v) {}

    Win() : type(WinType::NONE) {}

    bool operator==(const Win &other) const {
        if (type != other.type)
            return false;

        if (type == WinType::NONE)
            return true;

        return (mark == other.mark) && (value == other.value);
    }

    Mark mark;
    WinType type;
    int value;
};

struct Position {
    size_t row;
    size_t column;
    Mark mark;
};

class Board {
public:
    using Cells = std::vector<std::vector<Mark>>;

    Board(unsigned long size);

    void putMark(int row, int column, Mark mark);

    void putRow(int num, std::vector<Mark> marks);

    const Cells &getCells() const;

    Mark getCell(unsigned long row, unsigned long column) const;

    Win isWin(Mark mark) const;

    nlohmann::json toJson() const;
private:
    Cells _cells;
    size_t _size;
};


#endif //TIC_TAC_TOE_BOARD_H
