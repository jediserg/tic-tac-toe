//
// Created by serg on 11/5/16.
//

#ifndef TIC_TAC_TOE_BOARD_H
#define TIC_TAC_TOE_BOARD_H

#include <cstddef>
#include <boost/numeric/ublas/matrix.hpp>

enum class Mark {
    UNMARKED = 0, X = 1, O = 2
};

enum class WinType {
    NONE, ROW_EQUAL, COLUMN_EQUAL, DIAGONAL_EQUAL
};

struct Win {
    Win(Mark m, WinType t, unsigned long v) : mark(m), type(t), value(v) {}

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
    unsigned long value;
};

struct Position {
    size_t row;
    size_t column;
    Mark mark;
};

class Board {
public:
    using Cells = boost::numeric::ublas::matrix<Mark>;

    Board(unsigned long size);

    void putMark(unsigned long row, unsigned long column, Mark mark);

    void putRow(unsigned long num, std::vector<Mark> marks);

    const Cells &getCells() const;

    Mark getCell(unsigned long row, unsigned long column) const;

    Win isWin(Mark mark) const;
private:
    Cells _cells;
};


#endif //TIC_TAC_TOE_BOARD_H
