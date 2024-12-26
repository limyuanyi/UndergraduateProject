#pragma once
#ifndef GUI_H
#define GUI_H

#include "game.h"

#define WIDTH 900
#define HEIGHT 470

class GUI {
private:
	int states;

public:
	GUI();

	void init_environment();
	int begin_gui();

	int select_AI();
	bool select_side();
	int select_chs_index(const Game& game, int dice);
	int select_chs_direction(const Game& game);

	void draw_chess(int x, int y, char chess, int turn);
	void draw_movable_cnt_color(const Game& game, int dice);
	void draw_selected_chess(int chess);
	void draw_selected_direction(bool side, int direct);

	void rstboard(const ChessPosition& chp);
	void rstboard_1(const Game& game);

	void print_info();
	void round_txt(int round);
	void dice_txt(bool side, int dice);
	void movabledice_txt(bool side, int chess1, int chess2);
	void prob_txt(bool side, int mov_chs, int direct, float prob, int chs_index);
	void clock_txt(bool side, double clock_p);
	void result_txt(string result);
	void win_rate_txt(string win_rate);
};

#endif 