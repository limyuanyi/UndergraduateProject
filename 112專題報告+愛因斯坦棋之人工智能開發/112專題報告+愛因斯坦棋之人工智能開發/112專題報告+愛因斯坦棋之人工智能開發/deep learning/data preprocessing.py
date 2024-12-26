import numpy as np

players = ('Fraggle_Reloaded_c', 'EWN_ghost_', 'scrapple', 'sZamBa_', '1Stone', 'Martyn Hamer', 'bennok', 'Micco', 'Paul Wiselius', 'Masahito')
mode = 'w'

for p in players:
    with open(p + '.txt', 'r', encoding="utf-8") as f:
        if p != players[0]:
            mode = 'a'
        with open('training_data.csv', mode) as ff:
            event, i = [], 0
            for line in f.readlines():
                event.append(line)
                i += 1
                # 7行為1局(含空行)
                if i == 7:
                    # 超級理想狀況下，5手結束遊戲，若少於表示還未下完
                    if len(event[5]) <= 64:
                        event, i = [], 0
                        continue

                    games = event[5].split(' ')
                    moves = games[1]
                    j = 2
                    while j < len(games):
                        if len(games[j]) == 7:
                            moves += ',' + games[j]
                        # next game
                        elif games[j][0] == '[':
                            # red win
                            if moves[-2:] == 'a5':
                                ff.writelines(moves + ',0-1\n')
                            # blue win
                            elif moves[-2:] == 'e1':
                                ff.writelines(moves + ',1-0\n')
                            # not sure
                            else:
                                ff.writelines(moves + '\n')
                            moves = games[j] + ',' + games[j + 1] + ',' + games[j + 2]
                            j += 4
                            continue

                        if j + 1 < len(games):
                            # not yet ended
                            if games[j + 1] == '*\n':
                                break
                            elif len(games[j + 1]) == 7:
                                moves += ',' + games[j + 1]
                            # next game
                            elif games[j + 1][0] == '[':
                                # red win
                                if moves[-2:] == 'a5':
                                    ff.writelines(moves + ',0-1\n')
                                # blue win
                                elif moves[-2:] == 'e1':
                                    ff.writelines(moves + ',1-0\n')
                                # not sure
                                else:
                                    ff.writelines(moves + '\n')
                                moves = games[j + 1] + ',' + games[j + 2]
                                j += 4
                                continue
                            # blue win
                            elif games[j + 1] == '1-0\n':
                                ff.writelines(moves + ',1-0\n')
                
                        if j + 2 < len(games):
                            # not yet ended
                            if games[j + 2] == '*\n':
                                break
                            # red win
                            elif games[j + 2] == '0-1\n':
                                ff.writelines(moves + ',0-1\n')

                        j += 3

                    event, i = [], 0

with open('training_data.csv', 'r+') as f:
    data = ''
    for line in f.readlines():
        result = line[-4:-1]
        if result != '0-1' and result != '1-0':
            # check if ends
            red_num, blue_num = 6, 6
            board = np.zeros((5, 5))
            moves = line.split(',')

            board[4][4] = int(moves[0][1])+6
            board[4][3] = int(moves[0][2])+6
            board[4][2] = int(moves[0][3])+6
            board[3][4] = int(moves[0][4])+6
            board[3][3] = int(moves[0][5])+6
            board[2][4] = int(moves[0][6])+6

            board[0][0] = int(moves[1][0])
            board[0][1] = int(moves[1][1])
            board[0][2] = int(moves[1][2])
            board[1][0] = int(moves[1][3])
            board[1][1] = int(moves[1][4])
            board[2][0] = int(moves[1][5])

            chars = 'edcba'
            cols = {k:v for v, k in enumerate(chars)}
            for move in moves[2:]:
                src, dst = move[2:4], move[5:7]
                src_row, src_col, dst_row, dst_col = int(src[1])-1, cols[src[0]], int(dst[1])-1, cols[dst[0]]
                
                if board[dst_row][dst_col] != 0:
                    if 1 <= board[dst_row][dst_col] <= 6:
                        red_num -= 1
                    else:
                        blue_num -= 1

                board[dst_row][dst_col] = board[src_row][src_col]    
                board[src_row][src_col] = 0

            if red_num == 0:
                data += line[:-1] + ',1-0\n'
            elif blue_num == 0:
                data += line[:-1] + ',0-1\n'
        else:
            data += line
    f.seek(0)
    f.write(data)

    f.seek(0)
    n1, n2 = 0, 0
    for line in f.readlines():
        # red win or blue win
        result = line[-4:-1]
        if result == '0-1' or result == '1-0':
            if line[-7:-5] == 'a5' or line[-7:-5] == 'e1':
                n1 += 1
            n2 += 1
    print(f'Total games = {n2}, occupy enemy corner = {n1}')