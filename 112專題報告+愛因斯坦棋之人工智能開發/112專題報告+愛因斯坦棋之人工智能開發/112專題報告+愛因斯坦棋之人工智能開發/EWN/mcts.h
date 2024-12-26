#pragma once
#ifndef MCTS_H
#define MCTS_H

#include "uct.h"

const float eps = 1e-6;

class MCTS {
public:
    bool ai_side;
    int max_iterations;
    Selection* selection_ptr;

    MCTS();
    ~MCTS();

    Movement AI_move(Game& cur_game, int dice, int select);
    // Run 4 stages of MCTS for max_iterations times
    float run(const Game& cur_game, int select);
    // Run one time sumulation
    int simulation(const Game& cur_game);
    // for model
    int simulation_model(const Game& cur_game);
    // Used for destructor
    void recursive_delete_tree_node(Tree_node* node_ptr);
};
// src: https://github.com/memo/ofxMSAmcts/blob/master/src/ofxMSAmcts.h

#endif