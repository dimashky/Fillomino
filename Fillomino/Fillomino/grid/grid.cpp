#define _CRT_SECURE_NO_WARNINGS
#include "grid.hpp"
#include <stdio.h>

grid::grid(int height, int width) {
	this->width = width;
	this->height = height;
	this->node_counter = 0;
	board = new node*[height];
	for (int i = 0; i < height; ++i) {
		board[i] = new node[width];
	}
}

void grid::read() {
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			cin >> board[i][j].init_value;
			board[i][j].x = i;
			board[i][j].y = j;
			board[i][j].value = board[i][j].init_value;
		}
	}
}

void grid::print() {
	for (int j = 0; j < width; ++j) {
		printf("-------+");
	}
	printf("\n");
	for (int i = 0; i < height; ++i) {
		printf("|");
		for (int j = 0; j < width; ++j) {
			printf("   %d   ", board[i][j].value);
			if (j == width - 1 || board[i][j].value != board[i][j + 1].value)
				printf("|");
			else
				printf(" ");
		}
		printf("\n");
		printf("+");
		for (int j = 0; j < width; ++j) {
			if (i == height - 1 || board[i][j].value != board[i + 1][j].value)
				printf("-------+");
			else
				printf("       +");

		}
		printf("\n");
	}
}

bool grid::check_indices(int i, int j) {
	return i >= 0 && i < height && j >= 0 && j < width;
}

int grid::get_size(int i, int j, int value, bool visited[10][10], bool zero)
{
	visited[i][j] = true;
	int res = 1;
	for (int k = 0;k < 4;k++)
	{
		int x = i + dx[k];
		int y = j + dy[k];
		if (check_indices(x, y) && !visited[x][y] && (board[x][y].value == value || zero && board[x][y].value == 0)) {
			res += get_size(x, y, value, visited, zero);
			// here to check only if this cell (which has value 'value') can fill correctly
			if (zero && res >= value) {
				return res;
			}
		}
	}
	return res;
}

bool grid::solve(int i, int j) {
	// if we are out of board so we will go to next line instantly 
	if (j == width)
		return solve(i + 1, 0);

	bool visited[10][10];	// helpful matrix for calling 'grid::get_size()' method! 

	// record this node analysis
	node_counter++;

	// check if we visited all our variables so check it and return!
	if (i == height) {
		return all_ok();
	}

	// check if this variable has already some value other than zero
	if (board[i][j].value != 0) {
		// check if this variable can satisfact the constraints
		memset(visited, false, sizeof(visited));
		if (get_size(i, j, board[i][j].value, visited, true) < board[i][j].value) {
			return false;
		}
		else {
			// call next variable
			return solve(i, j + 1);
		}
	}

	/*
		Now current Variable has ZERO value
	*/
	// DECLARE VARIABLES!
	int x, y;
	int current_component_size;
	bool valid;
	// have chance to get all values for this cell
	for (int v = 1;v <= 9;v++) {
		// save the chosen value to current node
		board[i][j].value = v;
		/* check if this value disregard problem constraint */
		memset(visited, false, sizeof(visited));
		// check if we create component which had size bigger than chosen value 
		current_component_size = get_size(i, j, v, visited, false);
		if (current_component_size > v) {
			board[i][j].value = board[i][j].init_value;
			continue;
		}
		// check if this value have chance to get component size equal it
		if (current_component_size != v)
		{
			memset(visited, false, sizeof(visited));
			if (get_size(i, j, v, visited, true) < v) {
				board[i][j].value = board[i][j].init_value;
				continue;
			}
		}

		// traverse around this cell to check if this cell did not block any thier neighbour cells 
		valid = true;
		for (int k = 0;k < 4;k++)
		{
			x = i + dx[k], y = j + dy[k];
			if (check_indices(x, y) && board[x][y].value != 0 && board[x][y].value != v) {
				memset(visited, false, sizeof(visited));
				if (get_size(x, y, board[x][y].value, visited, true) < board[x][y].value) {
					valid = false;
					break;
				}
			}
		}
		// this value 'v' had block other cells so we continue to test another value
		if (!valid) {
			board[i][j].value = board[i][j].init_value;
			continue;
		}
		// solve next cell
		if (solve(i, j + 1)) {
			return true;
		}
		// backtrack
		board[i][j].value = board[i][j].init_value;
	}
	return false;
}








bool grid::solve_BFS(queue<node>q, bool bfs_visisted[10][10], int visited_count)
{
	node_counter++;

	node fr = q.front();
	q.pop();

	bfs_visisted[fr.x][fr.y] = true;

	if (board[fr.x][fr.y].value != 0)
	{

		bool visited[10][10];

		memset(visited, false, sizeof(visited));

		if (get_size(fr.x, fr.y, fr.value, visited, true) < fr.value)
			return false;
		
		else 
		
			for (int k = 0; k < 4; ++k) 
			{
				int x = fr.x + dx[k];
				int y = fr.y + dy[k];
				if (check_indices(x, y) && !bfs_visisted[x][y]) 
				{
					bfs_visisted[x][y] = true;
					q.push(board[x][y]);
				}
			}


		if (!q.empty())
			return solve_BFS(q, bfs_visisted, visited_count);
		
		return all_ok();
		
	}



	int current_component_size;
	bool valid;
	bool new_bfs_visited[10][10], visited[10][10];


	for (int v = 1; v <= 9; v++) 
	{

		for (int idx = 0; idx < height; idx++) {
			memcpy(&new_bfs_visited[idx], &bfs_visisted[idx], sizeof(bfs_visisted[idx]));
		}

		board[fr.x][fr.y].value = v;

		memset(visited, false, sizeof(visited));

		current_component_size = get_size(fr.x, fr.y, v, visited, false);
		
		if (current_component_size > v) {
			board[fr.x][fr.y].value = 0;
			continue;
		}
		
		if (current_component_size != v) 
		{
			memset(visited, false, sizeof(visited));
			current_component_size = get_size(fr.x, fr.y, v, visited, true);
			if (current_component_size < v) 
			{
				board[fr.x][fr.y].value = 0;
				continue;
			}
		}

		valid = true;
		for (int k = 0;k < 4;k++) 
		{
			int x = fr.x + dx[k];
			int y = fr.y + dy[k];

			if (check_indices(x, y) && board[x][y].value != 0 && board[x][y].value != v) 
			{
				memset(visited, false, sizeof(visited));
				if (get_size(x, y, board[x][y].value, visited, true) < board[x][y].value) 
				{
					valid = false;
					break;
				}
			}
		}

		if (valid == false) 
		{
			board[fr.x][fr.y].value = 0;
			continue;
		}


		
		for (int k = 0; k < 4; ++k) 
		{
			int x = fr.x + dx[k];
			int y = fr.y + dy[k];
				
			if (check_indices(x, y) && !new_bfs_visited[x][y])
			{
				new_bfs_visited[x][y] = true;
				q.push(board[x][y]);
			}
		}



		if (!q.empty())
		{
			if (solve_BFS(q, new_bfs_visited, visited_count))
				return true;
		}
		else return all_ok();
		

		board[fr.x][fr.y].value = board[fr.x][fr.y].init_value;
	}
	return false;



}




bool grid::all_ok() {
	bool visited[10][10];
	memset(visited, false, sizeof(visited));
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if (visited[i][j] == false) {
				if (get_size(i, j, board[i][j].value, visited, false) != board[i][j].value)
					return false;
			}
		}
	}
	return true;
}

void grid::reset() {
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			board[i][j].value = board[i][j].init_value;
		}
	}
}

bool grid::solve_DFS(int i, int j, bool dfs_visited[10][10], int visited_count) {

	node_counter++;			// record this node analysis

	// mark this cell as visited!
	dfs_visited[i][j] = true;
	visited_count++;

	bool visited[10][10];	// helpful matrix for calling 'grid::get_size()' method! 
	int x, y;
	// check if this variable has already some value other than zero
	if (board[i][j].value != 0) {
		// if we are in the last remaining cell
		if (visited_count == height*width) {
			if(all_ok())
				return true;
			return false;
		}
		// check if this variable can satisfact the constraints
		memset(visited, false, sizeof(visited));
		if (get_size(i, j, board[i][j].value, visited, true) < board[i][j].value) {
			return false;
		}
		else {
			bool ret = true;
			for (int k = 0; k < 4; ++k) {
				x = i + dx[k];
				y = j + dy[k];
				if (check_indices(x, y) && !dfs_visited[x][y]) {
					ret &= solve_DFS(x, y, dfs_visited, visited_count);
					if (ret == false)
						return false;
				}
			}
			if (ret == true)
				return true;
		}
	}

	/*
		Now current Variable has ZERO value
	*/
	// DECLARE VARIABLES!
	int current_component_size;
	bool valid;
	bool new_dfs_visited[10][10];
	// have chance to get all values for this cell
	for (int v = 1; v <= 9; v++) {
		// copy dfs_visited into new_dfs_visited
		for (int idx = 0; idx < height; idx++) {
			memcpy(&new_dfs_visited[idx], &dfs_visited[idx], sizeof(dfs_visited[idx]));
		}

		// save 'v' value to current node
		board[i][j].value = v;

		// if we are in the last remaining cell
		if (visited_count == height*width && all_ok()) {
			return true;
		}

		// check if this value disregard problem constraint
		memset(visited, false, sizeof(visited));
		current_component_size = get_size(i, j, v, visited, false);
		if (current_component_size > v) {
			board[i][j].value = board[i][j].init_value;
			continue;
		}
		// check if this value have chance to get component size equal it
		if (current_component_size != v) {
			memset(visited, false, sizeof(visited));
			current_component_size = get_size(i, j, v, visited, true);
			if (current_component_size < v) {
				board[i][j].value = board[i][j].init_value;
				continue;
			}
		}

		// traverse around this cell to check if this cell did not block any thier neighbour cells 
		valid = true;
		for (int k = 0;k < 4;k++) {
			x = i + dx[k]; y = j + dy[k];
			if (check_indices(x, y) && board[x][y].value != 0 && board[x][y].value != v) {
				memset(visited, false, sizeof(visited));
				if (get_size(x, y, board[x][y].value, visited, true) < board[x][y].value) {
					valid = false;
					break;
				}
			}
		}
		if (valid == false) {
			board[i][j].value = board[i][j].init_value;
			continue;
		}


		// else we have to go around 
		if (visited_count != height * width) {
			bool ret = true;
			for (int k = 0; k < 4; ++k) {
				x = i + dx[k], y = j + dy[k];
				if (check_indices(x, y) && !dfs_visited[x][y]) {
					ret &= solve_DFS(x, y, new_dfs_visited, visited_count);
					if (ret == false)
						break;
				}
			}
			if (ret == true)
				return true;
		}

		board[i][j].value = board[i][j].init_value;
	}
	return false;
}

bool grid::solve_DFS_Heuristic(int i, int j, bool dfs_visited[10][10], int visited_count) {

	node_counter++;			// record this node analysis

							// mark this cell as visited!
	dfs_visited[i][j] = true;
	visited_count++;

	bool visited[10][10];	// helpful matrix for calling 'grid::get_size()' method! 
	int x, y;
	// check if this variable has already some value other than zero
	if (board[i][j].value != 0) {
		// if we are in the last remaining cell
		if (visited_count == height*width) {
			if (all_ok())
				return true;
			return false;
		}
		// check if this variable can satisfact the constraints
		memset(visited, false, sizeof(visited));
		if (get_size(i, j, board[i][j].value, visited, true) < board[i][j].value) {
			return false;
		}
		else {
			bool ret = true;
			vector<pair<int, node*>> order;
			for (int k = 0; k < 4; ++k) {
				x = i + dx[k];
				y = j + dy[k];
				if (check_indices(x, y) && !dfs_visited[x][y]) {
					order.push_back(pair<int, node*>(degree_heuristic(x, y)+MRV_heuristic(x,y), &board[x][y]));
				}
			}
			sort(order.begin(), order.end());
			for (int idx = (int)order.size() - 1; idx >= 0 ; idx--) {
				ret &= solve_DFS((order[idx].second)->x, (order[idx].second)->y, dfs_visited, visited_count);
				if (ret == false)
					return false;
			}
			if (ret == true)
				return true;
		}
	}

	/*
	Now current Variable has ZERO value
	*/
	// DECLARE VARIABLES!
	int current_component_size;
	bool valid;
	bool new_dfs_visited[10][10];
	// have chance to get all values for this cell
	for (int v = 1; v <= 9; v++) {
		// copy dfs_visited into new_dfs_visited
		for (int idx = 0; idx < height; idx++) {
			memcpy(&new_dfs_visited[idx], &dfs_visited[idx], sizeof(dfs_visited[idx]));
		}

		// save 'v' value to current node
		board[i][j].value = v;

		// if we are in the last remaining cell
		if (visited_count == height*width && all_ok()) {
			return true;
		}

		// check if this value disregard problem constraint
		memset(visited, false, sizeof(visited));
		current_component_size = get_size(i, j, v, visited, false);
		if (current_component_size > v) {
			board[i][j].value = board[i][j].init_value;
			continue;
		}
		// check if this value have chance to get component size equal it
		if (current_component_size != v) {
			memset(visited, false, sizeof(visited));
			current_component_size = get_size(i, j, v, visited, true);
			if (current_component_size < v) {
				board[i][j].value = board[i][j].init_value;
				continue;
			}
		}

		// traverse around this cell to check if this cell did not block any thier neighbour cells 
		valid = true;
		for (int k = 0;k < 4;k++) {
			x = i + dx[k]; y = j + dy[k];
			if (check_indices(x, y) && board[x][y].value != 0 && board[x][y].value != v) {
				memset(visited, false, sizeof(visited));
				if (get_size(x, y, board[x][y].value, visited, true) < board[x][y].value) {
					valid = false;
					break;
				}
			}
		}
		if (valid == false) {
			board[i][j].value = board[i][j].init_value;
			continue;
		}


		// else we have to go around 
		if (visited_count != height * width) {
			bool ret = true;
			vector<pair<int, node*>> order;
			for (int k = 0; k < 4; ++k) {
				x = i + dx[k], y = j + dy[k];
				if (check_indices(x, y) && !dfs_visited[x][y]) {
					order.push_back(pair<int, node*>(degree_heuristic(x,y)+ MRV_heuristic(x, y),&board[x][y]));
				}
			}
			sort(order.begin(), order.end());
			for (int idx = (int)order.size(); idx >= 0; --idx) {
				ret &= solve_DFS((order[i].second)->x, (order[i].second)->y, new_dfs_visited, visited_count);
				if (ret == false)
					break;
			}
			if (ret == true)
				return true;
		}

		board[i][j].value = board[i][j].init_value;
	}
	return false;
}

int grid::degree_heuristic(int i, int j) {
	int neighbours_cnt = 0;
	for (int k = 0; k < 4; ++k)
		if (check_indices(i + dx[k], j + dy[k]) && board[i + dx[k]][j + dy[k]].value != 0)
			neighbours_cnt += 1;
	return neighbours_cnt * 10;
}

int grid::MRV_heuristic(int i, int j) {
	int cnt = 0;
	for (int v = 1; v <= 9; v++) {
		// save 'v' value to current node
		board[i][j].value = v;

		bool visited[10][10];
		// check if this value disregard problem constraint
		memset(visited, false, sizeof(visited));
		int current_component_size = get_size(i, j, v, visited, false);
		if (current_component_size > v) {
			board[i][j].value = board[i][j].init_value;
			continue;
		}
		// check if this value have chance to get component size equal it
		if (current_component_size != v) {
			memset(visited, false, sizeof(visited));
			current_component_size = get_size(i, j, v, visited, true);
			if (current_component_size < v) {
				board[i][j].value = board[i][j].init_value;
				continue;
			}
		}

		// traverse around this cell to check if this cell did not block any thier neighbour cells 
		bool valid = true;
		for (int k = 0;k < 4;k++) {
			int x = i + dx[k], y = j + dy[k];
			if (check_indices(x, y) && board[x][y].value != 0 && board[x][y].value != v) {
				memset(visited, false, sizeof(visited));
				if (get_size(x, y, board[x][y].value, visited, true) < board[x][y].value) {
					valid = false;
					break;
				}
			}
		}
		if (valid == false) {
			board[i][j].value = board[i][j].init_value;
			continue;
		}
		cnt++;
		board[i][j].value = board[i][j].init_value;
	}
	return -1 * cnt;
}