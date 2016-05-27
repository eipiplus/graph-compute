#include"label.h"

segment::segment(int p,int n){
	path=p;
	node=n;
}
Label::~Label(){
	delete tp;
	delete ps;
	delete hash;
	delete idx;
}
Label::Label(Topo* tps){
	int vs=tps->getVs();
	tp=tps;
	ps=tp->djikstr();
	hash=new vector<int> [ vs*vs ];
	idx=new vector<segment> [ vs ];
	int src=0,dst=0;
	Path* tmp;
	for (int i = 0; i < ps->num ; ++i) {
		tmp=ps->path + i;
		src=tmp->road[0];
		dst=tmp->getDst();
		hash [ src*vs + dst].push_back(i);
		for (int j = 1; j < tmp->len-1; ++j) {
			idx[ tmp->road[j] ].push_back(new segment(i,j));
		}

	}
}
