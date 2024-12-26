#include "tree_node.h"

Tree_node::Tree_node() {
	this->win_count = 0;
	this->visit_count = 0;
	this->prob = 0;
	this->game_status = 0;
	this->parent_ptr = NULL;
	for (int i = 0; i < 18; i++)
		this->children_ptr_list[i] = NULL;
}

Tree_node::Tree_node(const Game& game_, int select) {
	this->win_count = 0;
	this->visit_count = 0;
	this->prob = 0;
	this->game_status = 0;
	this->game = game_;
	this->parent_ptr = NULL;
	if (select) new_child_nodes_model();
	else new_child_nodes();
}

Tree_node::Tree_node(const Tree_node& node) {
	this->win_count = node.win_count;
	this->visit_count = node.visit_count;
	this->prob = node.prob;
	this->game_status = node.game_status;
	this->game = node.game;
	this->parent_ptr = node.parent_ptr;
	for (int i = 0; i < 18; i++)
		this->children_ptr_list[i] = node.children_ptr_list[i];
}

void Tree_node::operator=(const Tree_node& node) {
	this->win_count = node.win_count;
	this->visit_count = node.visit_count;
	this->prob = node.prob;
	this->game_status = node.game_status;
	this->game = node.game;
	this->parent_ptr = node.parent_ptr;
	for (int i = 0; i < 18; i++)
		this->children_ptr_list[i] = node.children_ptr_list[i];
}

bool Tree_node::is_terminate() {
	return this->game_status != 0;
}

bool Tree_node::is_visit() {
	return this->visit_count != 0;
}

int Tree_node::new_child_nodes() {
	int legal_child_num = 0;
	// new the legal nodes for children_ptr
	for (int i = 0; i < 18; i++) {
		// check if the movement is legal
		int chs_index = i / 3, direction = i % 3;
		if (this->game.cur_chs_ptr[chs_index] == 0) continue;
		Movement movement(chs_index, direction);
		if (!this->game.check_in_board(movement)) continue;

		// new legal child nodes
		this->children_ptr_list[legal_child_num] = new Tree_node();
		this->children_ptr_list[legal_child_num]->chess_number = chs_index;
		this->children_ptr_list[legal_child_num]->direct = direction;
		Tree_node* node_ptr = this->children_ptr_list[legal_child_num];
		legal_child_num++;

		// reset the values of the members
		node_ptr->game = this->game;
		node_ptr->game_status = node_ptr->game.update_game_status(movement);
		node_ptr->game.switch_player();
		node_ptr->parent_ptr = this;
	}
	// reset the rest of the children_ptr to NULL
	for (int i = legal_child_num; i < 18; i++)
		this->children_ptr_list[i] = NULL;
	return legal_child_num;
}

void Tree_node::update(int win_count_) {
	this->visit_count++;
	this->win_count += win_count_;
	//cout << "  update : " << this->win_count << ", " << this->visit_count << endl;
}

void Tree_node::update(int win_count_, int visit_count_) {
	this->visit_count += visit_count_;
	this->win_count += win_count_;
	//cout << "  update : " << this->win_count << ", " << this->visit_count << endl;
}

int Tree_node::get_win_count() const {
	return this->win_count;
}

int Tree_node::get_visit_count() const {
	return this->visit_count;
}

float Tree_node::get_prob() const {
	return this->prob;
}

int Tree_node::get_game_status() const {
	return this->game_status;
}

Tree_node* Tree_node::get_parent_ptr() const {
	return this->parent_ptr;
}

Tree_node* Tree_node::get_child_ptr(const int& child_index) const {
	return this->children_ptr_list[child_index];
}

float Tree_node::get_ucb(Tree_node* cur_node, int i) {
	Tree_node* node_ptr = cur_node->get_child_ptr(i);
	float exploitation = (float)node_ptr->get_win_count() / ((float)node_ptr->get_visit_count()  /*+ this->eps*/);
	float exploration = sqrt(2) * sqrt(log((float)cur_node->get_visit_count()) / ((float)node_ptr->get_visit_count() /*+ this->eps*/));
	float ucb_val = exploitation + exploration;
	return ucb_val;
}

void Tree_node::node_status() {
	cout << "vvvvvvvvvvvvvvv NODE_STATUS vvvvvvvvvvvvvvv\n";
	cout << "node : " << this << '\n';
	this->game.print_board();
	this->game.print_game_status();
	printf("win_count: %d, visit: %d, status: %d\n", this->win_count, this->visit_count, this->game_status);

	printf("check in board                  :");
	for (int i = 0; i < 18; i++)
		printf("%c ", this->game.check_in_board(Movement(i / 3, i % 3)) ? 'T' : 'F'); printf("\n");

	printf("is exist                        :");
	for (int i = 0; i < 18; i++)
		printf("%c ", this->game.cur_chs_ptr[i / 3] != 0 ? 'T' : 'F'); printf("\n");
	cout << "^^^^^^^^^^^^^^^ NODE_STATUS ^^^^^^^^^^^^^^^\n";
}

void Tree_node::print_tree(Tree_node* root_node_ptr, int max_level) {
	int level_node_sum[20] = { 0 };
	for (int i = 1; i <= max_level; i++)
		print_level(root_node_ptr, i, level_node_sum, i);
	for (int i = 1; i < 20; i++)
		cout << level_node_sum[i] << " ";
	cout << '\n';
}

void Tree_node::print_level(Tree_node* node, int level, int level_node_sum[20], int level_num) {
	int child_num = 0;
	for (int i = 0; i < 18; i++) {
		if (node->children_ptr_list[i] == NULL) break;
		child_num++;
	}

	if (level == 1)
		level_node_sum[level_num]++;
	else {
		for (int i = 0; i < child_num; i++)
			print_level(node->children_ptr_list[i], level - 1, level_node_sum, level_num);
	}
}