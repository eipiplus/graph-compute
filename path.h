#ifndef PATH__H
#define PATH__H

#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<cstring>
#include<vector>

using namespace std;

#define NODES 1000
#define ED -1
#define SEL_N -1
#define SEL_R 0
#define SEL_A 1

struct Path{
	int len;
	int wei;
	int *road;
	int sz;
	int getDst();
	void printPath();
};
struct Paths{
	int num;
	Path* path;
	int *del;
	int sz;
	int used;
	Paths(int size,int path_num);
	void and_self(Paths &dst);
	~Paths();
	Paths();
};
class Topo {
	public:
		Topo();
		~Topo();
		Topo(string fn);
		int getVs();
		int next(int nd);
		Paths* djikstr(int src,int mode=0);
		/**
		 *For this topo, to compute all shortest paths.
		 * */
		Paths* djikstr();
		
	private:
		string * nd;
		int *adj; // vs*vs; not using weight,just to remain adjust info;
		int vs;   //nodes number
		int *wei; // vs*vs: keep link weight.
		Path* djikstr_not_all_sht(int src); //end of path.len is ED;
		void select_path_all(const int *prior,int nd,int *path,int *prefix,int &len,int &path_n);
		Paths* select_path(const int *prior,int *wgt,int mode=0);
		int* djikstr_prior(int src,int* &p);
};
#endif

