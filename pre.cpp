#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<vector>
#include<map>
#include<string>
#include<queue>
#include<fstream>
#include<set>

using namespace std;
typedef struct{int i;int j;int c;} t_edge;
const int N=20;
const int E=50;
const double PREC=0.00000001;

t_edge linkk[E];
map<int,map<int,int> >hm;
vector<int> adj[N];
set<int>hs;

int T[N][N];
double x[N*E];
int visited[N];
int father[N];
int nn,ne;
int xn;
double trfe[E];
bool sumgflag=true;
int getgC(int j,int e)
{
    return j*ne+e;
}

int getj(int xid)
{
    return xid/ne;
}

int gete(int xid)
{
    return xid%ne;
}
void createGraph(char *edgef)
{   
      FILE *fin=fopen(edgef,"r");
      int a,b,c;
      ne=0;
      while(fscanf(fin,"%d%d%d",&a,&b,&c)!=EOF)
      {  
          a--;
          b--;
          hs.insert(a);
          hs.insert(b);
     
          if(a!=b)
          {   
              adj[a].push_back(b);
              linkk[ne].i=a;
              linkk[ne].j=b;
              linkk[ne].c=c;
              hm[a][b]=ne;
              ne++;

           
          }
      }
    nn=hs.size();
    xn=nn*ne;
}
void inputTraffic(char *p)
{
    FILE *fin=fopen(p,"r");
    int a,b;
    double c;
    while(fscanf(fin,"%d%d%lf",&a,&b,&c)!=EOF)
    {   
        a--;
        b--;
        if(a==b) continue;
        T[a][b]=c;
      }
    

}

void readX(char *p)
{
    FILE *fin=fopen(p,"r");

    for(int i=0;i<nn*ne;i++)
    {
        fscanf(fin,"%lf",&x[i]);
    }
}

bool DFS(int s,int d,int k)
{
    int si=adj[k].size();
    int tmp;
    visited[k]=1;
    for(int i=0;i<si;i++)
    {
        tmp=adj[k][i];
        int lid=hm[k][tmp];
        int col=getgC(d,lid);
        if((visited[tmp]==0)&&(x[col]>0))
        {
            father[tmp]=k;
            bool flag= DFS(s,d,tmp);
            if(flag==false) return false;
        }
        else if((visited[tmp]==1)&&(x[col]>0))
        {
            int p=father[k];
            int r=k;
            double mt=x[col];
            while(r!=tmp)
            {
                int lidd=hm[p][r];
                int coll=getgC(d,lidd);
                if(x[coll]<mt)
                {
                    mt=x[coll];
                }
                r=p;
                p=father[r];
            }

            x[col]-=mt;
            p=father[k];
            r=k;
            while(r!=tmp)
            {
                int lidd=hm[p][r];
                int coll=getgC(d,lidd);
                x[coll]-=mt;
                r=p;
                p=father[r];
            }
     //       printf("false %d %d %lf\n",k,tmp,mt);
            return false;        
        }    
    
    }
    visited[k]=2;
    return true;
}
void removeLoop()
{
    bool flag2=true;
    do
    {
        flag2=true;
        for(int i=0;i<nn;i++)
        {
            for(int j=0;j<nn;j++)
            {
                bool flag=true;
                do
                {
                    for(int k=0;k<N;k++)
                    {
                        visited[k]=0;
                        father[k]=-1;
                    }
                    flag=DFS(i,j,i);
                    if(flag==false) flag2=false;
                }while(flag==false);
               if(visited[j]!=2)
               {
                   printf("disconn %d %d %d\n",i,j,visited[j]);
               }
            }
        }

    }while(flag2==false);
}
void dfs(int s,int d,double tf,int level)
{
    double g[10];
    //if(level>3000) printf("%d\n",level);
 //   if(d==7) printf("%d %d %lf %d\n",s,d,tf,level);
   // if((d==7)&&(level>=10)) return;
    if(s==d) return;
    int si=adj[s].size();
    double sumg=0;
    for(int k=0;k<si;k++)
    {
        int ele=adj[s][k];
        int eid=hm[s][ele];
        int col=getgC(d,eid);
        g[k]=x[col];
        sumg+=g[k];
    }
    if(sumg<PREC)
    {   sumgflag=false;
        printf("sumg %lf\n",sumg);
    }
    for(int k=0;k<si;k++)
    {
        int ele=adj[s][k];
        int eid=hm[s][ele];
        double delta=tf*g[k]*1.0/sumg;
        //if(g[k]<PREC) continue;
        if(delta>PREC)
        {
            dfs(ele,d,delta,level+1); 
            trfe[eid]+=delta;
        }
    } 
}

int main(int argc, char *argv[])
{
    createGraph(argv[1]);
    inputTraffic(argv[2]);
    readX("x.txt");
    printf("read over nn=%d ne=%d xn=%d\n\n",nn,ne,xn);
    removeLoop();
    printf("remove loop over\n");

    /*for(int i=0;i<ne;i++)
    {
        printf("%d %d %lf\n",linkk[i].i,linkk[i].j,x[getgC(7,i)]);
    }*/
  //  return 0;
    for(int i=0;i<nn;i++)
    {
        for(int j=0;j<nn;j++)
        {

          //  printf("begin %d %d %d\n",i,j,T[i][j]);
            if(T[i][j]>0)
            {
                dfs(i,j,T[i][j],1);
            }
        }
    }
    double maxt=-1;
    int maxi=-1;
    for(int i=0;i<ne;i++)
    {
        double uti=trfe[i]*1.0/linkk[i].c;
        if(uti>maxt) 
        {
            maxt=uti;
            maxi=i+1;
        }
    }
    printf("%lf %d %d\n",maxt,sumgflag,maxi);
    FILE *fout=fopen("pre.txt","w");
    fprintf(fout,"%lf %d %d\n",maxt,sumgflag,maxi);
} 
