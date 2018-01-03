#ifndef GRID_H_
#define GRID_H_
#include "../node/node.hpp"
#include <bits\stdc++.h>

using namespace::std;

class grid {
public:
	node **board;
	int width;
	int height;
	int node_counter;
	int dx[4] = { 0,1,0,-1 };
	int dy[4] = { 1,0,-1,0 };


	grid(int width, int height);
	void read();
	void print();
	bool check_indices(int i, int j);
	int get_size(int i, int j, int value, bool visited[10][10], bool zero);
	bool solve(int i, int j);
	bool solve_DFS(int i, int j,bool dfs_visisted[10][10], int visited_count);
	bool solve_DFS_Heuristic(int i, int j, bool dfs_visisted[10][10], int visited_count);
	bool solve_BFS(queue<node>q, bool dfs_visisted[10][10], int visited_count);
	bool all_ok();
	void reset();
	int degree_heuristic(int i, int j);
	int MRV_heuristic(int i, int j);
	bool change(int i, int j, int value);
};

#endif