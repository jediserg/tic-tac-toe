//
// Created by serg on 11/5/16.
//

#ifndef TIC_TAC_TOE_BOARD_H
#define TIC_TAC_TOE_BOARD_H

#include <cstddef>
#include <boost/numeric/ublas/matrix.hpp>

enum class Mark {
    UNMARKED, MARK1, MARK2
};

struct Position {
    size_t row;
    size_t column;
    Mark mark;
};

class Board {
public:
    using Cells = boost::numeric::ublas::matrix<Mark>;

    Board(size_t size);

    void putMark(size_t row, size_t column, Mark mark);

    const Cells &getCells() const;

private:
    Cells _cells;
};


#endif //TIC_TAC_TOE_BOARD_H
