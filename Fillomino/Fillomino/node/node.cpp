#include "node.hpp"


node::node(int init_value) {
	this->init_value = init_value;
	this->value = init_value;
	this->visited = false;
}