#define _CRT_SECURE_NO_WARNINGS
#include<bits\stdc++.h>
#include <chrono>
#include <ctime>
#include"grid\grid.hpp"
using namespace::std;

int main() {
	int choice = 0;
	cout << "Select your choice:\n1- backtrack\n2- DFS\n3- DFS & heuristic\n4- BFS\n5- BFS & heuristic\n";
	cin >> choice;
	bool MRV, degree;
	if (choice == 3 || choice == 5) {
		MRV = true;
		degree = false;
	}
	freopen("fillomino.in", "r", stdin);
	freopen("fillomino.out", "w", stdout);
	int n, m, t = 1;
	while (cin >> n) {
		cin >> m;
		grid game(n, m);
		game.read();
		auto start = std::chrono::system_clock::now();
		bool res;
		bool visited[10][10];memset(visited, false, sizeof(visited));
		switch (choice)
		{
		case 1:
			res = game.solve(0, 0);
			break;
		case 2:
			res = game.solve_DFS(0, 0, visited, 0);
			break;
		case 3:
		{
			game.MRV = MRV;
			game.degree = degree;
			res = game.solve_DFS_Heuristic(0, 0, visited, 0);
			break;
		}
		case 4:
		{
			queue<node>q;
			q.push(game.board[0][0]);
			res = game.solve_BFS(q, visited);
			break;
		}
		case 5:
		{
			queue<node>q;
			q.push(game.board[0][0]);
			game.MRV = MRV;
			game.degree = degree;
			res = game.solve_BFS_Heuristic(q, visited);
			break;
		}
		default:
			res = game.solve_DFS(0, 0, visited, 0);
			break;
		}
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