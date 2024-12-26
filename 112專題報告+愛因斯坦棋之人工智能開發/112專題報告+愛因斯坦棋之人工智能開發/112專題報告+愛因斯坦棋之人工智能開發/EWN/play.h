#pragma once
#ifndef PLAY_H
#define PLAY_H

#include "game.h"
#include "ewnAi.h"

class Play {
private:
	Game game;

public:
	// One player plays the game (the integration)
	Movement player_move(const int& dice);

	/*-----------the Modes-----------*/
	// Two players fight!
	void two_players_mode();
	// One player fight with AI
	void player_AI_mode();
	// Two AIs fight
	void two_AIs_mode();
};

#endif