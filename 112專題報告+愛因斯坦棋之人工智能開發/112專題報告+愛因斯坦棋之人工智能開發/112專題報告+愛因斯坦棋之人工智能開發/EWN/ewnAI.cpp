#include "gui.h"
#include "ewnAi.h"

using namespace std;

EwnAI::EwnAI() {
    this->ai_mode = 0;
    cout << '\n';
    cout << "Please choose an AI:\n";
    cout << "0)MCTS\n";
    cout << "1)Random\n";
    cout << "2)Model\n";
    cout << "Choose: ";
    int cmd;
    // cin >> cmd;
    // GUI-new-----------------------------
    GUI g;
    cmd = g.select_AI();

    if (cmd >= 0 && cmd <= 2) this->ai_mode = cmd;
}

EwnAI::EwnAI(int mode) {
    this->ai_mode = mode;
}

Movement EwnAI::AI_move(Game& cur_game, int dice) {
    switch (this->ai_mode) {
    case 0:
        return this->mcts.AI_move(cur_game, dice, 0);
    case 1:
        return this->randomAi.AI_move(cur_game, dice);
    case 2:
        return this->mcts.AI_move(cur_game, dice, 1);
    }
}

int EwnAI::get_ai_mode() const {
    return this->ai_mode;
}