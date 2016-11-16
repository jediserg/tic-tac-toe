//
// Created by serg on 11/7/16.
//
#include <gtest/gtest.h>
#include <Board.h>

TEST(BoardTests, SimpleChecks) {
    Board board(3);

    //X 0 X
    //X 0 X
    //0 X 0

    board.putRow(0, {Mark::X, Mark::O, Mark::X});
    board.putRow(1, {Mark::X, Mark::O, Mark::X});
    board.putRow(2, {Mark::O, Mark::X, Mark::O});

    Win w = board.isWin(Mark::X);
    EXPECT_EQ(WinType::NONE, w.type);

    w = board.isWin(Mark::O);
    EXPECT_EQ(WinType::NONE, w.type);

    EXPECT_EQ(Mark::X, board.getCell(0, 0));
    EXPECT_EQ(Mark::O, board.getCell(0, 1));
    EXPECT_EQ(Mark::X, board.getCell(0, 2));

    EXPECT_EQ(Mark::X, board.getCell(1, 0));
    EXPECT_EQ(Mark::O, board.getCell(1, 1));
    EXPECT_EQ(Mark::X, board.getCell(1, 2));

    EXPECT_EQ(Mark::O, board.getCell(2, 0));
    EXPECT_EQ(Mark::X, board.getCell(2, 1));
    EXPECT_EQ(Mark::O, board.getCell(2, 2));
}

TEST(BoardTests, WinCheckRows) {
    Board board(3);

    //X X X
    //X 0 X
    //0 X 0

    board.putRow(0, {Mark::X, Mark::X, Mark::X});
    board.putRow(1, {Mark::X, Mark::O, Mark::X});
    board.putRow(2, {Mark::O, Mark::X, Mark::O});

    EXPECT_EQ(Win(Mark::X, WinType::ROW_EQUAL, 0), board.isWin(Mark::X));

    //X 0 X
    //X X X
    //0 X 0

    board.putRow(0, {Mark::X, Mark::O, Mark::X});
    board.putRow(1, {Mark::X, Mark::X, Mark::X});
    board.putRow(2, {Mark::O, Mark::X, Mark::O});

    EXPECT_EQ(Win(Mark::X, WinType::ROW_EQUAL, 1), board.isWin(Mark::X));

    //X 0 X
    //0 0 0
    //X X X

    board.putRow(0, {Mark::X, Mark::O, Mark::X});
    board.putRow(1, {Mark::O, Mark::X, Mark::O});
    board.putRow(2, {Mark::X, Mark::X, Mark::X});

    EXPECT_EQ(Win(Mark::X, WinType::ROW_EQUAL, 2), board.isWin(Mark::X));
}

TEST(BoardTests, WinCheckColumns) {
    Board board(3);

    //X O X
    //X 0 X
    //X X 0

    board.putRow(0, {Mark::X, Mark::O, Mark::X});
    board.putRow(1, {Mark::X, Mark::O, Mark::X});
    board.putRow(2, {Mark::X, Mark::X, Mark::O});

    EXPECT_EQ(Win(Mark::X, WinType::COLUMN_EQUAL, 0), board.isWin(Mark::X));

    //0 X X
    //0 X X
    //0 X 0

    board.putRow(0, {Mark::O, Mark::X, Mark::O});
    board.putRow(1, {Mark::O, Mark::X, Mark::X});
    board.putRow(2, {Mark::X, Mark::X, Mark::O});

    EXPECT_EQ(Win(Mark::X, WinType::COLUMN_EQUAL, 1), board.isWin(Mark::X));

    //0 0 X
    //0 X X
    //X 0 X

    board.putRow(0, {Mark::O, Mark::O, Mark::X});
    board.putRow(1, {Mark::O, Mark::X, Mark::X});
    board.putRow(2, {Mark::O, Mark::O, Mark::X});

    EXPECT_EQ(Win(Mark::X, WinType::COLUMN_EQUAL, 2), board.isWin(Mark::X));
}

TEST(BoardTests, WinCheckDiagonals) {
    Board board(3);

    //X O X
    //X X O
    //O X X

    board.putRow(0, {Mark::X, Mark::O, Mark::X});
    board.putRow(1, {Mark::X, Mark::X, Mark::O});
    board.putRow(2, {Mark::O, Mark::X, Mark::X});

    EXPECT_EQ(Win(Mark::X, WinType::DIAGONAL_EQUAL, 1), board.isWin(Mark::X));

    //0 0 X
    //0 X X
    //X X 0

    board.putRow(0, {Mark::O, Mark::O, Mark::X});
    board.putRow(1, {Mark::O, Mark::X, Mark::X});
    board.putRow(2, {Mark::X, Mark::X, Mark::O});

    EXPECT_EQ(Win(Mark::X, WinType::DIAGONAL_EQUAL, 2), board.isWin(Mark::X));
}

TEST(BoardTests, WinCheckLargeBoard) {
    const int BOARD_SIZE = 999;

    {
        Board board(BOARD_SIZE);

        std::vector<Mark> marks(BOARD_SIZE, Mark::X);

        board.putRow(100, marks);

        EXPECT_EQ(Win(Mark::X, WinType::ROW_EQUAL, 100), board.isWin(Mark::X));
    }

    {
        Board board(BOARD_SIZE);

        std::vector<Mark> marks(BOARD_SIZE, Mark::O);
        marks[100] = Mark::X;

        for (int i = 0; i < BOARD_SIZE; i++) {
            board.putRow(i, marks);
        }

        EXPECT_EQ(Win(Mark::X, WinType::COLUMN_EQUAL, 100), board.isWin(Mark::X));
    }

    {
        Board board(BOARD_SIZE);

        for (unsigned long i = 0; i < BOARD_SIZE; i++) {
            board.putMark(i, i, Mark::X);
        }

        EXPECT_EQ(Win(Mark::X, WinType::DIAGONAL_EQUAL, 1), board.isWin(Mark::X));
    }

    {
        Board board(BOARD_SIZE);

        for (unsigned long i = 0; i < BOARD_SIZE; i++) {
            board.putMark(i, BOARD_SIZE - i - 1, Mark::X);
        }

        EXPECT_EQ(Win(Mark::X, WinType::DIAGONAL_EQUAL, 2), board.isWin(Mark::X));
    }
}

TEST(BoardTests, CheckJson) {
    Board board(3);

    std::string expected_json_str = R"RAW({"board":[[2,0,1],[2,1,1],[1,1,2]]})RAW";

    board.putRow(0, {Mark::O, Mark::UNMARKED, Mark::X});
    board.putRow(1, {Mark::O, Mark::X, Mark::X});
    board.putRow(2, {Mark::X, Mark::X, Mark::O});

    nlohmann::json json = board.toJson();

    EXPECT_EQ(expected_json_str, json.dump());
}
