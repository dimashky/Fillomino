#include "node.hpp"

node::node() {
	this->x = 0;
	this->y = 0;
	this->init_value = 0;
	this->value = 0;
	this->visited = false;
}

node::node(int x, int y, int init_value) {
	this->x = x;
	this->y = y;
	this->init_value = init_value;
	this->value = init_value;
	this->visited = false;
}