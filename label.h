#ifndef LABEL_H
#define LABEL_H

#include"path.h"

struct segment{
	int path;
	int node;
	segment(int p,int n);
};
class label{
	public:
		label(Topo* tps);
		~label();
	private:
		Topo* tp;
		Paths* ps;
		vector<int>* hash;
		vector<segment>* idx;
};

#endif

