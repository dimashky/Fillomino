#define _CRT_SECURE_NO_WARNINGS
#include<bits\stdc++.h>
#include <chrono>
#include <ctime>
#include"grid\grid.hpp"
using namespace::std;

int main() {
	freopen("fillomino.in", "r", stdin);
	freopen("fillomino.out", "w", stdout);
	int n, m, t = 1;
	while (cin>>n) {
		cin >> m;
		grid game(n, m);
		game.read();
		auto start = std::chrono::system_clock::now();
		bool res;
		bool visited[10][10];memset(visited, false, sizeof(visited));
		res = game.solve_DFS(0, 0, visited, 0);
		//res = game.solve(0, 0);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;

		cout << "Finish test " << t++ << " in " << elapsed_seconds.count() << "s.\nNode Counter = " << game.node_counter << endl;
		if (res) {
			cout << "The solution is:\n";
			game.print();
			printf("\n====================================\n");
		}
		else {
			cout << "No solution!\n====================================\n";
		}
	}
	return 0;
}