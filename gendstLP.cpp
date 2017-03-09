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
typedef struct{int i;int j;int c;int w;} t_edge;
const int IMAX=0x7fffffff;
const int N=20;
const int E=50;
const double CP=100000.0;
const double rr=0.0000001;
t_edge linkk[E];
vector<int> adj[N];
set<int>hs;

int Aeq[N*N+N*E][N*E+1];
int beq[N*N+N*E];
int T[N][N];
double A1[E][N*E+1];
double ct[N*E+1];

int nn,ne;
int var;


int pre[N];
int visited[N];
int dist[N][N];
int vmt[N][N];// weight matrix

int hm[N][N];
int spfhm[N][E];
int dep[N];
int getgC(int j,int e)
{
    return j*ne+e;
}

void createGraph(char *edgef)
{   
      FILE *fin=fopen(edgef,"r");
      int a,b,c,w;
      ne=0;
      while(fscanf(fin,"%d%d%d%d",&a,&b,&c,&w)!=EOF)
      {  
          a--;
          b--;
          hs.insert(a);
          hs.insert(b);
            vmt[a][b]=w;
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
    var=nn*ne+1;
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
int getMini(int s)
{
    int mind=IMAX;
    int mini=-1;
    int i;
    for(i=0;i<nn;i++)
    {
        if(visited[i]==1) continue;
        if(dist[s][i]<mind)
        {
            mind=dist[s][i];
            mini=i;
            //printf("mini=%d s=%d",mini,s);
        }
    }
    if(mini==-1) printf("dijk error %d\n",mind);

    return mini;
}
void path(int s, int d)
{
    int now=pre[d];
    int last=d;
    while(now!=-1)
    {
        int lid=hm[now][last];
        spfhm[d][lid]=1;
        last=now;
        now=pre[now];
    }
}

void dijkstra2(int s)
{
    for(int i=0;i<=nn;i++)
    {
        dist[s][i]=IMAX;
        visited[i]=0;
        pre[i]=0;
        
    }
    
    dist[s][s]=0;
    //visited[s]=1;
    pre[s]=-1;
    int mind=IMAX;
    int mini=-1;
    for(int i=0;i<nn;i++)
    {
        mini=getMini(s);
       
        visited[mini]=1;
        mind=dist[s][mini];
        int si=adj[mini].size();
        for(int j=0;j<si;j++)
        {
            int ele=adj[mini][j];
            if(vmt[mini][ele]+dist[s][mini]<dist[s][ele])
            {
                dist[s][ele]=dist[s][mini]+vmt[mini][ele];
                pre[ele]=mini;
            }
        }
    }
    for(int i=0;i<nn;i++)
    {
        if(i==s) continue;
        path(s,i);
    }
}


void genAeqbeq()
{
    int r=0;
    for(int j=0;j<nn;j++)
    {
        for(int m=0;m<nn;m++)
        {
            if(m==j) continue;
            int si=adj[m].size();
            for(int k=0;k<si;k++)
            {
                int ele=adj[m][k];
                int eid=hm[m][ele];
                int col=getgC(j,eid);
                Aeq[r][col]=1;
                eid=hm[ele][m];
                col=getgC(j,eid);
                Aeq[r][col]=-1;
            }
        
            beq[r]=T[m][j];
           
            r++;
        }
    }
 
   
    for(int j=0;j<nn;j++)//destination
    {
        for(int m=0;m<nn;m++)//node
        {
            if(m==j) continue;
            if(dep[m]==1)
            {
                int si=adj[m].size();
                for(int k=0;k<si;k++)
                {
                    int ele=adj[m][k];
                    int rlid=hm[ele][m];
                    if(spfhm[j][rlid]==1)
                    {
                        int lid=hm[m][ele];
                        int col=getgC(j,lid);
                        //printf("%d %d %d %d\n",j,m,r,col);
                        Aeq[r][col]=1;
                        beq[r]=0;
                        r++;
                    }
                }

            }
            else
            {
                int si=adj[m].size();
                for(int k=0;k<si;k++)
                {
                    int ele=adj[m][k];
                    int lid=hm[m][ele];
                    if(spfhm[j][lid]!=1)
                    {
                        int col=getgC(j,lid);
                        //printf("%d %d %d %d\n",j,m,r,col);
                        Aeq[r][col]=1;
                        beq[r]=0;
                        r++;
                    }
                }
            }
        }
    }

    printf("Aeq r=%d\n",r);
    FILE *fAeq=fopen("Aeq.txt","w");
    for(int i=0;i<r;i++)
    {
        for(int j=0;j<var;j++)
        {
            fprintf(fAeq,"%d ",Aeq[i][j]);
        }
        fprintf(fAeq,"\n");
    }
    
    FILE *fbeq=fopen("beq.txt","w");
    for(int i=0;i<r;i++)
    {
       fprintf(fbeq,"%d\n",beq[i]);
    }


}
void genAb()
{
    int r1=0;
    for(int e=0;e<ne;e++)
    {
        for(int j=0;j<nn;j++)
        {
            int col=getgC(j,e);
            A1[r1][col]=1.0*CP/linkk[e].c;
        }
        A1[r1][nn*ne]=-1;
        r1++;
    }
    
       
    printf("A r1=%d\n",r1);
    FILE *fA1=fopen("A1.txt","w");
    for(int i=0;i<r1;i++)
    {
        for(int j=0;j<var;j++)
        {
            fprintf(fA1,"%lf ",A1[i][j]);
        }
        fprintf(fA1,"\n");
    }
    
}
void genobjulbl()
{

    for(int j=0;j<nn;j++)
    {
        for(int e=0;e<ne;e++)
        {
            int col=getgC(j,e);
            ct[col]=rr*CP*1.0/linkk[e].c;
        }
    }
    ct[nn*ne]=1;
    FILE *fct=fopen("ct.txt","w");
    for(int i=0;i<var;i++)
    {
        fprintf(fct,"%lf ",ct[i]);
    }
    fprintf(fct,"\n");

    FILE *fpar=fopen("par.txt","w");
       fprintf(fpar,"%d %d %d %lf\n",nn,ne,var,CP);


}
void readDep(char *p)
{
    for(int i=0;i<N;i++)
    {
        dep[i]=0;
    }
    FILE *fin=fopen(p,"r");
    double tmp;
    int x;
    while(fscanf(fin,"%lf",&tmp)!=EOF)
    {
        x=tmp+0.00001;
        x--;
        dep[x]=1;
    }
}
int main(int argc, char *argv[])
{
    createGraph(argv[1]);
 
    inputTraffic(argv[2]);
    readDep("deploy"); 
    printf("read over nn=%d ne=%d var=%d\n\n",nn,ne,var);
    for(int i=0;i<nn;i++)
    {
        dijkstra2(i);
    }
    
    genAeqbeq(); 
    genAb();
    genobjulbl();
} 
