#include "mcts.h"
#include <torch/torch.h>
#include <torch/script.h>

using namespace torch;

// need to replace path
torch::jit::script::Module jit_model = torch::jit::load("C:\\Users\\hsps6\\Desktop\\EWN\\model_ewn.pt", torch::kCPU);

int Game::get_board_FM(int i, int j) {
	return this->board_FM[i][j] - '0';
}

void Game::set_board_FM(int i, int j, int n) {
	this->board_FM[i][j] = n + '0';
}

int Game::get_board_P(int i, int j) {	
	BitBoard tmp = this->chessPosition.fullMask[i * 5 + j];
	if (this->chessPosition.all & tmp) {
		for (int k = 0; k < 6; k++) {
			if (this->chessPosition.chessRed[k] & tmp) return k + 1;
			if (this->chessPosition.chessBlue[k] & tmp) return k + 7;
		}
	}
	return 0;
}

void Tree_node::set_fm_board() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			int c = this->game.get_board_P(i, j);
			// red is 1~6, blue is 7~12; otherwise 0
			this->game.set_board_FM(i, j, c);
		}
	}
}

Tensor d_exist(int dice, int side, Tensor prev_board) {
	Tensor board = torch::zeros({ 5, 5 }, torch::kInt), non_zero_int;
	int range = (dice <= 6) ? 0 : 6, d = dice + side;

	while (d <= (6 + range) && d >= (1 + range)) {
		non_zero_int = nonzero(prev_board[d - 1]);
		if (non_zero_int.numel()) {
			board = prev_board[d - 1];
			break;
		}
		d += side;
	}
	return board;
}

Tensor board_to_FM(Tree_node* t, int dice) {
	Tensor prev_board = torch::zeros({ 16, 5, 5 }, torch::kInt);
	bool dice_exist = false;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			int k = t->game.get_board_FM(i, j);
	
			if (k > 0) prev_board[k - 1][i][j] = 1;
			else prev_board[15][i][j] = 1;

			if (k == dice && dice_exist == false) {
				prev_board[12][i][j] = 1;
				dice_exist = true;
			}
			prev_board[13][i][j] = 1;
		}
	}

	if (!dice_exist) {
		prev_board[12] += d_exist(dice, 1, prev_board);
		prev_board[12] += d_exist(dice, -1, prev_board);
	}
	return prev_board;
}

int Tree_node::new_child_nodes_model() {
	int legal_child_num = 0, range = (!this->game.side) ? 0 : 6;
	// new the legal nodes for children_ptr
	for (int chs_index = 0; chs_index < 6; chs_index++) {
		if (this->game.cur_chs_ptr[chs_index] == 0) continue;

		this->set_fm_board();
		Tensor input_tensor = board_to_FM(this, chs_index + 1 + range).to(torch::kFloat).reshape({ 1, 16, 5, 5 });
		Tensor output_tensor = jit_model.forward({ input_tensor }).toTensor();
		//Tensor softmax_output = torch::nn::functional::softmax(output_tensor, /*dim=*/ 1);

		for (int direction = 0; direction < 3; direction++) {
			Movement movement(chs_index, direction);
			if (!this->game.check_in_board(movement)) continue;

			this->children_ptr_list[legal_child_num] = new Tree_node();
			this->children_ptr_list[legal_child_num]->chess_number = chs_index;
			this->children_ptr_list[legal_child_num]->direct = direction;
			this->children_ptr_list[legal_child_num]->prob = abs(output_tensor[0][direction].item<float>());

			// cout << "chs_index: " << chs_index << ", direction: " << direction << ", prob: "<< children_ptr_list[legal_child_num]->prob << '\n';
			Tree_node* node_ptr = this->children_ptr_list[legal_child_num];
			legal_child_num++;

			// reset the values of the member
			node_ptr->game = this->game;
			node_ptr->game_status = node_ptr->game.update_game_status(movement);
			node_ptr->game.switch_player();
			node_ptr->parent_ptr = this;
		}
	}
	// reset the rest of the children_ptr to NULL
	for (int i = legal_child_num; i < 18; i++)
		this->children_ptr_list[i] = NULL;
	return legal_child_num;
}

int UCT::select_children_list_index_model(Tree_node* cur_node) {
	float child_value_list[18] = { 0 }, best_value = 0, best_prob = 0;
	int idx_best_prob = -1;
	// calculate every UCT value of the child nodes to find the max
	for (int i = 0; i < 18; i++) {
		if (cur_node->get_child_ptr(i) == NULL) break;

		Tree_node* node_ptr = cur_node->get_child_ptr(i);
		//cout << node_ptr->get_prob() << '\n';
		float exploitation = (float)node_ptr->get_win_count() / ((float)node_ptr->get_visit_count() /*+ this->eps*/);
		float exploration = this->exploration_param * sqrt(log((float)cur_node->get_visit_count() + this->t_param) / ((float)node_ptr->get_visit_count() /*+ this->eps*/));
		float uct_ = 10 * node_ptr->get_prob() / (float)node_ptr->get_visit_count();
		child_value_list[i] = exploitation + exploration + uct_;
		//cout << "UCB : " << child_value_list[i] << '\n';
		//cout << cur_node->prob << '\n';
		if (child_value_list[i] > best_value)
			best_value = child_value_list[i];

		if (node_ptr->get_prob() > best_prob) {
			best_prob = node_ptr->get_prob();
			idx_best_prob = i;
		}
	}
	//cout << "----------\n";
	//cout <<"idx_best_prob: "<< idx_best_prob << '\n';

	// gather the nodes whose UCT value approaches best_value
	int best_children_num = 0, child_index_list[18];
	for (int i = 0; i < 18; i++) {
		if (cur_node->get_child_ptr(i) == NULL) break;
		if (best_value - child_value_list[i] < this->eps) {
			child_index_list[best_children_num++] = i;
			//cout << "select_child :" << i << '\n';
		}
	}
	//cout << "----------\n";
	//cout << "best_children_num : " << best_children_num << '\n';

	// select one of best child node randomly and return its index
	return child_index_list[rand() % best_children_num];
}

int MCTS::simulation_model(const Game& cur_game) {
	Game simu_game = cur_game;
	int game_status = 0, ai_win = (!this->ai_side) ? 1 : 2;

	while (game_status == 0) {
		//simu_game.print_board();
		Movement available_mvmt_list[18];
		int available_mvmt_cnt = 0;
		// find all available moves (6 dice indices, 3 directions)
		for (int chs_index = 0; chs_index < 6; chs_index++) {
			if (simu_game.cur_chs_ptr[chs_index] > 0) {
				for (int direct = 0; direct < 3; direct++) {
					Movement tmp_mvmt(chs_index, direct);
					if (simu_game.check_in_board(tmp_mvmt)) {
						// new simulation policy
						if (simu_game.is_winning_move(tmp_mvmt)) {
							game_status = simu_game.update_game_status(tmp_mvmt);
							return (game_status == ai_win ? 1 : 0);
						}
						available_mvmt_list[available_mvmt_cnt++] = tmp_mvmt;
					}
				}
			}
		}
		// the game ends
		if (available_mvmt_cnt == 0)
			game_status = (!simu_game.side) ? 2 : 1;
		else {
			// randomly pick a move.
			Movement next_mvmt = available_mvmt_list[rand() % available_mvmt_cnt];
			game_status = simu_game.update_game_status(next_mvmt);
		}
		if (game_status == 0) simu_game.switch_player();
	}
	return (game_status == ai_win ? 1 : 0);
}