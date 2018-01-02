#ifndef NODE_H_
#define NODE_H_

class node {
public:
	int init_value;
	int value;
	bool visited;
	int x;
	int y;
	node();
	node(int x, int y, int value);
};


#endif