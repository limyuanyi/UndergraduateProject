#include "mcts.h"
#include "gui.h"

clock_t s, f;

MCTS::MCTS() {
	this->selection_ptr = new UCT(eps);
}

MCTS::~MCTS() {
	delete this->selection_ptr;
}

Movement MCTS::AI_move(Game& cur_game, int dice, int select) {
	this->max_iterations = 100;
	this->ai_side = cur_game.side;

	float best_val = -1e9, child_val;
	int win = (!this->ai_side) ? 1 : 2, movable_chs_cnt = cur_game.count_movable_chs(dice), game_status;
	int movable_chs_index_list[2] = { cur_game.get_chess_index(cur_game.movable_chs_list[0]), cur_game.get_chess_index(cur_game.movable_chs_list[1]) };
	bool end = false;
	Movement answer;
	GUI g;

	g.draw_movable_cnt_color(cur_game, dice);
	g.movabledice_txt(cur_game.side, movable_chs_index_list[0], movable_chs_index_list[1]);
	cout << "movable chess: " << movable_chs_index_list[0] + 1;
	if (movable_chs_index_list[0] != movable_chs_index_list[1]) cout << ", " << movable_chs_index_list[1] + 1;
	cout << '\n';

	for (int i = 0; i < movable_chs_cnt; i++) {
		for (int direct = 0; direct < 3; direct++) {
			Movement mvmt(movable_chs_index_list[i], direct);
			if (cur_game.check_in_board(mvmt)) {
				Game child_game = cur_game;
				game_status = child_game.update_game_status(mvmt);
				child_game.switch_player();

				// check if the game ends
				if (game_status == win) {
					answer = mvmt;
					end = true;
					break;
				}
				// lose the game
				else if (game_status != 0) continue;
				else {
					child_val = this->run(child_game, select);
					g.prob_txt(cur_game.side, i, direct, child_val, movable_chs_index_list[i]);
					// update the best value and the best movement
					if (child_val >= best_val) {
						best_val = child_val;
						answer = mvmt;
					}
				}
			}
		}
		if (end) break;
	}
	g.draw_selected_chess(answer.first + 1);
	g.draw_selected_direction(cur_game.side, answer.second);
	cout << "move: (" << answer.first + 1 << ", " << answer.second << ")\n";
	return answer;
}

// mcts main
float MCTS::run(const Game& cur_game, int select) {
	// initialize root Tree_node with current state
	Tree_node* root_node_ptr = new Tree_node(cur_game, select);
	int iteration = 0, simu_num = 0;
	s = clock();

	while (iteration++ < this->max_iterations) {
		//cout << "----------------------\n";
		//cout << "iteration : " << iteration << '\n';

		// 1. select
		// Start from the root, digging down until finding an unvisited node
		Tree_node* node_ptr = root_node_ptr;
		int best_child_index, level = 1;
		// The root has been visited
		if (root_node_ptr->is_visit()) {
			while (!node_ptr->is_terminate()) {
				if (select)
					best_child_index = this->selection_ptr->select_children_list_index_model(node_ptr);
				else
					best_child_index = this->selection_ptr->select_children_list_index(node_ptr);
				node_ptr = node_ptr->get_child_ptr(best_child_index);
				level++;
				//node_ptr->game.print_board();
				// The next child pointer hasn't been visited
				if (node_ptr->get_child_ptr(0) == NULL) break;
			}
		}
		//cout << "level : " << level << '\n';
		int win_reward, legal_child_number;
		if (node_ptr->is_terminate()) {
			//cout << "the node is terminal\n";
			win_reward = node_ptr->get_game_status() == ((!this->ai_side) ? 1 : 2);
			simu_num++;

			// 4. back propagation
			while (true) {
				node_ptr->update(win_reward);
				if (node_ptr->get_parent_ptr() == NULL) break;
				node_ptr = node_ptr->get_parent_ptr();
			}
		}
		else {
			// 2. expand by adding a single child (if not terminal or not fully expanded)
			if (select) legal_child_number = node_ptr->new_child_nodes_model();
			else legal_child_number = node_ptr->new_child_nodes();
			//cout << "legal child number : " << legal_child_number << '\n';

			// 3. simulate
			for (int k = 0; k < legal_child_number; k++) {
				int win = 0, visit = 0;
				Tree_node* child_ptr;
				for (int l = 0; l < 100; l++) {
					//child_ptr->game.print_board();
					child_ptr = node_ptr->get_child_ptr(k);
					if (select) win_reward = this->simulation_model(child_ptr->game);
					else win_reward = this->simulation(child_ptr->game);

					// gather results of some simulation and update
					win += win_reward;
					visit++;
					//cout << win << " " << visit << '\n';
					simu_num++;

					if (l < 99) continue;
					// 4. back propagation
					while (true) {
						child_ptr->update(win, visit);
						if (child_ptr->get_parent_ptr() == NULL) break;
						child_ptr = child_ptr->get_parent_ptr();
					}
				}
			}
		}
	}

	f = clock();
	//cout << "time cost : " << (f - s) / (double)(CLOCKS_PER_SEC) << " seconds\n" << "simu:" << simu_num << '\n';
	float result = (float)(root_node_ptr->get_win_count()) / root_node_ptr->get_visit_count();
	this->recursive_delete_tree_node(root_node_ptr);
	return result;
}

int MCTS::simulation(const Game& cur_game) {
	Game simu_game = cur_game;
	int game_status = 0, ai_win = (!this->ai_side) ? 1 :2;

	while (game_status == 0) {
		//simu_game.print_board();
		Movement available_mvmt_list[18];
		int available_mvmt_cnt = 0;
		// find all available move (6 dice indices, 3 directions)
		for (int chs_index = 0; chs_index < 6; chs_index++) {
			if (simu_game.cur_chs_ptr[chs_index] > 0) {
				for (int direct = 0; direct < 3; direct++) {
					Movement tmp_mvmt(chs_index, direct);
					if (simu_game.check_in_board(tmp_mvmt))
						available_mvmt_list[available_mvmt_cnt++] = tmp_mvmt;
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

void MCTS::recursive_delete_tree_node(Tree_node* node_ptr) {
	for (int i = 0; i < 18; i++)
		if (node_ptr->get_child_ptr(i))
			recursive_delete_tree_node(node_ptr->get_child_ptr(i));
	delete node_ptr;
}