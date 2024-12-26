#include "randomAi.h"
#include "gui.h"

Movement RandomAi::AI_move(Game& cur_game, int dice) {
    this->ai_side = cur_game.side;
    Movement available_mvmt_list[6];
    int available_mvmt_cnt = 0, next_move_cnt = cur_game.count_movable_chs(dice);
    int movable_chs_index_list[2] = { cur_game.get_chess_index(cur_game.movable_chs_list[0]), cur_game.get_chess_index(cur_game.movable_chs_list[1]) };
    GUI g;

    g.draw_movable_cnt_color(cur_game, dice);
    g.movabledice_txt(cur_game.side, movable_chs_index_list[0], movable_chs_index_list[1]);
    cout << "movable chess: ";
    for (int i = 0; i < next_move_cnt; i++) {
        cout << "(" << i << ")";
        cout << movable_chs_index_list[i] + 1 << ' ';
    }
    cout << '\n';
    
    for (int i = 0; i < next_move_cnt; i++) {
        for (int direct = 0; direct < 3; direct++) {
            Movement mvmt(movable_chs_index_list[i], direct);
            if (cur_game.check_in_board(mvmt))
                available_mvmt_list[available_mvmt_cnt++] = mvmt;
        }
    }
    // randomly pick a move.
    Movement answer = available_mvmt_list[rand() % available_mvmt_cnt];
    g.draw_selected_chess(answer.first + 1);
    g.draw_selected_direction(cur_game.side, answer.second);
    cout << "move: (" << answer.first + 1 << ", " << answer.second << ")\n";
    return answer;
}