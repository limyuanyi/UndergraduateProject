#include "play.h"
#include "gui.h"

using namespace std;

clock_t start, finish;

int main() {
	initgraph(WIDTH, HEIGHT, 1);
	GUI g;
	start = clock();

	for (int i = 0; i < 1; i++) {
		cout << "Please choose a mode:\n";
		cout << "0)Player v.s Player\n";
		cout << "1)AI v.s Player\n";
		cout << "2)AI v.s AI\n";

		cout << "Choose: ";
		int cmd = g.begin_gui();
		Play ewn;

		if (cmd == 0) ewn.two_players_mode();
		else if (cmd == 1) ewn.player_AI_mode();
		else if (cmd == 2) ewn.two_AIs_mode();
		else cout << "It's not a command\n";
	}
	finish = clock();
	cout << "time cost : " << (finish - start) / (double)(CLOCKS_PER_SEC) << " seconds\n";
	system("pause");
	return 0;
}