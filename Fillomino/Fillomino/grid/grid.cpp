#define _CRT_SECURE_NO_WARNINGS
#include "grid.hpp"
#include <stdio.h>


grid::grid(int height, int width) {
	this->width = width;
	this->height = height;
	board = new node*[height];
	for (int i = 0; i < height; ++i) {
		board[i] = new node[width];
	}
}

void grid::read() {
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			cin >> board[i][j].init_value;
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
	// visit all cells
	if (i == height) {
		return all_ok();
	}

	// go to next line
	if (j == width)
		return solve(i + 1, 0);
	// go to next cell duo to current_size cell has already value
	if (board[i][j].init_value != 0)
		return solve(i, j + 1);

	// have chance to get all values for this cell
	bool visited[10][10];
	for (int v = 1;v <= 9;v++) {
		// save 'v' value to current node
		board[i][j].value = v;

		// check if this value disregard problem constraint
		memset(visited, false, sizeof(visited));
		int current_size = get_size(i, j, v, visited, false);
		if (current_size > v) {
			board[i][j].value = board[i][j].init_value;
			continue;
		}
		// check if this value have chance to get component size equal it
		memset(visited, false, sizeof(visited));
		if (current_size != v) {
			current_size = get_size(i, j, v, visited, true);

			if (current_size < v) {
				board[i][j].value = board[i][j].init_value;
				continue;
			}
		}

		// traverse around this cell to check if this cell did not block any thier neighbour cells 
		bool valid = true;
		for (int k = 0;k < 4;k++)
		{
			int x = i + dx[k];
			int y = j + dy[k];
			if (check_indices(x, y) && board[x][y].value != 0 && board[x][y].value != v) {
				memset(visited, false, sizeof(visited));
				current_size = get_size(x, y, board[x][y].value, visited, true);
				if (current_size < board[x][y].value) {
					valid = false;
					break;
				}
			}
		}

		if (!valid) {
			board[i][j].value = board[i][j].init_value;
			continue;
		}

		if (solve(i, j + 1)) {
			return true;
		}

		board[i][j].value = board[i][j].init_value;
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