#pragma once
#ifndef EWNAI_H
#define EWNAI_H

#include "mcts.h"
#include "randomAi.h"

class EwnAI {
private:
	int ai_mode;
	MCTS mcts;
	RandomAi randomAi;

public:
	EwnAI();
	EwnAI(int mode);

	Movement AI_move(Game& cur_game, int dice);
	int get_ai_mode() const;
};

#endif