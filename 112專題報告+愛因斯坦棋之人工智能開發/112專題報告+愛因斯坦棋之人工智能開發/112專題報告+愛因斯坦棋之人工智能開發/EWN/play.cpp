#include "play.h"
#include "gui.h"

string roundn(string num, int n) {
    int pt_pos = 0;
    while (num[pt_pos] != '.')
        pt_pos++;
    pt_pos += n;
    return num.substr(0, pt_pos + 1);
}

Movement Play::player_move(const int& dice) {
    int movable_chs_cnt = this->game.count_movable_chs(dice), chs_index, direct;
    int movable_chs_index_list[2] = { this->game.get_chess_index(this->game.movable_chs_list[0]), this->game.get_chess_index(this->game.movable_chs_list[1]) };
    GUI g;

    cout << "\nNext chessman able to move: ";
    for (int i = 0; i < movable_chs_cnt; i++) {
        cout << "(" << i << ")";
        cout << movable_chs_index_list[i] + 1 << ' ';
    }
    g.movabledice_txt(this->game.side, movable_chs_index_list[0], movable_chs_index_list[1]);
        
    cout << "\nChoose: ";
    chs_index = g.select_chs_index(this->game, dice) - 1;
    if (!this->game.side)
        cout << "Direction: 0)Right 1)Down 2)Right-down\n";
    else
        cout << "Direction: 0)Left 1)Up 2)Left-up\n";

    cout << "Choose: ";
    direct = g.select_chs_direction(this->game);

    return Movement(chs_index, direct);
}

void Play::two_players_mode() {
    int dice, game_status;
    double clock_p1 = 0, clock_p2 = 0;
    clock_t start, finish;
    Movement mvmt;
    GUI g;

    this->game.print_board();

    while (1) {
        g.round_txt(1);
        dice = this->game.roll_dice();
        g.dice_txt(this->game.side, dice + 1);
        cout << "Dice: " << dice + 1 << '\n';
        g.clock_txt(0, clock_p1);
        g.clock_txt(1, clock_p2);
        
        start = clock();
        mvmt = this->player_move(dice);
        finish = clock();

        if (!this->game.side) clock_p1 += (finish - start) / (double)(CLOCKS_PER_SEC);
        else clock_p2 += (finish - start) / (double)(CLOCKS_PER_SEC);

        Sleep(500);
        // update: 0(game continues), 1(red wins), 2(blue wins)
        game_status = this->game.update_game_status(mvmt);
        this->game.print_board();

        if (game_status != 0) {
            cout << "====================================\n";

            string result;
            if (game_status == 1) result = "Red is the winner!!!";
            else result = "Blue is the winner!!!";

            cout << result << "\n";
            cout << "====================================\n";

            g.result_txt(result);
            return;
        }
        this->game.switch_player();
    }
}

void Play::player_AI_mode() {
    int dice, game_status;
    double clock_p1 = 0, clock_p2 = 0;
    clock_t start, finish;
    Movement mvmt;
    EwnAI ewnAI;
    GUI g;

    bool player_is_red = g.select_side();
    this->game.print_board();

    while (1) {
        g.round_txt(1);
        dice = this->game.roll_dice();
        g.dice_txt(this->game.side, dice + 1);
        cout << "Dice: " << dice + 1 << '\n';
        g.clock_txt(0, clock_p1);
        g.clock_txt(1, clock_p2);

        if (player_is_red && !this->game.side || !player_is_red && this->game.side) {
            start = clock();
            mvmt = this->player_move(dice);
            finish = clock();
        }
        else {
            start = clock();
            mvmt = ewnAI.AI_move(this->game, dice);
            finish = clock();
        }

        if (!this->game.side) clock_p1 += (finish - start) / (double)(CLOCKS_PER_SEC);
        else clock_p2 += (finish - start) / (double)(CLOCKS_PER_SEC);

        Sleep(500);
        // update: 0(game continues), 1(red wins), 2(blue wins)
        game_status = this->game.update_game_status(mvmt);
        this->game.print_board();

        if (game_status != 0) {
            cout << "====================================\n";

            string result;
            if (game_status == 1) {
                if (player_is_red) result = "Player(red) is the winner!!!";
                else result = "AI(red) is the winner!!!";
            }
            else {
                if (!player_is_red) result = "Player(blue) is the winner!!!";
                else result = "AI(blue) is the winner!!!";
            }

            cout << result << "\n";
            cout << "====================================\n";

            g.result_txt(result);
            return;
        }
        this->game.switch_player();
    }
}

void Play::two_AIs_mode() {
    int dice, game_status, mode;
    double clock_p1 = 0, clock_p2 = 0;
    clock_t start, finish;
    Movement mvmt;
    EwnAI ewnAI1, ewnAI2;
    GUI g;

    // set round
    int round = 500, win_count = 0;

    for (int i = 1; i <= round; i++) {
        clock_p1 = 0, clock_p2 = 0;
        cout << "Round " << i << '\n';
        this->game.print_board();

        while (1) {
            g.round_txt(i);
            dice = this->game.roll_dice();
            g.dice_txt(this->game.side, dice + 1);
            cout << "Dice: " << dice + 1 << '\n';
            g.clock_txt(0, clock_p1);
            g.clock_txt(1, clock_p2);
            
            if (!this->game.side) {
                start = clock();
                mvmt = ewnAI1.AI_move(this->game, dice);
                finish = clock();
                clock_p1 += (finish - start) / (double)(CLOCKS_PER_SEC);
            }
            else {
                start = clock();
                mvmt = ewnAI2.AI_move(this->game, dice);
                finish = clock();
                clock_p2 += (finish - start) / (double)(CLOCKS_PER_SEC);
            }

            // update: 0(game continues), 1(red wins), 2(blue wins)
            game_status = this->game.update_game_status(mvmt);
            this->game.print_board();

            if (game_status != 0) {
                cout << "====================================\n";

                string result;
                if (game_status == 1) {
                    mode = ewnAI1.get_ai_mode();
                    win_count++;

                    if (mode == 0) result = "MCTS(red) is the winner!!!";
                    else if (mode == 1) result = "Random(red) is the winner!!!";
                    else result = "Model(red) is the winner!!!";
                }
                else {
                    mode = ewnAI2.get_ai_mode();

                    if (mode == 0) result = "MCTS(blue) is the winner!!!";
                    else if (mode == 1) result = "Random(blue) is the winner!!!";
                    else result = "Model(blue) is the winner!!!";
                }

                cout << result << "\n";
                cout << "====================================\n";

                string win_rate = "The win rate of the red side: ";
                win_rate += to_string(win_count) + " / " + to_string(i) + " = ";
                win_rate += roundn(to_string((float)win_count / i * 100), 2) + "%";
                cout << win_rate << '\n';

                if (i == round) {
                    g.result_txt(result);
                    g.win_rate_txt(win_rate);
                }
                break;
            }
            this->game.switch_player();
        }
        // reset game
        this->game = Game();
    }
}