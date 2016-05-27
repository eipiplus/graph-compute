#include"path.h"
int Path::getDst(){
		return road[len-1];
}
void Path::printPath(){
	int i=0;
	printf("#From v%d to v%d  wei (%d) len(%d)\n:> ",road[0],getDst(),wei,len-1);
	for(; i<len-1; i++ ){
		printf(" %d =>",road[i]);
	}
	printf(" %d ]\n", road[i]);
}
Paths::~Paths(){
	delete [] del;
	delete [] path;
}
Paths::Paths(int size,int path_num){
	num=0;
	del=new int [size];
	path=new Path [path_num];
	sz=size;
	used=0;
}
Paths::Paths(){}
void Paths::and_self( Paths &dst){
	int *tmp=del+used;
	if(used + dst.used > sz){
		cerr<<"Paths add error: no space for add"<<endl;
		return;
	}
	memcpy(tmp,dst.del,dst.used*sizeof(int));
	for (int i = 0; i < dst.num; ++i) {
		path[num].road = dst.path[i].road - dst.del+tmp;
		path[num].wei= dst.path[i].wei;
		path[num].sz=dst.path[i].sz;
		path[num].len=dst.path[i].len;
		++num;
	}
	used += dst.used;
}
Topo::Topo(){
	adj=0;
	vs=0;
	wei=0;
}
Topo::~Topo(){
	delete adj;
	delete wei;
	delete nd;
};
Topo::Topo(string fn){
	const char*  fnc=fn.c_str();
	FILE *f=fopen(fnc,"r");
	int i=0,n1=0,n2=0,t1=0,t2=0,w;
	fscanf(f,"%d",&vs);
	adj=new int [vs*vs];
	wei=new int [vs*vs];
	for(i=0;i<vs;i++)
		adj[i*vs + vs -1]=0;
	/*** only for no weight link **/
	while(fscanf(f,"%d %d %d",&n1,&n2,&w)!=EOF){
		wei[ n1*vs + n2 ]=w;
		wei[ n2*vs +n1  ]=w;
		t1 = adj[n1*vs + vs -1] ++;
		t2 = adj[n2*vs + vs -1] ++;
		adj[n1*vs + t1]=n2;
		adj[n2*vs + t2]=n1;

	}
	for(i=0; i<vs; i++){
		t1 = adj[i*vs + vs -1];
		adj[ i*vs + t1 ]=ED;
	}
	fclose(f);
}
Paths* Topo::djikstr(){
	int i=0,sz=0,pths=0;
	Paths* *djik=new Paths*[vs];
	for (i = 0; i < vs; ++i) {
		djik[i]=djikstr(i);
	}
	for (i = 0; i < vs; ++i) {
		sz += djik[i]->sz;
		pths += djik[i]->num;
	}
	
	Paths *rt = new Paths(sz,pths);
	for ( i = 0; i < vs; ++i) {
		rt->and_self(*djik[i]);
	}	
	delete []djik;
	return rt;
}
// first time to record all paths, and find it's difficult to show all shortest paths
// The second time, using direct graph, to find out shortest path direct graph.
//  The use DSF to find all path.
Path* Topo::djikstr_not_all_sht (int src){
	const int inf=0x3ffffff;
	const int ql=vs*vs;
	const int pv=vs+1;
	int i=0,j=0;
	int *len=new int [vs];
	int *path = new int [vs*pv];
	int **oth=new int* [vs], *oths=new int [vs];
	int *queue=new int [ql],op=0,ed=0;
	memset(len,0,sizeof(int)*vs);
	memset(path,0,sizeof(int)*vs*pv);
	memset(queue,0,sizeof(int)*ql);
	memset(oths,0,sizeof(int)*vs);
	for(i=0; i<vs; i++) 
		len[i]=inf;
	len[src]=0; 
	// add src to queue
	queue[op]=src; ++ed;
	// path for src
	path[src*pv]=0;
	int* base = 0;
		while(op != ed){
			int tmp=ed;
			// looking for new node to add : weight is less.
			for(; op != tmp;  op = ++op %ql){
				int lka=queue[op],lkb=0;
				base = adj+queue[op]*vs;
				for(j=0; base[j] != ED && j<vs; j++){
					lkb = base[j];
					int newei = wei[ lka*vs + lkb ]+len[lka] ;
					if( newei < len[ lkb ] ){
						len[ lkb ] = wei[ lka*vs+lkb ] + len[lka];
						// need to know memcpy args meaning=> dst, src, size
						memcpy(path+lkb*pv, path +lka*pv , pv*sizeof(int));
						++path[ lkb * pv];
						path[lkb*pv + path[lkb*pv] ] = lkb;
						queue[ed]= base[j];
						ed = ++ed % ql;
						// delete all recorded other used path
						if(oths[lkb] != 0){
							delete[] oth[lkb];
							oths[lkb] = 0;
						}
					}else if (newei == len[ lkb ]){
						if(oths[lkb] == 0)
							oth[lkb] = new int  [vs*pv];
						memcpy(oth[lkb]+oths[lkb]*pv , path+lka*pv, pv*sizeof(int));
						int ot = ++oth[lkb][ oths[lkb]*pv ];
						oth[lkb][ oths[lkb]*pv + ot ]=lkb;
						++oths[lkb];
						
					}
				}
			}
		}
	int all=0;
	for(i=0; i<vs; i++){
		if(i != inf && i!=0 ) ++all;
		all += oths[i];
	}
	Path* rt = new Path [all+1];
	int cd=0;
	for(i=0; i<vs; i++){
		if(i != inf && i!=0 ){
			rt[cd].wei=len[i];
			rt[cd].len=path[i*pv];
			rt[cd++].road=path+pv*i+1;
		}
		for(int a=0; a< oths[i]; a++ ){
			rt[cd].wei=len[i];
			rt[cd].len=oth[i][a*pv];
			rt[cd++].road=oth[i]+a*pv+1;
		}
	}
	rt[cd].len=ED;
	delete [] len;
	delete [] queue;
	delete [] oths;
	return rt;	
}
/**
 * select path use prior nodes, this function has three modes:normal select(-1),random select(0) and all select(1);
 * */

void Topo::select_path_all(const int *prior,int nd,int *path,int *prefix,int &len,int &path_n){
	int i=0;
	const int pv=vs+1;
	if(path_n >= 10*vs*vs){
		cerr<<"Sorry, Path number is too big"<<endl;
		return;
	}
	if(prior[ nd*vs ]){
		prefix[ len++ ] = nd;
		for (i = 1; i < prior[ nd*vs ]+1; ++i) {
			select_path_all(prior,prior[ nd*vs+i ],path,prefix,len,path_n);
		}
		--len;
	}else{
		prefix[  len++ ] = nd;
		memcpy(path+path_n*pv,prefix,sizeof(int)*pv);
		path[ path_n*pv +len ]=-1;
		++path_n;
		--len;
	}
}
int Topo::getVs(){
	return vs;
}
Paths* Topo::select_path(const int *prior,int *wgt,int mode){
	const int random=0;
	const int normal=-1;
	const int all=1;
	const int pv=vs+1;
	int* path = new int [vs*pv];
	int* path_all = new int [10* vs*vs*pv],paths=0;
	int* len = new int[vs];
	int i=0,nd=0;
	int *prefix=new int [pv];
	memset(len,0,vs*sizeof(int));
	switch (mode){
		case normal:
			for (i = 0; i < vs; ++i) {
				nd=i;
				if( !prior[nd*vs] )continue;
				while(prior[ nd*vs ]){
					path[ paths * pv + len[ paths ]++ ]=nd;
					nd=prior[nd*vs+ 1];
				}
				path[paths * pv + len[paths]++ ]=nd;
				paths++;
			}
			break;
		case random:
			srand(time(NULL));
			for (i = 0; i < vs; ++i) {
				nd=i;
				if( !prior[nd*vs] )continue;
				int hd=0;
				while(prior[ nd*vs ]){
					hd = prior[ nd*vs ];
					path[paths * pv + len[ paths ]++]=nd;
					nd=prior[nd*vs+ 1+ rand()%hd ];
				}
				path[paths * pv + len[paths]++ ]=nd;
				paths++;
			}
			break;
		case all: // need rescuit
			for( i=0; i < vs; ++i )
				if(prior[ i*vs ])
					select_path_all(prior,i,path_all,prefix,nd,paths);
			break;

	}
	/**add path*/
	struct{
		void operator()(int *list,int len){
			int tmp=0;
			for (int i = 0; i < len/2; ++i) {
				tmp=list[i];
				list[i] = list[len-1-i];
				list[len-1-i]=tmp;
			}
		}
	}reverse;
	Paths *rts=new Paths();
	Path* rt = new Path [paths];
	int* tp=new int [paths*pv];
	if(mode == all)
		memcpy(tp,path_all,paths*pv*sizeof(int));
	else
		memcpy(tp,path, paths*pv*sizeof(int));
	for ( i = 0; i < paths; ++i) {
		rt[i].len=0;
		rt[i].road=tp+i*pv;
		rt[i].wei=wgt[ rt[i].road[0] ];
		rt[i].sz=pv;
		if (mode!=all) {
			rt[i].len=len[i];
		}else
			while( rt[i].road[ ++rt[i].len ]!=-1 );
		reverse(rt[i].road,rt[i].len);
	}
	rts->path=rt;
	rts->num=paths;
	rts->del=tp;
	rts->sz=paths*pv;
	rts->used=paths*pv;
	delete []path_all;
	delete []path;
	delete []wgt;
	return rts;
}
/**
 * For normal djikstr, no equal shortest path;
 * **/
Paths* Topo::djikstr(int src,int mode){
	int *w = NULL;
	int *prior=djikstr_prior(src,w);
	Paths* tp= select_path(prior,w,mode);
	delete []prior;
	return tp;

}
int* Topo::djikstr_prior(int src,int* &p){
	const int inf=0x3ffffff;
	const int ql=vs*vs;
	int i=0,j=0;
	int *len=new int [vs];
	int *prior=new int [vs*vs]; //record the shortest path DGA ,prior node.
	int *queue=new int [ql],op=0,ed=0;
	memset(len,0,sizeof(int)*vs);
	memset(prior,0,sizeof(int)*vs*vs);
	p=len;
	for(i=0; i<vs; i++) 
		len[i]=inf;
	len[src]=0; 
	// add src to queue
	queue[op]=src;
	++ed;
	int* base = 0;
		while(op != ed){
			int tmp=ed;
			// looking for new node to add : weight is less.
			for(; op != tmp;  op = ++op %ql){
				int lka=queue[op],lkb=0;
				base = adj+queue[op]*vs;
				for(j=0; base[j] != ED && j<vs; j++){
					lkb = base[j];
					int newei = wei[ lka*vs + lkb ]+len[lka] ;
					if( newei < len[ lkb ] ){
						len[ lkb ] = wei[ lka*vs+lkb ] + len[lka];
						queue[ed]= base[j];
						ed = ++ed % ql;
						// delete all recorded other used path
						prior[ lkb*vs ] = 1;
						prior[ lkb*vs + prior[ lkb*vs ]  ]=lka;
					}else if (newei == len[ lkb ]){
						++prior[ lkb*vs ];
						prior[ lkb*vs + prior[ lkb*vs ] ]=lka;
					}
				}
			}
		}
	delete [] queue;
	return prior;
}
