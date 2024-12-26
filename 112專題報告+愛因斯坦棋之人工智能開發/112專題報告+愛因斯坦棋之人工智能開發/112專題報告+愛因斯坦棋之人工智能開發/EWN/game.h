#pragma once
#ifndef GAME_H
#define GAME_H
#define NOMINMAX

#include <iostream>
#include <easyX.h>
#include <bitset>
#include <ctime>
#include <string>
#include <vector>
#include <random>
#include <process.h>

using namespace std;

// < the index of the currentPlayer (chs), direction >
typedef pair<int, int> Movement;
typedef int BitBoard;

class ChessPosition {
public:
	BitBoard chessRed[6];
    BitBoard chessBlue[6];
	BitBoard all;
	BitBoard fullMask[25];

    ChessPosition();
    ChessPosition(const ChessPosition& chesspos);
    void operator=(const ChessPosition& chesspos);
};

class Game {
public:
    char board_FM[5][5]; // for model, red is 1~6, blue is 7~12; otherwise is 0
    bool side; // 0 is red, 1 is blue
    ChessPosition chessPosition;
    BitBoard movable_chs_list[2];
    BitBoard* cur_chs_ptr;

    Game();
    Game(const Game& game);
    void operator=(const Game& game);

    int roll_dice();
    void switch_player();
    int count_movable_chs(const int& dice);
    int get_chess_index(BitBoard chs) const;
    // return 0(game continues), 1(red wins), 2(blue wins)
    int update_game_status(const Movement& mvmt);
    bool check_in_board(const Movement& mvmt);
    bool is_winning_move(const Movement& mvmt);

    // for model
    int get_board_FM(int i, int j);
    void set_board_FM(int i, int j, int n);
    int get_board_P(int i, int j);

    void print_board();
    void print_movable(const int& dice);
    void print_game_status();
};

#endif