#include "game.h"
#include "gui.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

ChessPosition::ChessPosition() {
	memset(this->chessRed, 0, sizeof(this->chessRed));
	memset(this->chessBlue, 0, sizeof(this->chessBlue));
	this->all = 0b1110011000100010001100111;
	BitBoard tmp = 1;
	for (int i = 0; i < 25; i++) {
		this->fullMask[i] = tmp;
		tmp = tmp << 1;
	}
}

ChessPosition::ChessPosition(const ChessPosition& chesspos) {
	for (int i = 0; i < 6; i++) {
		this->chessRed[i] = chesspos.chessRed[i];
		this->chessBlue[i] = chesspos.chessBlue[i];
	}
	this->all = chesspos.all;
}

void ChessPosition::operator=(const ChessPosition& chesspos) {
	for (int i = 0; i < 6; i++) {
		this->chessRed[i] = chesspos.chessRed[i];
		this->chessBlue[i] = chesspos.chessBlue[i];
	}
	this->all = chesspos.all;
}

Game::Game() {
	ios_base::sync_with_stdio(0);
	cin.tie(nullptr);

	srand(time(NULL) + _getpid());
	this->side = 0;

	vector<int> chs_red{ 0, 1, 2, 3, 4, 5 };
	vector<int> chs_blue{ 0, 1, 2, 3, 4, 5 };
	// mt19937 is a random generator based on Mersenne twister algorithm
	mt19937 gen(random_device{}());
	
	shuffle(chs_red.begin(), chs_red.end(), gen);
	this->chessPosition.chessRed[chs_red[0]] = this->chessPosition.fullMask[0];
	this->chessPosition.chessRed[chs_red[1]] = this->chessPosition.fullMask[1];
	this->chessPosition.chessRed[chs_red[2]] = this->chessPosition.fullMask[2];
	this->chessPosition.chessRed[chs_red[3]] = this->chessPosition.fullMask[5];
	this->chessPosition.chessRed[chs_red[4]] = this->chessPosition.fullMask[6];
	this->chessPosition.chessRed[chs_red[5]] = this->chessPosition.fullMask[10];

	shuffle(chs_blue.begin(), chs_blue.end(), gen);
	this->chessPosition.chessBlue[chs_blue[0]] = this->chessPosition.fullMask[24];
	this->chessPosition.chessBlue[chs_blue[1]] = this->chessPosition.fullMask[23];
	this->chessPosition.chessBlue[chs_blue[2]] = this->chessPosition.fullMask[22];
	this->chessPosition.chessBlue[chs_blue[3]] = this->chessPosition.fullMask[19];
	this->chessPosition.chessBlue[chs_blue[4]] = this->chessPosition.fullMask[18];
	this->chessPosition.chessBlue[chs_blue[5]] = this->chessPosition.fullMask[14];

	for (int i = 0; i < 2; i++)
		this->movable_chs_list[i] = 0;
	this->cur_chs_ptr = this->chessPosition.chessRed;
}

Game::Game(const Game& game) {
	this->side = game.side;
	this->chessPosition = game.chessPosition;
	for (int i = 0; i < 2; i++)
		this->movable_chs_list[i] = game.movable_chs_list[i];
	this->cur_chs_ptr = (!this->side) ? this->chessPosition.chessRed : this->chessPosition.chessBlue;
}

void Game::operator=(const Game& game) {
	this->side = game.side;
	this->chessPosition = game.chessPosition;
	for (int i = 0; i < 2; i++)
		this->movable_chs_list[i] = game.movable_chs_list[i];
	this->cur_chs_ptr = (!this->side) ? this->chessPosition.chessRed : this->chessPosition.chessBlue;
}

int Game::roll_dice() {
	mt19937 gen(random_device{}());
	uniform_int_distribution uni_dis(0, 5);
	return uni_dis(gen);
}

void Game::switch_player() {
	this->cur_chs_ptr = (!this->side) ? this->chessPosition.chessBlue : this->chessPosition.chessRed;
	this->side = !this->side;
}

int Game::count_movable_chs(const int& dice) {
	for (int i = 0; i < 2; i++)
		this->movable_chs_list[i] = 0;
	if (this->cur_chs_ptr[dice] != 0) {
		for (int i = 0; i < 2; i++)
			this->movable_chs_list[i] = this->cur_chs_ptr[dice];
		return 1;
	}
	else {
		int movable_chs_cnt = 2;
		for (int i = dice - 1; i >= 0; i--) {
			if (this->cur_chs_ptr[i] != 0) {
				this->movable_chs_list[0] = this->cur_chs_ptr[i];
				break;
			}
		}
		for (int i = dice + 1; i < 6; i++) {
			if (this->cur_chs_ptr[i] != 0) {
				this->movable_chs_list[1] = this->cur_chs_ptr[i];
				break;
			}
		}
		for (int i = 0; i < 2; i++) {
			if (this->movable_chs_list[i] == 0) {
				this->movable_chs_list[i] = this->movable_chs_list[1 - i];
				movable_chs_cnt--;
			}
		}
		return movable_chs_cnt;
	}
}

int Game::get_chess_index(BitBoard chs) const {
	for (int i = 0; i < 6; i++)
		if (this->chessPosition.chessRed[i] == chs || this->chessPosition.chessBlue[i] == chs)
			return i;
}

int Game::update_game_status(const Movement& mvmt) {
	BitBoard chs = this->cur_chs_ptr[mvmt.first], movemask;
	int direct = mvmt.second;

	if (!this->side) {
		if (direct == 0) movemask = chs << 1;
		else if (direct == 1) movemask = chs << 5;
		else if (direct == 2) movemask = chs << 6;
	}
	else {
		if (direct == 0) movemask = chs >> 1;
		else if (direct == 1) movemask = chs >> 5;
		else if (direct == 2) movemask = chs >> 6;		
	}

	for (int i = 0; i < 6; i++) {
		if (this->chessPosition.chessRed[i] == movemask) this->chessPosition.chessRed[i] = 0;
		if (this->chessPosition.chessBlue[i] == movemask) this->chessPosition.chessBlue[i] = 0;
	}
	this->chessPosition.all ^= chs;
	this->chessPosition.all |= movemask;
	this->cur_chs_ptr[mvmt.first] = movemask;

	// return 0(game continues), 1(red wins), 2(blue wins)
	// Check if any player arrives the end-game position
	for (int i = 0; i < 6; i++) {
		if (this->chessPosition.chessRed[i] & this->chessPosition.fullMask[24]) return 1;
		if (this->chessPosition.chessBlue[i] & this->chessPosition.fullMask[0]) return 2;
	}

	// Check if the other player is killed the game.
	bool BlueAllDie = true, RedAllDie = true;
	for (int i = 0; i < 6; i++) {
		if (this->chessPosition.chessRed[i] != 0) RedAllDie = false;
		if (this->chessPosition.chessBlue[i] != 0) BlueAllDie = false;
	}
	if (BlueAllDie) return 1;
	if (RedAllDie) return 2;
	return 0;
}

bool Game::check_in_board(const Movement& mvmt) {
	BitBoard chs = this->cur_chs_ptr[mvmt.first];
	int direct = mvmt.second;

	if (!this->side) {
		if (direct == 0) return ((chs & 0b1000010000100001000010000) == 0);
		else if (direct == 1) return ((chs & 0b1111100000000000000000000) == 0);
		else if (direct == 2) return ((chs & 0b1111110000100001000010000) == 0);
	}
	else {
		if (direct == 0) return ((chs & 0b0000100001000010000100001) == 0);
		else if (direct == 1) return ((chs & 0b0000000000000000000011111) == 0);
		else if (direct == 2) return ((chs & 0b0000100001000010000111111) == 0);
	}
}

bool Game::is_winning_move(const Movement& mvmt) {
	BitBoard chs = this->cur_chs_ptr[mvmt.first], newplace = 0;
	int direct = mvmt.second;

	if (!this->side) {
		if (direct == 0) newplace = chs << 1;
		else if (direct == 1) newplace = chs << 5;
		else if (direct == 2) newplace = chs << 6;
	}
	else {
		if (direct == 0) newplace = chs >> 1;
		else if (direct == 1) newplace = chs >> 5;
		else if (direct == 2) newplace = chs >> 6;
	}
	if (!this->side) return (newplace & this->chessPosition.fullMask[24]);
	else return (newplace & this->chessPosition.fullMask[0]);
}

void Game::print_board() {
	cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
	cout << "The current board:\n";

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			BitBoard pos = this->chessPosition.fullMask[i * 5 + j];
			if (this->chessPosition.all & pos) {
				for (int k = 0; k < 6; k++) {
					if (this->chessPosition.chessRed[k] & pos) {
						cout << ANSI_COLOR_RED << k + 1 << ' ' << ANSI_COLOR_RESET;
						break;
					}
					else if (this->chessPosition.chessBlue[k] & pos) {
						cout << ANSI_COLOR_BLUE << k + 1 << ' ' << ANSI_COLOR_RESET;
						break;
					}
				}
			}
			else cout << 0 << ' ';
		}
		cout << '\n';
	}
	cout << "\n====================================" << endl;
	GUI g;
	g.rstboard(this->chessPosition);
}

void Game::print_movable(const int& dice) {
	int num = this->count_movable_chs(dice);
	for (int i = 0; i < num; i++)
		for (int j = 0; j < 6; j++)
			if (this->cur_chs_ptr[j] == this->movable_chs_list[i])
				cout << j + 1 << ' ';
	cout << '\n';
}

void Game::print_game_status() {
	if (!this->side) cout << "red round\n";
	else cout << "blue round\n";
	cout << "all:" << (bitset<25>)this->chessPosition.all << '\n';
	for (int i = 0; i < 6; i++)
		cout << "blue" << i + 1 << ':' << (bitset<25>)this->chessPosition.chessBlue[i] << '\n';
	for (int i = 0; i < 6; i++)
		cout << "red" << i + 1 << ':' << (bitset<25>)this->chessPosition.chessRed[i] << '\n';
	for (int i = 0; i < 6; i++)
		cout << "cur" << i + 1 << ':' << (bitset<25>)this->cur_chs_ptr[i] << '\n';
	cout << "movable1:" << (bitset<25>)this->movable_chs_list[0] << '\n';
	cout << "movable2:" << (bitset<25>)this->movable_chs_list[1] << '\n';
}