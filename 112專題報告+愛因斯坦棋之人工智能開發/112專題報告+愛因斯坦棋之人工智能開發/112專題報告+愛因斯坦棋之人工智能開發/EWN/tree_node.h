#pragma once
#ifndef TREE_NODE_H
#define TREE_NODE_H

#include "game.h"

class Tree_node {
public:
	int win_count;
	int visit_count;
	float prob;

	int chess_number;
	int direct;

	int game_status;
	Game game;
	Tree_node* parent_ptr;
	// List for legal child nodes
	Tree_node* children_ptr_list[18];

	Tree_node();
	// Constructor for expand
	Tree_node(const Game& game_, int select);
	// Copy Constructor
	Tree_node(const Tree_node& node);
	// Copy operator
	void operator=(const Tree_node& node);

	// If the node is a end-gamed node
	bool is_terminate();
	// If the node has been visited (it is expanded)
	bool is_visit();

	// New legal child nodes and reset the values of the members(except children_ptr_list)
	int new_child_nodes();
	// for model
	int new_child_nodes_model();

	// for model
	void set_fm_board();
	// Update win_count and visit_count
	void update(int win_count_);
	void update(int win_count_, int visit_count_);

	int get_win_count() const;
	int get_visit_count() const;
	float get_prob() const;
	int get_game_status() const;
	Tree_node* get_parent_ptr() const;
	// Get child node pointer from children_ptr_list
	Tree_node* get_child_ptr(const int& child_index) const;
	float get_ucb(Tree_node* cur_node, int i);

	// Debuging console
	void node_status();
	void print_tree(Tree_node* root_node_ptr, int max_level);
	void print_level(Tree_node* node, int level, int level_node_sum[20], int level_num);
};

#endif