#include "gui.h"

int player = 1;
int playerA = 0, playerB = 0;
bool player_is_red = false;

void string2tchar(TCHAR* buf, string& src) {
#ifdef UNICODE  
	_stprintf_s(buf, MAX_PATH, _T("%S"), src.c_str());  
#else  
	_stprintf_s(buf, MAX_PATH, _T("%s"), src.c_str());
#endif
}

GUI::GUI() {
	this->states = -1;
}

void GUI::init_environment() {
	IMAGE imgBG;
	loadimage(&imgBG, _T("./picture/ewn_p1_gui.jpg"), WIDTH, HEIGHT);
	putimage(0, 0, &imgBG);

	setfillcolor(WHITE);
	//block 90*90
	for (int x = 10; x < 450; x += 90)
		for (int y = 10; y < 450; y += 90)
			fillrectangle(x + 1, y + 1, x + 90 - 1, y + 90 - 1);
	this->print_info();
	line(700, 0, 700, 470);
}

int GUI::begin_gui() {
	IMAGE imgBG;
	loadimage(&imgBG, _T("./picture/ewn_p1_gui.jpg"), WIDTH, HEIGHT);
	putimage(0, 0, &imgBG);

	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(60, 0, _T("Elephant"));
	outtextxy(200, 70, _T("Please choose a mode")); // 406 x 60

	settextstyle(50, 0, _T("Berlin Sans FB"));
	outtextxy(250, 170, _T("0)Player vs. Player"));	// 350 x 50
	outtextxy(250, 250, _T("1)AI vs. Player"));	// 257 x 50
	outtextxy(250, 330, _T("2)AI vs. AI"));	// 187 x 50

	ExMessage msg;
	int mode;
	bool b1 = false, b2 = false, b3 = false;
	while (1) {
		if (peekmessage(&msg, EX_MOUSE) && msg.message == WM_LBUTTONDOWN) {
			if (msg.x >= 250 && msg.x <= 600 && msg.y >= 170 && msg.y <= 220) {
				if (b1) break;

				settextcolor(YELLOW);
				outtextxy(250, 170, _T("0)Player vs. Player"));
				settextcolor(BLACK);
				outtextxy(250, 250, _T("1)AI vs. Player"));
				outtextxy(250, 330, _T("2)AI vs. AI"));

				mode = 0;
				b1 = true, b2 = false, b3 = false;
			}
			else if (msg.x >= 250 && msg.x <= 507 && msg.y >= 250 && msg.y <= 300) {
				if (b2) break;

				settextcolor(YELLOW);
				outtextxy(250, 250, _T("1)AI vs. Player"));
				settextcolor(BLACK);
				outtextxy(250, 170, _T("0)Player vs. Player"));
				outtextxy(250, 330, _T("2)AI vs. AI"));

				mode = 1;
				b2 = true, b1 = false, b3 = false;
			}
			else if (msg.x >= 250 && msg.x <= 437 && msg.y >= 330 && msg.y <= 380) {
				if (b3) break;

				settextcolor(YELLOW);
				outtextxy(250, 330, _T("2)AI vs. AI"));
				settextcolor(BLACK);
				outtextxy(250, 170, _T("0)Player vs. Player"));
				outtextxy(250, 250, _T("1)AI vs. Player"));

				mode = 2;
				b3 = true, b1 = false, b2 = false;
			}
		}
	}
	if (mode == 0) cout << "Player vs. Player\n";
	else if (mode == 1) cout << "AI vs. Player\n";
	else cout << "AI vs. AI\n";
	return mode;
}

int GUI::select_AI() {
	IMAGE imgBG;
	loadimage(&imgBG, _T("./picture/ewn_p1_gui.jpg"), WIDTH, HEIGHT);
	putimage(0, 0, &imgBG);

	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(60, 0, _T("Elephant"));
	outtextxy(100, 70, _T("Choose an AI"));	// 243 x 60

	if (player == 1)
		outtextxy(520, 70, _T("(Player 1):")); // 186 x 60
	else
		outtextxy(520, 70, _T("(Player 2):")); // 186 x 60

	player = -player;
	settextstyle(50, 0, _T("Berlin Sans FB"));
	outtextxy(150, 170, _T("0)MCTS")); // 149 x 50
	outtextxy(150, 250, _T("1)Random")); // 190 x 50
	outtextxy(150, 330, _T("2)Model"));	// 152 x 50
	
	ExMessage msg;
	int ai;
	bool b1 = false, b2 = false, b3 = false;
	while (1) {
		if (peekmessage(&msg, EX_MOUSE) && msg.message == WM_LBUTTONDOWN) {
			if (msg.x >= 150 && msg.x <= 299 && msg.y >= 170 && msg.y <= 220) {
				if (b1) break;

				settextcolor(YELLOW);
				outtextxy(150, 170, _T("0)MCTS"));
				settextcolor(BLACK);
				outtextxy(150, 250, _T("1)Random"));
				outtextxy(150, 330, _T("2)Model"));

				if (!playerA) playerA = 1;
				else playerB = 1;
				ai = 0;
				b1 = true, b2 = false, b3 = false;
			}
			else if (msg.x >= 150 && msg.x <= 340 && msg.y >= 250 && msg.y <= 300) {
				if (b2) break;

				settextcolor(YELLOW);
				outtextxy(150, 250, _T("1)Random"));
				settextcolor(BLACK);
				outtextxy(150, 170, _T("0)MCTS"));
				outtextxy(150, 330, _T("2)Model"));

				if (!playerA) playerA = 2;
				else playerB = 2;
				ai = 1;
				b2 = true, b1 = false, b3 = false;
			}
			else if (msg.x >= 150 && msg.x <= 302 && msg.y >= 330 && msg.y <= 380) {
				if (b3) break;

				settextcolor(YELLOW);
				outtextxy(150, 330, _T("2)Model"));
				settextcolor(BLACK);
				outtextxy(150, 170, _T("0)MCTS"));
				outtextxy(150, 250, _T("1)Random"));

				if (!playerA) playerA = 3;
				else playerB = 3;
				ai = 2;
				b3 = true, b1 = false, b2 = false;
			}
		}
	}
	if (playerA) {
		if (playerA == 1) cout << "MCTS";
		else if (playerA == 2) cout << "Random";
		else cout << "Model";
	}
	if (playerB) {
		if (playerB == 1) cout << " MCTS\n";
		else if (playerB == 2) cout << " Random\n";
		else cout << " Model\n";
	}
	cout << '\n';
	return ai;
}

bool GUI::select_side() {
	IMAGE imgBG;
	loadimage(&imgBG, _T("./picture/ewn_p1_gui.jpg"), WIDTH, HEIGHT);
	putimage(0, 0, &imgBG);

	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(60, 0, _T("Elephant"));
	outtextxy(150, 70, _T("Please choose which side"));	// 449 x 60

	settextstyle(50, 0, _T("Berlin Sans FB"));
	outtextxy(280, 200, _T("The red side(first)"));	// 318 x 50
	outtextxy(280, 280, _T("The blue side")); // 240 x 50

	ExMessage msg;
	bool b1 = false, b2 = false;
	while (1) {
		if (peekmessage(&msg, EX_MOUSE) && msg.message == WM_LBUTTONDOWN) {
			if (msg.x >= 280 && msg.x <= 598 && msg.y >= 200 && msg.y <= 250) {
				if (b1) break;

				settextcolor(YELLOW);
				outtextxy(280, 200, _T("The red side(first)"));
				settextcolor(BLACK);
				outtextxy(280, 280, _T("The blue side"));

				b1 = true, b2 = false;
			}
			else if (msg.x >= 280 && msg.x <= 520 && msg.y >= 280 && msg.y <= 330) {
				if (b2) break;

				settextcolor(YELLOW);
				outtextxy(280, 280, _T("The blue side"));
				settextcolor(BLACK);
				outtextxy(280, 200, _T("The red side(first)"));

				b2 = true, b1 = false;
			}
		}
	}
	if (b1) cout << "Player is the red side(first)\n";
	else cout << "Player is the blue side\n";
	cout << '\n';
	player_is_red = b1;
	return b1;
}

int GUI::select_chs_index(const Game& game, int dice) {
	int chess, movable_chs_cnt = (game.movable_chs_list[0] == game.movable_chs_list[1]) ? 1 : 2;
	int x, y, x_selected = -2, y_selected = -2;
	BitBoard tmp;
	ExMessage msg;
	this->draw_movable_cnt_color(game, dice);

	while (1) {
		if (peekmessage(&msg, EX_MOUSE) && msg.message == WM_LBUTTONDOWN) {
			this->rstboard_1(game);
			this->draw_movable_cnt_color(game, dice);

			x = (msg.x - 10) / 90, y = (msg.y - 10) / 90;
			if (x >= 0 && x < 5 && y >= 0 && y < 5) {
				tmp = game.chessPosition.fullMask[y * 5 + x];
				if (game.chessPosition.all & tmp) {
					chess = game.get_chess_index(tmp) + 1;
					cout << chess << '\n';
					this->draw_chess(x * 90 + 55, y * 90 + 55, chess + '0', -1);
					this->states = y * 5 + x;

					if (x == x_selected && y == y_selected) {
						for (int i = 0; i < movable_chs_cnt; i++) {
							if (tmp == game.movable_chs_list[i]) {
								this->draw_selected_chess(chess);
								return chess;
							}
						}
					}
				}
			}
			x_selected = x, y_selected = y;
		}
	}
}

int GUI::select_chs_direction(const Game& game) {
	int dir[3] = { -1, -5, -6 }, state_dir;
	int x, y, x_selected = -2, y_selected = -2;
	if (!game.side)
		dir[0] = 1, dir[1] = 5, dir[2] = 6;
	ExMessage msg;

	while (1) {
		if (peekmessage(&msg, EX_MOUSE) && msg.message == WM_LBUTTONDOWN) {
			x = (msg.x - 10) / 90, y = (msg.y - 10) / 90;
			if (x >= 0 && x < 5 && y >= 0 && y < 5) {
				state_dir = y * 5 + x;

				if (x == x_selected && y == y_selected) {
					for (int i = 0; i < 3; i++) {
						if (state_dir == this->states + dir[i]) {
							this->draw_selected_direction(game.side, i);
							cout << i << '\n';
							return i;
						}
					}
				}
			}
			x_selected = x, y_selected = y;
		}
	}
}

void GUI::draw_chess(int x, int y, char chess, int turn) {
	settextcolor(WHITE);
	if (turn == -1) {
		setfillcolor(YELLOW); setbkcolor(YELLOW);
	}
	else if (turn == 0) {
		setfillcolor(RED); setbkcolor(RED);
	}
	else {
		setfillcolor(BLUE); setbkcolor(BLUE);
	}
	fillcircle(x, y, 40);
	settextstyle(40, 40, _T("Consolas"));
	outtextxy(x - 20, y - 20, chess);
}

void GUI::draw_movable_cnt_color(const Game& game, int dice) {
	int movable_chs_cnt = (game.movable_chs_list[0] == game.movable_chs_list[1]) ? 1 : 2;

	for (int i = 0; i < movable_chs_cnt; i++) {
		bool out = false;
		BitBoard tmp = game.movable_chs_list[i];
		char c = game.get_chess_index(tmp) + '1';

		for (int x = 0; x < 5; x++) {
			for (int y = 0; y < 5; y++) {
				if (tmp == game.chessPosition.fullMask[x * 5 + y]) {
					setlinecolor(YELLOW);
					setlinestyle(PS_SOLID, 6);
					this->draw_chess(y * 90 + 55, x * 90 + 55, c, game.side);
					setlinestyle(PS_SOLID, 1);
					setlinecolor(WHITE);
					out = true;
					break;
				}
			}
			if (out) break;
		}
	}
}

void GUI::draw_selected_chess(int chess) {
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	settextstyle(35, 0, _T("Consolas"));
	outtextxy(700 + 75, 30 + 157, chess + '0');
}

void GUI::draw_selected_direction(bool side, int direct) {
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	settextstyle(35, 0, _T("Consolas"));

	string dir_str[3] = {"Left", "Up", "Left-up"};
	if (!side)
		dir_str[0] = "Right", dir_str[1] = "Down", dir_str[2] = "Right-down";

	TCHAR buf[11];
	string2tchar(buf, dir_str[direct]);
	outtextxy(740 + int(-7.5 * dir_str[direct].size() + 55), 250, buf);
}

void GUI::rstboard(const ChessPosition& chp) {
	BeginBatchDraw();
	init_environment();

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (chp.all & chp.fullMask[i * 5 + j]) {
				for (int k = 0; k < 6; k++) {
					if (chp.fullMask[i * 5 + j] & chp.chessRed[k]) {
						this->draw_chess(j * 90 + 55, i * 90 + 55, k + '1', 0);
						break;
					}
					else if (chp.fullMask[i * 5 + j] & chp.chessBlue[k]) {
						this->draw_chess(j * 90 + 55, i * 90 + 55, k + '1', 1);
						break;
					}
				}
			}
		}
	}
	EndBatchDraw();
}

void GUI::rstboard_1(const Game& game) {
	BeginBatchDraw();
	setfillcolor(WHITE);
	//block 90*90
	for (int x = 10; x < 450; x += 90)
		for (int y = 10; y < 450; y += 90)
			fillrectangle(x + 1, y + 1, x + 90 - 1, y + 90 - 1);
	line(700, 0, 700, 470);
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (game.chessPosition.all & game.chessPosition.fullMask[i * 5 + j]) {
				for (int k = 0; k < 6; k++) {
					if (game.chessPosition.fullMask[i * 5 + j] & game.chessPosition.chessRed[k]) {
						this->draw_chess(j * 90 + 55, i * 90 + 55, k + '1', 0);
						break;
					}
					else if (game.chessPosition.fullMask[i * 5 + j] & game.chessPosition.chessBlue[k]) {
						this->draw_chess(j * 90 + 55, i * 90 + 55, k + '1', 1);
						break;
					}
				}
			}
		}
	}
	EndBatchDraw();
}

void GUI::print_info() {
	int zeroX = 470, zeroY = 10;
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	settextstyle(50, 0, _T("Comic Sans MS"));
	outtextxy(zeroX + 40, zeroY, _T("EinStein"));

	settextstyle(20, 0, _T("Times New Roman"));
	outtextxy(zeroX + 10, zeroY + 60, _T("Round:"));
	outtextxy(zeroX + 10, zeroY + 90, _T("Dice:"));
	outtextxy(zeroX + 10, zeroY + 120, _T("Movable chess:"));
	outtextxy(zeroX + 10, zeroY + 170, _T("Next move win rate:"));

	int tmpX = 700, tmpY = 10;
	outtextxy(tmpX + 60, tmpY + 20, _T("Time cost"));

	string red = "The red side: ", blue = "The blue side: ";

	if (player_is_red) {
		red += "player";
		if (playerA == 0) blue += "player";
		else if (playerA == 1) blue += "MCTS";
		else if (playerA == 2) blue += "Random";
		else if (playerA == 3) blue += "Model";
	}
	else {
		if (playerA == 0) red += "player";
		else if (playerA == 1) red += "MCTS";
		else if (playerA == 2) red += "Random";
		else if (playerA == 3) red += "Model";

		if (playerB == 0) blue += "player";
		else if (playerB == 1) blue += "MCTS";
		else if (playerB == 2) blue += "Random";
		else if (playerB == 3) blue += "Model";
	}

	TCHAR buf1[25], buf2[25];
	string2tchar(buf1, red);
	string2tchar(buf2, blue);
	outtextxy(tmpX + 20, tmpY + 60, buf1);
	outtextxy(tmpX + 20, tmpY + 110, buf2);

	outtextxy(tmpX + 20, tmpY + 160, _T("Selected chess"));
	outtextxy(tmpX + 20, tmpY + 210, _T("Selected direction"));
}

void GUI::round_txt(int round) {
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	settextstyle(25, 0, _T("Consolas"));

	string round_str = to_string(round);
	TCHAR buf[5];
	string2tchar(buf, round_str);
	outtextxy(550, 66, buf);
}

void GUI::dice_txt(bool side, int dice) {
	if (!side) settextcolor(RED);
	else settextcolor(BLUE);
	setbkmode(TRANSPARENT);
	settextstyle(35, 0, _T("Consolas"));
	outtextxy(530, 92, dice + '0');
}

void GUI::movabledice_txt(bool side, int chess1, int chess2) {
	if (!side) settextcolor(RED);
	else settextcolor(BLUE);
	setbkmode(TRANSPARENT);
	settextstyle(35, 0, _T("Consolas"));
	outtextxy(610, 122, chess1 + '1');
	if (chess1 != chess2) outtextxy(640, 122, chess2 + '1');
}

void GUI::prob_txt(bool side, int mov_chs, int direct, float prob, int chs_index) {
	int zeroX = 470, y = (mov_chs == 0) ? 220 : 330, space = 25 * (direct + 1);

	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	settextstyle(25, 0, _T("Consolas"));

	string str1 = "Chess:" + to_string(chs_index + 1);
	TCHAR buf1[8];
	string2tchar(buf1, str1);
	outtextxy(zeroX, y, buf1);

	string dir_str[3] = { "L", "U", "LU" };
	if (!side)
		dir_str[0] = "R", dir_str[1] = "D", dir_str[2] = "RD";
	string str2 = "DIR:" + dir_str[direct];
	TCHAR buf2[7];
	string2tchar(buf2, str2);
	outtextxy(zeroX, y + space, buf2);

	string str3 = "PR:" + to_string(prob);
	TCHAR buf3[12];
	string2tchar(buf3, str3);
	outtextxy(zeroX + 90, y + space, buf3);
}

void GUI::clock_txt(bool side, double clock_p) {
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	settextstyle(30, 0, _T("Consolas"));

	string clock_str = to_string(clock_p);
	TCHAR buf[15];
	string2tchar(buf, clock_str);

	if (!side) outtextxy(750, 90, buf);
	else outtextxy(750, 140, buf);
}

void GUI::result_txt(string result) {
	IMAGE imgBG;
	loadimage(&imgBG, _T("./picture/ewn_p1_gui.jpg"), WIDTH, HEIGHT);
	putimage(0, 0, &imgBG);

	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	settextstyle(40, 0, _T("Times New Roman"));

	TCHAR buf[30];
	string2tchar(buf, result);
	outtextxy(230, 170, buf);
}

void GUI::win_rate_txt(string win_rate) {
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	settextstyle(40, 0, _T("Times New Roman"));

	TCHAR buf[55];
	string2tchar(buf, win_rate);
	outtextxy(120, 250, buf);
}