#pragma once
#ifndef UCT_H
#define UCT_H

#include "tree_node.h"

class Selection {
public:
	float eps;

	Selection() {}
	Selection(float eps_) { this->eps = eps_; }
	~Selection() {}

	virtual int select_children_list_index(Tree_node* cur_node) { return 0; }
	virtual int select_children_list_index_model(Tree_node* cur_node) { return 0; }
};

class UCT : public Selection {
public:
	float exploration_param;
	float t_param;

	UCT();
	UCT(float eps_);

	int select_children_list_index(Tree_node* cur_node);
	// for model
	int select_children_list_index_model(Tree_node* cur_node);
};

#endif